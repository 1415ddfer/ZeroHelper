//
// Created by 17834 on 5/29/2023.
//

#ifndef ZEROHELPER_TEXTDATASTREAMER_H
#define ZEROHELPER_TEXTDATASTREAMER_H


#include <QString>
#include <QMap>
#include <QFile>
#include <ctime>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

struct DefaultConfig {
    const char *fileName, *defaultData;
    int key;
};
extern DefaultConfig defaultConfig;

//struct AccData{
//    int providerId;
//    char *userName, *userPWD, *nickName;
//};
//
//typedef QList<AccData> AccList;
////typedef QMap<QString, TeamList> TeamMap;
struct TextDataStreamer {
    QJsonDocument data;
    bool hasLocData;
    void init();
    void findValue(const char *rootKey, QJsonValue *ret) const;
//    void setValue(const char *rootKey, QJsonValue *value);
    void getObj(QJsonObject *obj);
    void setObj(QJsonObject *obj);
};

extern TextDataStreamer streamer;

#endif //ZEROHELPER_TEXTDATASTREAMER_H
