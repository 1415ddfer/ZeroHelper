//
// Created by 17834 on 2023/9/24.
//

#ifndef ZEROHELPER_GAMEFRAMENEW_H
#define ZEROHELPER_GAMEFRAMENEW_H


#include <QWindow>
#include <QFutureWatcher>
#include "../playerData/TeamManager.h"
#include "GameState.h"


class GameFrameNew: public QWindow {
    Q_OBJECT;

    AccData accData;

    QList<QString> textData;
    QWindow *flashSa;
    QBackingStore* m_store;
    QFuture<HWND> future;
    QFutureWatcher<HWND> watcher;

    void render(QPainter *painter); // 渲染文本
    HWND loginGame();
signals:
    void freeGame(AccData*);
    void updateWId(HWND);
    void updateState(GameState);
private slots:
    void takeFlash();
    void renderLater(); // 延迟渲染
    void renderNow(); // 立即渲染
protected:
    void closeEvent(QCloseEvent *e) override;
//    bool event(QEvent *e) override;
    void resizeEvent(QResizeEvent *event) override;
    void focusInEvent(QFocusEvent *e) override;
//    void exposeEvent(QExposeEvent *event) override;
public:
    void normalSize();
    void miniSize();
    explicit GameFrameNew(AccData*);
};


#endif //ZEROHELPER_GAMEFRAMENEW_H
