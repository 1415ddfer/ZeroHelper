//
// Created by 17834 on 2023/6/24.
//

#include "ServicesManager.h"

void ServicesManager::GetAllServices(QList<QString> *ret) {
    auto value = QJsonValue();
    streamer.findValue("serverList", &value);
    for (auto &&service : value.toObject().value("Services").toArray()){
        ret->append(service.toObject()["name"].toString());
    }
}

void ServicesManager::GetService(int index, Services* ret) {
    auto value = QJsonValue();
    streamer.findValue("serverList", &value);
    auto obj = value.toObject().value("Services").toArray()[index].toObject();
    *ret = Services{obj["name"].toString(), obj["postUrl"].toString(), obj["loginBody"].toString(), obj["gameUrl"].toString()};
}

QString ServicesManager::GetServiceName(int index) {
    auto value = QJsonValue();
    streamer.findValue("serverList", &value);
    auto obj = value.toObject().value("Services").toArray()[index].toObject();
    return obj["name"].toString();
}

QString ServicesManager::GetServiceShortName(int index) {
    auto value = QJsonValue();
    streamer.findValue("serverList", &value);
    auto obj = value.toObject().value("Services").toArray()[index].toObject();
    return obj["nick"].toString();
}