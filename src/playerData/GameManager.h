//
// Created by 17834 on 2023/8/31.
//

#ifndef ZEROHELPER_GAMEMANAGER_H
#define ZEROHELPER_GAMEMANAGER_H


#include <QReadWriteLock>
#include <QObject>
#include <QMap>
#include "../gameWindow/GameState.h"
#include "TeamManager.h"

class GameFrame;



class GameInfo : public QObject{
Q_OBJECT;
    unsigned long long hwnd;
    GameState state;
    QReadWriteLock *mutex; // 用于线程安全的互斥锁
public slots:
    void setWId(unsigned long long newWid);
    void setState(GameState newState);
public:
    explicit GameInfo(QReadWriteLock *);
    unsigned long long getWId();
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
    ~GameManager() override;
    bool createGame(AccData *acc);
    GameInfo* getInfo(AccData* acc);
    bool counterInfo(AccData* acc);
    void closeGame(AccData* acc);
};

extern GameManager gameManager;

#endif //ZEROHELPER_GAMEMANAGER_H
