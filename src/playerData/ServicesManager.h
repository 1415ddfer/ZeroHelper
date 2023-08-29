//
// Created by 17834 on 2023/6/24.
//

#ifndef ZEROHELPER_SERVICESMANAGER_H
#define ZEROHELPER_SERVICESMANAGER_H


#include <QString>
#include "../DataStream/TextDataStreamer.h"

struct Services{
    QString name, postUrl, loginBody, gameUrl;
};

struct ServicesManager {
    static void GetAllServices(QList<QString>* ret);
    static void GetService(int index, Services* ret);
    static QString GetServiceName(int index);
    static QString GetServiceShortName(int index);
};


#endif //ZEROHELPER_SERVICESMANAGER_H
