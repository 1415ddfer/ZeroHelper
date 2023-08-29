//
// Created by 17834 on 2023/6/29.
//

#include "LoginGamePost.h"


bool LoginGamePost::login4399(AccData *acc, QTextEdit *log, QString *ret) {
    HttpManager http;
    auto ser = Services{};
    ServicesManager::GetService(0, &ser);
    log->append(QString("尝试登玩家[%1]到{%2}的[%3]服").arg(acc->nickName, ser.name, acc->serverId));
    if(http.fastPost(ser.postUrl, ser.loginBody.arg(acc->userName, acc->userPWD).toLatin1())){
        if(http.rep->error()){
            log->append(http.rep->errorString());
            return false;
        }else{
            log->append("开始解析iframe");
            if(http.fastGet(ser.gameUrl.arg(acc->serverId))){
                if(http.rep->error()){
                    log->append(http.rep->errorString());
                    return false;
                }
                auto str0 = QString(http.rep->readAll());
                if(!str0.contains("name=\"game_box\"")){
                    log->append("获取iframe失败:服务器返回的数据不正确");
                    return false;
                }
                str0 = str0.split("name=\"game_box\"")[1].split("src=\"")[1].split(R"(" code="">)")[0];
                log->append("开始解析游戏地址");
                if(http.fastGet(str0)){
                    if(http.rep->error()){
                        log->append(http.rep->errorString());
                        return false;
                    }
                    auto str0 = QString(http.rep->readAll());
                    if(!str0.contains("movie\" value='")){
                        log->append("获取游戏地址失败:服务器返回的数据不正确");
                        return false;
                    }
                    *ret = "http://";
                    *ret += http.rep->url().toString().split("/")[2];
                    *ret += "/";
                    *ret += str0.split("movie\" value='")[1].split("' />")[0];
                    log->append("解析成功!");
                    return true;
                }
            }
        }
    }


    log->append("请求超时");
    return false;
}

bool LoginGamePost::login7k7k(AccData *, QTextEdit *, QString *ret) {
    return false;
}
