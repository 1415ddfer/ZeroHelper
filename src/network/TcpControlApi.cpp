////
//// Created by 17834 on 2023/9/27.
////
//
//#include <QtConcurrent>
//#include "TcpControlApi.h"
////#include "../playerData/GameManager.h"
//
//ApiServer::ApiServer() :
//        server(QHttpServer())
//{
//
//    server.route("/", [] (int page, const QHttpServerRequest &request) {
//        qDebug() << page;
//        qDebug() << request.body();
//        return QtConcurrent::run([] () {
//            return QHttpServerResponse("the future is coming");
//        });
//    });
//    server.listen(QHostAddress("127.0.0.1"), 7233);
//}
//
//void ApiServer::repStartGame(QHttpServerResponder &&responder) {
//
//}
