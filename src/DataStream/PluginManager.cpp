//
// Created by 17834 on 2023/6/28.
//

#include "PluginManager.h"

DefaultPluginConfig defaultPluginConfig{":/plugin/plugin/normal.exe", ":/plugin/plugin/debug.exe"};

void PluginManager::checkFLashSaFile(QString* str0, bool isDebug) {
    auto file = isDebug ? defaultPluginConfig.debugFileName : defaultPluginConfig.fileName;
    PathManage::checkPath(file);
    auto exe = QFileInfo(file);
    if(!exe.exists()){
        QFile exeFile(isDebug ? ":/plugin/plugin/normal.exe" : ":/plugin/plugin/debug.exe");
        exeFile.open(QIODevice::ReadOnly);
        QByteArray exeData = exeFile.readAll();

        QFile tmpExeFile(file);
        tmpExeFile.open(QIODevice::WriteOnly);
        tmpExeFile.write(exeData);
        tmpExeFile.close();
    }
    *str0 = QString(file);
}
