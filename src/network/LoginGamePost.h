//
// Created by 17834 on 2023/6/29.
//

#ifndef ZEROHELPER_LOGINGAMEPOST_H
#define ZEROHELPER_LOGINGAMEPOST_H

#include <QTextEdit>
#include "HttpManager.h"
#include "../playerData/TeamManager.h"
#include "../playerData/ServicesManager.h"

struct LoginGamePost{
    static bool doLogin(AccData*, QTextEdit*, QString* ret);
    static bool login4399(AccData*, QTextEdit*, QString* ret);
    static bool login4399(AccData*, QList<QString>*, QString* ret);
    static bool login7k7k(AccData*, QTextEdit*, QString* ret);
};


#endif //ZEROHELPER_LOGINGAMEPOST_H
