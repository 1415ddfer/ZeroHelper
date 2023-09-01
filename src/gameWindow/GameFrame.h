//
// Created by 17834 on 2023/6/16.
//

#ifndef ZEROHELPER_GAMEFRAME_H
#define ZEROHELPER_GAMEFRAME_H


#include <QWidget>
#include <QProcess>
#include <QMainWindow>
#include <QStackedWidget>
#include <QTextEdit>
#include <QMenuBar>
#include <QStackedLayout>
#include <QtWidgets/QVBoxLayout>

#include <windows.h>
#include <QFutureWatcher>

#include "../network/LoginGamePost.h"
#include "../playerData/TeamManager.h"
#include "../loginWindow/ArtButton.h"



typedef enum{
    OnClosed,
    OnLoading,
    OnPosting,
    OnCallingFlash,
    OnRunning
} GameState;


class GameFrame : public QMainWindow{
    Q_OBJECT;
    AccData accData;

    bool isCloseByManager;

    QWidget mWidget;
    QVBoxLayout mLayout;
    QTextEdit logBox;
    QWidget *flashSaShadow;
    QWindow *flashSa;
    QFuture<HWND> future;
    QFutureWatcher<HWND> watcher;
//    QPixmap background;
//    void paintEvent(QPaintEvent*) override;
    void closeEvent(QCloseEvent* e) override;

    HWND loginGame();
    signals:
    void freeGame(AccData*);
    void updateWId(HWND);
    void updateState(GameState);
private slots:
    void takeFlash();
public:
    void normalSize();
    void miniSize();
    void closeByManager();
    explicit GameFrame(AccData*);
};



#endif //ZEROHELPER_GAMEFRAME_H
