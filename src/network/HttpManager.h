//
// Created by 17834 on 2023/7/2.
//

#ifndef ZEROHELPER_HTTPMANANGER_H
#define ZEROHELPER_HTTPMANANGER_H

#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QTimer>
#include <QObject>
#include <QNetworkReply>

struct HttpManager: QObject{
    QNetworkAccessManager manager;
    QNetworkReply *rep;

    QNetworkRequest* fastBuildReq(const QUrl&);
    bool fastPost(const QUrl&, const QByteArray&);
    bool fastGet(const QUrl&);
};


#endif //ZEROHELPER_HTTPMANANGER_H
