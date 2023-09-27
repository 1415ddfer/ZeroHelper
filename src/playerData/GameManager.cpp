//
// Created by 17834 on 2023/8/31.
//

#include "GameManager.h"
#include "../gameWindow/GameFrame.h"

GameManager gameManager;

bool GameManager::createGame(AccData *acc) {
    auto key = QString::number(acc->providerId) + acc->serverId + acc->userName;
    bool result = false;
    mutex.lockForWrite();
    if(!frames.count(key)){
        auto game = new GameFrame(acc);
        frames.insert(key, game);
        connect(game, SIGNAL(freeGame(AccData*)), this, SLOT(removeGame(AccData*)));
        auto info = new GameInfo(&mutex);
        connect(game, SIGNAL(updateState(GameState)), info, SLOT(setState(GameState)));
        connect(game, SIGNAL(updateWId(unsigned long long)), info, SLOT(setWId(unsigned long long)));
        infos.insert(key, info);
        game->show();
        result = true;
    } else{
        qDebug() << "waring: " << acc->nickName << " is already login";
    }
    mutex.unlock();
    return result;
}

GameInfo* GameManager::getInfo(AccData *acc) {
    auto key = QString::number(acc->providerId) + acc->serverId + acc->userName;
    mutex.lockForRead();
    auto result = infos[key];
    mutex.unlock();
    return result;
}

void GameManager::closeGame(AccData *acc) {
    auto key = QString::number(acc->providerId) + acc->serverId + acc->userName;
    mutex.lockForRead();
    auto *game = frames[key];
    mutex.unlock();
    if(game == nullptr) {
        qDebug() << "waring: closing not found key:" << key;
        return;
    }
    game->close();
}

void GameManager::removeGame(AccData *acc) {
    // warning: need close game before
    auto key = QString::number(acc->providerId) + acc->serverId + acc->userName;
    mutex.lockForRead();
    auto *game = frames[key];
    mutex.unlock();
    if(game == nullptr) {
        qDebug() << "waring: removing not found key:" << key;
        return;
    }
//    game->deleteLater();
    mutex.lockForWrite();
    frames.remove(key);
    auto info = infos[key];
    info->deleteLater();
    infos.remove(key);
    mutex.unlock();
    qDebug() << "done";
}

GameManager::GameManager() : QObject(){

}

GameManager::~GameManager(){
    for (auto it = frames.begin(); it != frames.end(); ++it){
        it.value()->close();
        it.value()->deleteLater();
    }
    for (auto it = infos.begin(); it != infos.end(); ++it){
        it.value()->deleteLater();
    }
//    QObject::~QObject();
}

void GameInfo::setWId(unsigned long long newWid) {
    mutex->lockForWrite();
    hwnd = newWid;
    mutex->unlock();
}

void GameInfo::setState(GameState newState) {
    mutex->lockForWrite();
    state = newState;
    mutex->unlock();
}

unsigned long long GameInfo::getWId() {
    unsigned long long result;
    mutex->lockForRead();
    result = hwnd;
    mutex->unlock();
    return result;
}

GameState GameInfo::getGameState() {
    GameState result;
    mutex->lockForRead();
    result = state;
    mutex->unlock();
    return result;
}

GameInfo::GameInfo(QReadWriteLock *m) {
    mutex = m;
    hwnd = 0;
    state = OnClosed;
}
