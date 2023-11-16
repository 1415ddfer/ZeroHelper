//
// Created by 17834 on 5/31/2023.
//

#include "TeamManager.h"
#include "../DataStream/TextDataStreamer.h"

TeamNames *TeamManager::getAllTeamName() {
    auto ret = QJsonValue();
    streamer.findValue("teamAcc", &ret);
    if (!ret.isArray()) {
        qDebug() << "wrong team data";
        throw;
    }
    auto teamArray = ret.toArray();
    auto names = new TeamNames{nullptr, nullptr};
    for (int i = 0; i < teamArray.size(); i++) {
        auto team = teamArray.at(i);
        if (!team.isObject()) {
            qDebug() << "wrong team data:" << i;
            throw;
        }
        auto data = team.toObject();
        names->add(new TeamName{data.value("name").toString(), nullptr});
    }
    return names;
}

void TeamManager::getTeam(int index, Team *ret) {
    auto re = QJsonValue();
    streamer.findValue("teamAcc", &re);
    auto team = re.toArray().at(index).toObject();
    ret->name = team.value("name").toString();
    ret->member = QList<AccData>();
    auto members = team.value("member").toArray();
    for (auto &&member: members) {
        auto acc = member;
        auto data = acc.toObject();
        ret->member.append(
                AccData{
                    data.value("providerId").toInt(),
                    data.value("user").toString(),
                    data.value("pwd").toString(),
                    data.value("nickName").toString(),
                    data.value("serverId").toString(),
                    });
    }
}

//void TeamManager::setTeam(int index, Team *team) {
//    auto re = QJsonValue();
//    steam->findValue("teamAcc", &re);
//    auto teams = re.toArray();
//    auto jsonMembers = QJsonArray();
//    for(auto &i : team->member){
//        jsonMembers.insert("")
//    }
//    auto jsonTeam = QJsonObject{
//            {"name", team->name},
//            {"member", QJsonArray()}
//    };
//    teams[index] = QJsonValue(team);
//    auto obj = QJsonObject();
//    steam->getObj(&obj);
//    obj["teamAcc"] = teams;
//    steam->setObj(&obj);
//}

void TeamManager::addTeam(QString name) {
    auto newTeam = QJsonObject{
            {"name",   name},
            {"member", QJsonArray()}
    };
    auto re = QJsonValue();
    streamer.findValue("teamAcc", &re);
    auto team = re.toArray();
    team.append(newTeam);
    auto obj = QJsonObject();
    streamer.getObj(&obj);
    obj["teamAcc"] = team;
    streamer.setObj(&obj);
}

void TeamManager::delTeam(int index) {
    auto re = QJsonValue();
    streamer.findValue("teamAcc", &re);
    auto team = re.toArray();
    team.removeAt(index);
    auto obj = QJsonObject();
    streamer.getObj(&obj);
    obj["teamAcc"] = team;
    streamer.setObj(&obj);
}

void TeamManager::addMember(int index, const AccData member) {
    auto re = QJsonValue();
    streamer.findValue("teamAcc", &re);
    auto teams = re.toArray();

    auto team = teams[index].toObject();
    auto members = team["member"].toArray();
    members.append(QJsonObject{
            {"nickName", member.nickName},
            {"user", member.userName},
            {"pwd", member.userPWD},
            {"providerId", member.providerId},
            {"serverId", member.serverId}
    });
    team["member"] = members;
    teams[index] = team;
    auto obj = QJsonObject();
    streamer.getObj(&obj);
    obj["teamAcc"] = teams;
    streamer.setObj(&obj);
}

void TeamManager::delMember(int index, int mIndex) {
    auto re = QJsonValue();
    streamer.findValue("teamAcc", &re);
    auto teams = re.toArray();

    auto team = teams[index].toObject();
    auto members = team["member"].toArray();
    members.removeAt(mIndex);
    team["member"] = members;
    teams[index] = team;
    auto obj = QJsonObject();
    streamer.getObj(&obj);
    obj["teamAcc"] = teams;
    streamer.setObj(&obj);
}

void TeamManager::updateMember(int index, int mIndex, const AccData member) {
    auto re = QJsonValue();
    streamer.findValue("teamAcc", &re);
    auto teams = re.toArray();

    auto team = teams[index].toObject();
    auto members = team["member"].toArray();
    members[mIndex] = QJsonObject{
            {"nickName", member.nickName},
            {"user", member.userName},
            {"pwd", member.userPWD},
            {"providerId", member.providerId},
            {"serverId", member.serverId}
    };
    team["member"] = members;
    teams[index] = team;
    auto obj = QJsonObject();
    streamer.getObj(&obj);
    obj["teamAcc"] = teams;
    streamer.setObj(&obj);
}

void TeamManager::updateTeamName(int index, const QString& name) {
    auto re = QJsonValue();
    streamer.findValue("teamAcc", &re);
    auto teams = re.toArray();

    auto team = teams[index].toObject();
    team["name"] = name;
    teams[index] = team;
    auto obj = QJsonObject();
    streamer.getObj(&obj);
    obj["teamAcc"] = teams;
    streamer.setObj(&obj);
}

bool TeamManager::findMember(const QString& name, AccData *acc) {
    auto re = QJsonValue();
    streamer.findValue("teamAcc", &re);
    for (auto i : re.toArray()){
        for (auto j : i.toObject().value("member").toArray()){
            auto data = j.toObject();
            if(data.value("nickName").toString() == name){
                *acc = AccData{
                        data.value("providerId").toInt(),
                        data.value("user").toString(),
                        data.value("pwd").toString(),
                        data.value("nickName").toString(),
                        data.value("serverId").toString(),
                };
                return true;
            }
        }
    }
    return false;
}

bool TeamManager::findMember(int index, const QString &name, AccData *acc) {
    auto re = QJsonValue();
    streamer.findValue("teamAcc", &re);
    auto i = re.toArray()[index].toObject();
    for (auto j : i.value("member").toArray()){
        auto data = j.toObject();
        if(data.value("user").toString() == name){
            *acc = AccData{
                    data.value("providerId").toInt(),
                    data.value("user").toString(),
                    data.value("pwd").toString(),
                    data.value("nickName").toString(),
                    data.value("serverId").toString(),
            };
            return true;
        }
    }
    return false;
}

bool TeamManager::hasLocData() {
    return streamer.hasLocData;
}

//void TeamManager::updateTeam(int index, UpdateFun fun, void* data) {
//    auto re = QJsonValue();
//    steam->findValue("teamAcc", &re);
//    auto teams = re.toArray();
//    auto team = teams[index].toObject();
//    fun(team, data);
//    teams[index] = team;
//    auto obj = QJsonObject();
//    steam->getObj(&obj);
//    obj["teamAcc"] = teams;
//    steam->setObj(&obj);
//}

void TeamNames::add(TeamName *newTeam) {
    if (firstTeam)endTeam->next = newTeam;
    else firstTeam = newTeam;
    endTeam = newTeam;
}

void TeamNames::free() {
    TeamName *cur;
    while (firstTeam) {
        cur = firstTeam->next;
        delete firstTeam;
        firstTeam = cur;
    }
    delete this;
}
