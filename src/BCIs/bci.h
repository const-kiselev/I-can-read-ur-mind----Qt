#ifndef BCI_H
#define BCI_H

#include <QObject>

#define BCI_TYPE_EYE_TRACKER 1

class BCI : public QObject
{
    Q_OBJECT
public:
    explicit BCI(QObject *parent = nullptr);

signals:

public slots:
private:
    int type;
    int linkEyeTracker();
};

#endif // BCI_H
