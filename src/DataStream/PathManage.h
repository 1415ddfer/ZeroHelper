//
// Created by 17834 on 2023/6/28.
//

#ifndef ZEROHELPER_PATHMANAGE_H
#define ZEROHELPER_PATHMANAGE_H

#include <QString>
#include <QDir>
#include <qdebug.h>
#include <QMessageBox>

struct PathManage {
    static void checkPath(const QString& dir);
};


#endif //ZEROHELPER_PATHMANAGE_H
