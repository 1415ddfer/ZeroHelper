//
// Created by 17834 on 2023/6/28.
//

#include "PathManage.h"

void PathManage::checkPath(const QString& dir) {
    // 如果目录不存在,则递归创建
    auto dir_ = QFileInfo(dir).absolutePath();
    if(QDir(dir_).exists()) return;  // 存在则返回
    checkPath(dir_);
    // 创建当前目录
    if (!QDir().mkdir(dir)) {
        QMessageBox::warning(nullptr, "警告", QString("创建路径失败:%1").arg(dir));
        qDebug() << "Failed to create directory" << dir;
        throw ;
    }
}