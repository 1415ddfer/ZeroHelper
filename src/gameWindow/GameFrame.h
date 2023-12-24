//
// Created by 17834 on 2023/6/16.
//

#ifndef ZEROHELPER_GAMEFRAME_H
#define ZEROHELPER_GAMEFRAME_H


#include <QWidget>
#include <QMainWindow>
#include <QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QFutureWatcher>

#include "../playerData/TeamManager.h"
#include "GameState.h"

struct MathTable;

class GameFrame : public QMainWindow{
    Q_OBJECT;
    AccData accData;

    QWidget mWidget;
    QVBoxLayout mLayout;
    QTextEdit logBox;
    ulong flashSaPid;
    QWidget *flashSaShadow;
    QWindow *flashSa;
    MathTable* mathTable;
    QFuture<HWND> future;
    QFutureWatcher<HWND> watcher;
//    QPixmap background;
//    void paintEvent(QPaintEvent*) override;
    void closeEvent(QCloseEvent* e) override;
    void moveEvent(QMoveEvent* e) override;

    void resizeGame(int w, int h);
    void ruleManage();

    HWND loginGame();
signals:
    void freeGame(AccData*);
    void updateWId(unsigned long long);
    void updateState(GameState);
    void updateSizeState(bool);
private slots:
    void takeFlash();
    void toNormalSize();
    void toMiniSize();
    void toCloseGame();
public:
    explicit GameFrame(AccData*);
};



#endif //ZEROHELPER_GAMEFRAME_H
