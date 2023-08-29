//
// Created by 17834 on 5/29/2023.
//

#include "TextDataStreamer.h"

// [i + len%255 for i in [time||plugin||team||]]
DefaultConfig defaultConfig = DefaultConfig{"user.data", ":/data/res/defaultUserData.json", 77};
TextDataStreamer streamer;


void TextDataStreamer::init() {
    auto file = QFile(defaultConfig.fileName);
    if (file.open(QIODevice::ReadOnly)) {
        auto buffers = file.readAll();
        file.close();
        buffers = qUncompress(buffers);
        QJsonParseError jsonError{};
        data = QJsonDocument(QJsonDocument::fromJson(buffers, &jsonError));
        if (jsonError.error == QJsonParseError::NoError) {
            return;
        }
        qDebug() << "open data json error!" << jsonError.errorString();
    }
    qDebug() << "try to load default data.";
    auto defile = QFile(defaultConfig.defaultData);
    if(!defile.open(QIODevice::ReadOnly)){
        qDebug() << "could't open defaultData";
        throw;
    }
    auto buffers = defile.readAll();
    defile.close();
    QJsonParseError jsonError{};
    data = QJsonDocument(QJsonDocument::fromJson(buffers, &jsonError));

    if(jsonError.error != QJsonParseError::NoError)
    {
        qDebug() << "json error!" << jsonError.errorString();
        throw;
    }
//   qDebug() << QString(data.toJson());
}

void TextDataStreamer::findValue(const char *key, QJsonValue  *ret) const {
    auto d = data.object();
    *ret = d.value(key);
}

//void TextDataStreamer::setValue(const char *rootKey, QJsonValue *value) {
//    auto d = data.object();
//    d.insert(rootKey, *value);
//    data.setObject(d);
//    auto file = QFile(defaultConfig.fileName);
//    file.open(QIODevice::WriteOnly| QFile::Truncate);
//    auto stream = data.toJson();
//    qCompress(stream);
//    file.write(stream);
//    file.close();
//}

void TextDataStreamer::getObj(QJsonObject *obj) {
    *obj = data.object();
}

void TextDataStreamer::setObj(QJsonObject *obj) {
    data.setObject(*obj);
    auto file = QFile(defaultConfig.fileName);
    file.open(QIODevice::WriteOnly| QFile::Truncate);
    auto stream = data.toJson();
    stream = qCompress(stream);
    file.write(stream);
    file.close();
}


