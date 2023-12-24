//
// Created by 17834 on 2023/9/27.
//

#include <QtConcurrent>
#include "TcpControlApi.h"
#include "../playerData/GameManager.h"

ApiServer::ApiServer() :
        server(QHttpServer())
{

//    server.route("/", [] (int page, const QHttpServerRequest &request) {
//        qDebug() << page;
//        qDebug() << request.body();
//        return QtConcurrent::run([] () {
//            return QHttpServerResponse("the future is coming");
//        });
//    });
    server.route("/", [] () {
        return "Hello World";
    });

    server.route("/nickName/<arg>/start", [](const QString &userName){
        AccData acc;
        if (TeamManager::findMember(userName, &acc)){
            if (gameManager.counterInfo(&acc)){
                auto info = gameManager.getInfo(&acc);
                if (info->getGameState() == OnRunning){
                    return QJsonObject{
                            {"state", info->getGameState()},
                            {"tis", "游戏正在运行"},
                            {"hwnd", qint64 (info->getWId())}
                    };
                }
                return QJsonObject{
                        {"state", info->getGameState()},
                        {"tis", "还没加载出游戏"},
                        {"hwnd", 0}
                };
            }
            gameManager.createGame(&acc);
            return QJsonObject{
                    {"state", 66},
                    {"tis", "正在启动游戏"},
                    {"hwnd", 0}
            };
        }
        return QJsonObject{
            {"state", -1},
            {"tis", QString("没找到对应账号:%1").arg(userName)},
            {"hwnd", 0}
    };
    });

    server.route("/nickName/<arg>/restart", [](const QString &userName){
        AccData acc;
        if (TeamManager::findMember(userName, &acc)){
            if (gameManager.counterInfo(&acc)){
                auto info = gameManager.getInfo(&acc);
                if (info->getGameState() == OnRunning){
                    emit info->closeGame();
                    QThread::msleep(3000); // 延迟3秒后启动
                    gameManager.createGame(&acc);
                    return QJsonObject{
                            {"state", 66},
                            {"tis", "正在启动游戏"},
                            {"hwnd", 0}
                    };
                }
                return QJsonObject{
                        {"state", info->getGameState()},
                        {"tis", "还没加载出游戏"},
                        {"hwnd", 0}
                };
            }
            return QJsonObject{
                    {"state", -1},
                    {"tis", "游戏未启动！"},
                    {"hwnd", 0}
            };
        }
        return QJsonObject{
                {"state", -2},
                {"tis", QString("没找到对应账号:%1").arg(userName)},
                {"hwnd", 0}
        };
    });

    server.route("/nickName/<arg>/toMiniSize", [](const QString &userName){
        AccData acc;
        if (TeamManager::findMember(userName, &acc)){
            if (gameManager.counterInfo(&acc)){
                auto info = gameManager.getInfo(&acc);
                if (info->getGameState() == OnRunning){
                    if (info->changeGameToMiniSize()){
                        return QJsonObject{
                                {"state", info->getGameState()},
                                {"tis", "迷你化成功"},
                                {"hwnd", qint64 (info->getWId())}
                        };
                    }
                    return QJsonObject{
                            {"state", -1},
                            {"tis", "迷你化失败：已经是迷你化了"},
                            {"hwnd", qint64 (info->getWId())}
                    };
                }
                return QJsonObject{
                        {"state", info->getGameState()},
                        {"tis", "还没加载出游戏"},
                        {"hwnd", 0}
                };
            }
            return QJsonObject{
                    {"state", -2},
                    {"tis", "游戏未启动"},
                    {"hwnd", 0}
            };
        }
        return QJsonObject{
                {"state", -3},
                {"tis", QString("没找到对应账号:%1").arg(userName)},
                {"hwnd", 0}
        };
    });

    server.route("/nickName/<arg>/toNormalSize", [](const QString &userName){
        AccData acc;
        if (TeamManager::findMember(userName, &acc)){
            if (gameManager.counterInfo(&acc)){
                auto info = gameManager.getInfo(&acc);
                if (info->getGameState() == OnRunning){
                    if (info->changeGameToMiniSize()){
                        return QJsonObject{
                                {"state", info->getGameState()},
                                {"tis", "恢复常规化成功"},
                                {"hwnd", qint64 (info->getWId())}
                        };
                    }
                    return QJsonObject{
                            {"state", -1},
                            {"tis", "恢复常规化成功：已经是常规化了"},
                            {"hwnd", qint64 (info->getWId())}
                    };
                }
                return QJsonObject{
                        {"state", info->getGameState()},
                        {"tis", "还没加载出游戏"},
                        {"hwnd", 0}
                };
            }
            return QJsonObject{
                    {"state", -2},
                    {"tis", "游戏未启动"},
                    {"hwnd", 0}
            };
        }
        return QJsonObject{
                {"state", -3},
                {"tis", QString("没找到对应账号:%1").arg(userName)},
                {"hwnd", 0}
        };
    });

    server.listen(QHostAddress::LocalHost, 7233);
}

void ApiServer::repStartGame(QHttpServerResponder &&responder) {

}
