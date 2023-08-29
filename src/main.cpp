#include <QApplication>
#include "gameLogic/calculationPower.h"
#include "loginWindow/MainFrame.h"
#include "playerData/TeamManager.h"
#include "DataStream/TextDataStreamer.h"
#include "loginWindow/AccFrame.h"
#include "gameWindow/GameFrame.h"
#include <QThread>
#include <windows.h>
#include <QProcess>
#include <QJSEngine>

void testCal() {
    for (int i = 1; i <= 20; i++) {
        auto x = calForce(0, 65, i, i);
        if (x.has_value())
            std::cout << "dx:" << i << "\t" << x.value() << std::endl;
        else
            std::cout << "dx:" << i << "\t" << "no solution" << std::endl;
    }
}

class TestThread :public QThread{
    void run() override;

public:
    int testData;
};

void TestThread::run() {
    msleep(1000);
    testData = 2;
    msleep(2000);
}

void testThread(){
    auto tt = new TestThread;
    tt->testData = 0;
    tt->start();
    printf("test data is %d\n", tt->testData);
    Sleep(2000);
    printf("test data is %d\n", tt->testData);

}

void testWins(){
    auto win1 = new QWidget;
    auto win2 = new QWidget;
    win1->show();
    win2->show();
}

void accTest(){
    {
        auto teamManager = TeamManager{};
        printf("here has team{");
        auto teamData = TeamManager::getAllTeamName();
        auto team = teamData->firstTeam;
        while (team != nullptr) {
            qDebug() << team->name;
            team = team->next;
        }
        teamData->free();
        printf("}\n");
    }
}

int main(int argc, char *argv[])
{



    streamer.init();
    QApplication app(argc, argv);
//    QJSEngine engine;
//
//    QString jsCode = "import { createRequire } from 'module';"
//                     "const require = createRequire(import.meta.url);"
//                     "const CryptoJS = require('crypto-js');"
//                     "var encrypted = CryptoJS.AES.encrypt('123', 'lzYW5qaXVqa').toString();";
//
//    QJSValue result = engine.evaluate(jsCode);
//
//    QString encryptedText = result.toString();
//    qDebug() << encryptedText;
    auto test = MainFrame();
    test.show();
    QApplication::exec();
////	return QApplication::exec();
    return 0;
}
