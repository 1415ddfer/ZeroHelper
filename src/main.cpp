#include <QApplication>
//#include "gameLogic/calculationPower.h"
#include "loginWindow/MainFrame.h"
#include <windows.h>
//#include <QJSEngine>

//void testCal() {
//    for (int i = 1; i <= 20; i++) {
//        auto x = calForce(0, 65, i, i);
//        if (x.has_value())
//            std::cout << "dx:" << i << "\t" << x.value() << std::endl;
//        else
//            std::cout << "dx:" << i << "\t" << "no solution" << std::endl;
//    }
//}

int main(int argc, char *argv[])
{


    SetProcessDPIAware();
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
//	return QApplication::exec();
    return 0;
}
