//
// Created by 17834 on 2023/6/28.
//

#ifndef ZEROHELPER_PLUGINMANAGER_H
#define ZEROHELPER_PLUGINMANAGER_H

#include "../DataStream/PathManage.h"

struct DefaultPluginConfig {
    const char *fileName, *debugFileName;
};
extern DefaultPluginConfig defaultConfig;

struct PluginManager {
    static void checkFLashSaFile(QString* ret, bool isDebug = false);
};


#endif //ZEROHELPER_PLUGINMANAGER_H
