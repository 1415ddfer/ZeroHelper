//
// Created by 17834 on 2023/8/31.
//

#ifndef ZEROHELPER_GAMEMANAGER_H
#define ZEROHELPER_GAMEMANAGER_H

#include "../gameWindow/GameFrame.h"
#include <QReadWriteLock>

class GameInfo : public QObject{
Q_OBJECT;
    HWND hwnd;
    GameState state;
    QReadWriteLock mutex; // 用于线程安全的互斥锁
public slots:
    void setWId(HWND newWid);
    void setState(GameState newState);
public:
    explicit GameInfo();
    HWND getWId();
    GameState  getGameState();
};

class GameManager : QObject{
Q_OBJECT;
    QMap<QString, GameFrame*> frames;
    QMap<QString, GameInfo*> infos;
    QReadWriteLock mutex; // 用于线程安全的互斥锁
public slots:
    void removeGame(AccData* acc);
public:
    explicit GameManager();
    ~GameManager();
    bool createGame(AccData* acc);
    GameInfo* getInfo(AccData* acc);
    void closeGame(AccData* acc);
    WId getWindowId(AccData* acc);
};

extern GameManager gameManager;

#endif //ZEROHELPER_GAMEMANAGER_H
