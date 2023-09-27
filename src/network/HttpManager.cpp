//
// Created by 17834 on 2023/7/2.
//

#include "HttpManager.h"

bool HttpManager::fastPost(const QUrl &url, const QByteArray &data) {
    auto req = QNetworkRequest(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  "User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.131 Safari/537.36");
    rep = manager.post(req, data);

    QEventLoop eventLoop;
    connect(&manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    QTimer::singleShot(2000, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if(rep->isFinished()) return true;
    disconnect(rep, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    rep->abort();
    rep->deleteLater();
    qDebug() << "timeout";
    return false;
}

bool HttpManager::fastGet(const QUrl &url) {
    auto req = QNetworkRequest(url);
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  "User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.131 Safari/537.36");
    rep = manager.get(req);

    QEventLoop eventLoop;
    connect(&manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    QTimer::singleShot(3000, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();
    if(rep->isFinished()) return true;
    disconnect(rep, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    rep->abort();
    rep->deleteLater();
    qDebug() << "timeout";
    return false;
}

QNetworkRequest* HttpManager::fastBuildReq(const QUrl &url) {
    auto *req = new QNetworkRequest(url);
    req->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    req->setHeader(QNetworkRequest::UserAgentHeader,
                  "User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.131 Safari/537.36");
    return req;
}
