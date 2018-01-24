#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "src/additional_header.h"
#include <QFile>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

signals:
    void modelHandler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
    void viewHandler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");
public slots:
    void handler(const ResponseAnswer_ENUM cmd, const QString JSONdata = "");

private:
    struct fileCnt{
        int ID;
        QFile name;
    };
    QList<fileCnt> listOfFiles;
    void fileController(ResponseAnswer_ENUM);
    void setWorkDir(QString);
    void createFile(QString);

};

#endif // CONTROLLER_H
