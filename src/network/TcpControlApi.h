//
// Created by 17834 on 2023/9/27.
//

#ifndef ZEROHELPER_TCPCONTROLAPI_H
#define ZEROHELPER_TCPCONTROLAPI_H
#include <QHttpServer>
#include <QHttpServerResponse>


class ApiServer : public QObject {
    QHttpServer server;

    void repStartGame(QHttpServerResponder &&responder);
public:
    explicit ApiServer();
};

#endif //ZEROHELPER_TCPCONTROLAPI_H
