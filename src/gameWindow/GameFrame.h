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

class GameData;

typedef enum{
    OnClosed,
    OnPosting,
    OnRunning
} GameState;


class GameFrame : public QMainWindow{
    Q_OBJECT
    GameData* data;

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

private slots:
    void takeFlash();
public:
    void normalSize();
    void miniSize();
    explicit GameFrame(GameData*);
};

struct GameData {
    GameFrame *game;
    GameState state;
    HWND hwnd;
    AccData acc;
    int runnerTeamIndex;
    int distIndex;
    bool lockFree;

    explicit GameData(AccData, int rti=0);

    void doReLogin();
    void doClose();
    void doLogin();
    void free();
};


extern QMultiHash<QString, GameData*> memberGames;

#endif //ZEROHELPER_GAMEFRAME_H
