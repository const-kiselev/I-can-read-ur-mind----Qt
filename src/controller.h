#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include "src/additional_header.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

signals:
    void modelHandler(const QString&);
public slots:
    void handler(const QString &inString);
};

#endif // CONTROLLER_H
