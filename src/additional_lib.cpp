#include "additional_header.h"

QString JSONtoStr(QJsonObject inJson)
{
    return QString(QJsonDocument(inJson).toJson(QJsonDocument::Compact));
}
QJsonObject JSON_fromStr(QString inStr){
    QJsonDocument doc;
    QJsonObject json;
    doc = QJsonDocument::fromJson(inStr.toUtf8());
    json = doc.object();
    return json;
}
