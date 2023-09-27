//
// Created by 17834 on 2023/9/24.
//

#include <QPainter>
#include <QBackingStore>
#include <QPaintEvent>
#include <QtConcurrent>
#include <windows.h>
#include "GameFrameNew.h"
#include "../network/LoginGamePost.h"

void GameFrameNew::normalSize() {
//    flashSa->resize(1000, 600);
    resize(1000, 600);
}

void GameFrameNew::miniSize() {
    resize(500, 300);
}

GameFrameNew::GameFrameNew(AccData *d):
flashSa{}
{
    accData = *d;
    m_store = new QBackingStore(this);
    miniSize();
    textData.append("窗体加载完成");
    emit updateState(OnLoading);
    future = QtConcurrent::run([this]() {
        return loginGame();
    });
    connect(&watcher, &QFutureWatcher<QString>::finished, this, &GameFrameNew::takeFlash);
    watcher.setFuture(future);
}

HWND GameFrameNew::loginGame() {
    auto src = QString{};
    switch (accData.providerId) {
        default: {
            textData.append("该服务器暂未收录");
            return nullptr;
        }
        case 0: {
            if (LoginGamePost::login4399(&accData, &textData, &src)) break;
            textData.append("自动登录失败，启用浏览器模式登录");
            return nullptr;
        }
    }

    auto buff = src.toLatin1();
    auto url_char = buff.data();
    int len = MultiByteToWideChar( CP_ACP, 0, url_char, strlen(url_char), nullptr, 0);
    auto * url = new wchar_t[len+1];
    DWORD pid;
    MultiByteToWideChar( CP_ACP, 0, url_char, strlen( url_char), url, len);
    url[len]= '\0' ;
    SHELLEXECUTEINFO sei = {0}; // 初始化结构体
    sei.cbSize = sizeof(SHELLEXECUTEINFO); // 设置结构体大小
    sei.fMask = SEE_MASK_NOCLOSEPROCESS; // 设置标志位
    // 这个exe 的目录必须保持干净，不然会出现参数-relaunch导致出现两个进程
    sei.lpFile = L"plugin\\normal.exe"; // 设置要启动的程序
    sei.lpParameters = url; // 设置要传递的参数
    sei.nShow = SW_MINIMIZE; // 设置显示命令为最小化

    if (ShellExecuteEx(&sei)) // 调用函数
    {
        pid = GetProcessId(sei.hProcess); // 获取PID
        printf("The PID of test.exe is %lu\n", pid); // 打印PID
        CloseHandle(sei.hProcess); // 关闭进程句柄
    }
    else
    {
        LPWSTR bufPtr = nullptr;
        DWORD err = GetLastError();
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       nullptr,
                       err,
                       0,
                       (LPWSTR) &bufPtr,
                       0,
                       nullptr);
        const QString result = (bufPtr) ? QString::fromUtf16((const char16_t *) bufPtr).trimmed() : QString(
                "Unknown Error %1").arg(err);
        LocalFree(bufPtr);
        textData.append("创建进程失败:" + result);
        return nullptr;
    }

    // 捕获窗口
    HWND hwnd;
    int count = 0;
    while (count < 3) {
        Sleep(1000);
        hwnd = GetTopWindow(nullptr);
        while (hwnd)
        {
            DWORD dwProcessID = 0;
            DWORD dwThreadId = GetWindowThreadProcessId(hwnd, &dwProcessID);
            if (dwThreadId != 0)
            {
                if (pid == dwProcessID && GetParent(hwnd) == nullptr && ::IsWindowVisible(hwnd))
                {
                    return hwnd;
                }
            }
            hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
        }
        textData.append(QString("获取失败:[%1]").arg(count));
        // 异步延迟，弃用，因实际上该操作在子线程
        count++;
    }
    textData.append("捕获失败");
    return nullptr;
}

void GameFrameNew::takeFlash() {
    auto hwnd = future.result();
    if(hwnd == nullptr)
    {
        emit updateState(OnError);
        return;
    }
    emit updateWId(hwnd);
    textData.clear();
    qDebug() << (WId)hwnd;
    flashSa = QWindow::fromWinId((WId) hwnd);
    flashSa->setFlags(flashSa->flags()|Qt::CustomizeWindowHint|Qt::WindowTitleHint);
    flashSa->setParent(this);
    flashSa->showFullScreen();
    flashSa->setX(0);
    flashSa->setY(0);
    setBaseSize(QSize(1000, 600));
    emit updateState(OnRunning);
}

void GameFrameNew::closeEvent(QCloseEvent *e) {
    QWindow::closeEvent(e);
    emit freeGame(&accData);
    QWindow::closeEvent(e);
}

// draw text code

//bool GameFrameNew::event(QEvent *event) {
////    if (event->type() == QEvent::UpdateRequest) { // 如果是更新请求事件
////        renderNow(); // 立即渲染
////        return true;
////    }
//    qDebug() << event->type();
//    return QWindow::event(event);
//}
//
void GameFrameNew::resizeEvent(QResizeEvent *event) {
//    m_store->resize(event->size()); // 调整后备存储的大小
    QWindow::resizeEvent(event);
    if (flashSa == nullptr) return;
    flashSa->resize(event->size());
}

void GameFrameNew::focusInEvent(QFocusEvent *e) {
    QWindow::focusOutEvent(e);
    if (flashSa == nullptr) return;
    flashSa->raise();
}
//
//void GameFrameNew::exposeEvent(QExposeEvent *event) {
//    if (isExposed()) { // 如果窗口可见
//        renderNow(); // 立即渲染
//    }
//    QWindow::exposeEvent(event);
//}

void GameFrameNew::renderNow() {
    if (!isExposed()) return; // 如果窗口不可见，直接返回
    QRect rect(0, 0, width(), height()); // 获取窗口矩形区域
    m_store->beginPaint(rect); // 开始绘制
    if (textData.count())
    {
        QPaintDevice *device = m_store->paintDevice(); // 获取绘制设备
        QPainter painter(device); // 创建画家对象
        render(&painter); // 调用渲染函数
    }
    m_store->endPaint(); // 结束绘制
    m_store->flush(rect); // 刷新窗口显示
}

void GameFrameNew::renderLater() {
    requestUpdate(); // 请求更新窗口
}

void GameFrameNew::render(QPainter *painter) {
    painter->setRenderHint(QPainter::Antialiasing); // 设置抗锯齿
//    painter->fillRect(0, 0, width(), height(), Qt::white); // 填充白色背景
    int x0 = 10, y0 = 5, fontSize = 15;
    painter->setPen(Qt::red); // 设置黑色画笔
    painter->setFont(QFont("Arial", fontSize)); // 设置字体和大小

    for (auto &str0: textData) {
        painter->drawText(x0, y0, str0); // 绘制文本
        y0 += y0;
    }
    painter->end();
}
