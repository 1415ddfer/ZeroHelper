//
// Created by 17834 on 2023/8/31.
//

#include "GameManager.h"

GameManager gameManager;

bool GameManager::createGame(AccData *acc) {
    auto key = QString::number(acc->providerId) + acc->serverId + acc->userName;
    bool result = false;
    mutex.lockForWrite();
    if(!frames.count(key)){
        auto game = new GameFrame(acc);
        frames.insert(key, game);
        connect(game, SIGNAL(freeGame(AccData*)), this, SLOT(removeGame(AccData*)));
        auto info = new GameInfo();
        connect(game, SIGNAL(updateState(GameState)), info, SLOT(setState(GameState)));
        connect(game, SIGNAL(updateWId(HWND)), info, SLOT(setWId(HWND)));
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
    // warning: need close game before
    auto key = QString::number(acc->providerId) + acc->serverId + acc->userName;
    mutex.lockForWrite();
    auto *game = frames[key];
    if(game == nullptr) {
        qDebug() << "waring: closing not found key:" << key;
        mutex.unlock();
        return;
    }
    game->closeByManager();
    game->deleteLater();
    frames.remove(key);
    auto info = infos[key];
    info->deleteLater();
    infos.remove(key);
    mutex.unlock();
}

void GameManager::removeGame(AccData *acc) {
    // warning: need close game before
    auto key = QString::number(acc->providerId) + acc->serverId + acc->userName;
    mutex.lockForWrite();
    auto *game = frames[key];
    if(game == nullptr) {
        qDebug() << "waring: removing not found key:" << key;
        mutex.unlock();
        return;
    }
    game->deleteLater();
    frames.remove(key);
    auto info = infos[key];
    info->deleteLater();
    infos.remove(key);
    mutex.unlock();
}

GameManager::GameManager() : QObject(){

}

GameManager::~GameManager(){
    for(auto game : frames.values()){
        game->closeByManager();
        game->deleteLater();
    }
    for(auto info : infos.values()){
        info->deleteLater();
    }
}

void GameInfo::setWId(HWND newWid) {
    mutex.lockForWrite();
    hwnd = newWid;
    mutex.unlock();
}

void GameInfo::setState(GameState newState) {
    mutex.lockForWrite();
    state = newState;
    mutex.unlock();
}

HWND GameInfo::getWId() {
    HWND result;
    mutex.lockForRead();
    result = hwnd;
    mutex.unlock();
    return result;
}

GameState GameInfo::getGameState() {
    GameState result;
    mutex.lockForRead();
    result = state;
    mutex.unlock();
    return result;
}

GameInfo::GameInfo() {
    hwnd = nullptr;
    state = OnClosed;
}
