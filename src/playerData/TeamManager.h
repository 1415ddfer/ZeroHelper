//
// Created by 17834 on 5/31/2023.
//

#ifndef ZEROHELPER_TEAMMANAGER_H
#define ZEROHELPER_TEAMMANAGER_H
#include <QString>
#include <QList>
#include "../DataStream/TextDataStreamer.h"


struct AccData{
    int providerId;
    QString userName, userPWD, nickName, serverId;
};

struct Team{
    QString name;
    QList<AccData> member;
};

struct TeamName{
    QString name;
    TeamName *next;
};

struct TeamNames{
    TeamName *firstTeam, *endTeam;
    void add(TeamName*);
    void free();
};


//typedef void(*UpdateFun)(QJsonObject team, void* data);
struct TeamManager {
    [[nodiscard]]static TeamNames *getAllTeamName();
    static void getTeam(int index, Team*);
//    void setTeam(int index, Team*);
    static void addTeam(QString name);
    static void addMember(int index, AccData);
    static void delMember(int index, int mIndex);
    static void updateMember(int index, int mIndex, AccData);
    static void updateTeamName(int index, const QString& name);

    static void delTeam(int index);
};


#endif //ZEROHELPER_TEAMMANAGER_H
