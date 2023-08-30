//
// Created by 17834 on 2023/6/16.
//

#include "GameFrame.h"
#include <QWindow>
#include <QScreen>
#include <QApplication>

#include <ctime>
#include <direct.h>

#include <QMessageBox>
#include <QtConcurrent>

QMultiHash<QString, GameData *> memberGames;


GameData::GameData(AccData acc0, int rti) {
    acc = std::move(acc0);
    runnerTeamIndex = rti;
    state = OnClosed;
    lockFree = false;
}

void GameData::doLogin() {
    judgeMessage(state == OnClosed, QString("重复登录:%1").arg(acc.nickName))
    game = new GameFrame(this);
    game->show();
}

void GameData::free() {
    if (lockFree) return;
    memberGames.remove(acc.userName, this);
    delete this;
}

GameFrame::GameFrame(GameData *d) :
        mWidget(this),
        mLayout(&mWidget),
        logBox(&mWidget) {
    data = d;
    setCentralWidget(&mWidget);
    mLayout.setSpacing(0);
    mLayout.setContentsMargins(0, 0, 0, 0);
    mLayout.addWidget(&logBox);
    logBox.setAttribute(Qt::WA_TransparentForMouseEvents, true); //设置该属性窗口的子控件不能再响应鼠标消息

    logBox.append("窗体加载完成");

     future = QtConcurrent::run([this]() {
        return loginGame();
    });
    connect(&watcher, &QFutureWatcher<QString>::finished, this, &GameFrame::takeFlash);

    // 将QFuture对象设置给QFutureWatcher对象，开始监控
    watcher.setFuture(future);

}

void GameFrame::normalSize() {
    resize(1000, 600);
}

void GameFrame::miniSize() {
    resize(500, 300);
}

HWND GameFrame::loginGame() {
    data->state = OnPosting;
    auto src = QString{};
    switch (data->acc.providerId) {
        default: {
            logBox.append("该服务器暂未收录");
            return nullptr;
        }
        case 0: {
            if (LoginGamePost::login4399(&data->acc, &logBox, &src)) break;
            logBox.append("自动登录失败，启用浏览器模式登录");
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
    // 这个exe 的目录必须保持干净，不然会出现参数-reluacher导致出现两个进程
    sei.lpFile = L"plugin\\normal.exe"; // 设置要启动的程序
    sei.lpParameters = url; // 设置要传递的参数
    sei.nShow = SW_MINIMIZE; // 设置显示命令为最小化

    if (ShellExecuteEx(&sei)) // 调用函数
    {
        pid = GetProcessId(sei.hProcess); // 获取PID
        printf("The PID of test.exe is %u\n", pid); // 打印PID
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
        logBox.append("创建进程失败:" + result);
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
                    qDebug() << (WId)hwnd;
                    return hwnd;
                }
            }
            hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
        }
        logBox.append(QString("获取失败:[%1]").arg(count));
        // 异步延迟，弃用，因实际上该操作在子线程
        count++;
    }
    logBox.append("捕获失败");
    return nullptr;
}

void GameFrame::closeEvent(QCloseEvent *e) {
    data->free();
    QWidget::closeEvent(e);
}

void GameFrame::takeFlash() {
    auto hwnd = future.result();
    if(hwnd == nullptr) return;
    logBox.setVisible(false);
    qDebug() << (WId)hwnd;
    flashSa = QWindow::fromWinId((WId) hwnd);
//        double dpi  = screen()->logicalDotsPerInch() / 96;
//        qDebug() << dpi;
//        flashSa->resize(1000/dpi, 600/dpi);
    flashSaShadow = QWidget::createWindowContainer(flashSa);
    flashSaShadow->setParent(&mWidget);
    mLayout.addWidget(flashSaShadow);
    auto time0 = time(nullptr);
    while (time(nullptr) - time0 < 1) {
        QApplication::processEvents();
    }
    MoveWindow((HWND) winId(), 0, 0, 1000, 600, 0);
    resize(1000, 600);
}

