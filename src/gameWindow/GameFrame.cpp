//
// Created by 17834 on 2023/6/16.
//

#include "GameFrame.h"
#include "../network/LoginGamePost.h"
#include <windows.h>
#include <QWindow>
#include <QMenuBar>
#include <QApplication>
#include <QtConcurrent>


GameFrame::GameFrame(AccData *d) :
        mWidget(this),
        mLayout(&mWidget),
        logBox(&mWidget) {
    accData = *d;
    flashSaShadow = nullptr;
    flashSa = nullptr;

    setCentralWidget(&mWidget);
    mLayout.setSpacing(0);
    mLayout.setContentsMargins(0, 0, 0, 0);
    mLayout.addWidget(&logBox);
    logBox.setAttribute(Qt::WA_TransparentForMouseEvents, true); //设置该属性窗口的子控件不能再响应鼠标消息

    auto menu = menuBar();
    auto mSize = new QMenu("窗口大小", menu);
    auto aSizeMini = new QAction("迷你", mSize);
    connect(aSizeMini, &QAction::triggered, this, &GameFrame::toMiniSize);
    auto aSizeNormal = new QAction("常规", mSize);
    connect(aSizeNormal, &QAction::triggered, this, &GameFrame::toNormalSize);
    mSize->addAction(aSizeNormal);
    mSize->addAction(aSizeMini);
    menu->addMenu(mSize);

    logBox.append("窗体加载完成");
    emit updateState(OnLoading);
     future = QtConcurrent::run([this]() {
        return loginGame();
    });
    connect(&watcher, &QFutureWatcher<QString>::finished, this, &GameFrame::takeFlash);

    // 将QFuture对象设置给QFutureWatcher对象，开始监控
    watcher.setFuture(future);

}

HWND GameFrame::loginGame() {
    auto src = QString{};
    switch (accData.providerId) {
        default: {
            logBox.append("该服务器暂未收录");
            return nullptr;
        }
        case 0: {
            if (LoginGamePost::login4399(&accData, &logBox, &src)) break;
            logBox.append("自动登录失败，启用浏览器模式登录");
            return nullptr;
        }
    }

    auto buff = src.toLatin1();
    auto url_char = buff.data();
    int len = MultiByteToWideChar( CP_ACP, 0, url_char, strlen(url_char), nullptr, 0);
    auto * url = new wchar_t[len+1];
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
        flashSaPid = GetProcessId(sei.hProcess); // 获取PID
        printf("The PID of test.exe is %lu\n", flashSaPid); // 打印PID
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
                if (flashSaPid == dwProcessID && GetParent(hwnd) == nullptr && ::IsWindowVisible(hwnd))
                {
                    return hwnd;
                }
            }
            hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
        }
        logBox.append(QString("获取失败:[%1]").arg(count));
        count++;
    }
    logBox.append("捕获失败");
    return nullptr;
}

void GameFrame::closeEvent(QCloseEvent *e) {
    emit freeGame(&accData);
    auto pid = flashSaPid;
    QWidget::closeEvent(e);
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid); // 获取进程句柄
    if (hProcess == nullptr) // 判断是否获取成功
    {
        qDebug() << "OpenProcess failed: " << GetLastError();
    }
    if (TerminateProcess(hProcess, 0)) // 终止进程
    {
        qDebug() << "TerminateProcess succeeded.";
    }
    else
    {
        qDebug() << "TerminateProcess failed: " << GetLastError();
    }
    CloseHandle(hProcess); // 关闭句柄
}

void GameFrame::takeFlash() {
    auto hwnd = future.result();
    if(hwnd == nullptr)
    {
        emit updateState(OnError);
        return;
    }
    logBox.setVisible(false);
    emit updateWId((unsigned long long)hwnd);
    qDebug() << (WId)hwnd;
    flashSa = QWindow::fromWinId((WId) hwnd);
    flashSaShadow = QWidget::createWindowContainer(flashSa);
//    flashSaShadow->setParent(&mWidget);
    mLayout.addWidget(flashSaShadow);
    auto time0 = time(nullptr);
    while (time(nullptr) - time0 < 1) {
        QApplication::processEvents();
    }
    // must
    MoveWindow((HWND) winId(), 0, 0, 1000, 600, 0);
    // must
    resize(1000, 621);

    emit updateState(OnRunning);
}

void GameFrame::toNormalSize() {
    resize(1000, 621);
    auto hwnd = (HWND)flashSa->winId();
    SendMessage(hwnd, WM_COMMAND, 20046, 0);
}

void GameFrame::toMiniSize() {
    resize(500, 321);
    auto hwnd = (HWND)flashSa->winId();
    SendMessage(hwnd, WM_COMMAND, 20034, 0);
}

