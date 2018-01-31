#include "testview.h"

TestView::TestView(QWidget *parent) : QWidget(parent),
    testLoaded(false)
{
    _demo = false;
}

int TestView::load(QFile *inAdr)
{ //  в данном методе происходит загрузка теста из файла (в будущем)
    if(_demo)
    {
        testLoaded = true;

        demoLoad();
        return 0;
    }
    else{
        xmlFile = inAdr;
        readXML();
        testLoaded = true;
        draw();
    }
    return 0;
}

void TestView::draw()
{
    QPushButton *pcmd1 = new QPushButton(this);
    QPushButton *pcmd2 = new QPushButton(this);
    pcmd1->setText("Обновить");
    pcmd2->setText("Открыть новый файл");
    pcmd1->move(width()-200, height()-70);
    pcmd2->move(width()-200, height()-100);
    connect(pcmd1, &QPushButton::clicked, this,
            [=](){emit reload(); });
    connect(pcmd2, &QPushButton::clicked, this,
            [=](){emit loadNew(); });

}

void TestView::drawROI(ROI_VectorElement &printROI)
{
    QLabel *tmpLabel;
    QPalette pall;
    QFont f( "Arial", 21, QFont::Normal);
    tmpLabel = new QLabel(this);
    tmpLabel->move(printROI.position.x(), printROI.position.y());
    tmpLabel->resize(printROI.points.at(0).x(),printROI.points.at(0).y());
    qDebug() << printROI.points.at(0).x() << " " << printROI.points.at(0).y();
    tmpLabel->setText(printROI.content.replace("\\n", "\n"));
    tmpLabel->setFont( f);
    tmpLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    tmpLabel->setWordWrap(true);
    tmpLabel->setFrameStyle(QFrame::Box);
    tmpLabel->setLineWidth(1);
    /// Проработать, в зависимости от логики теста переход между
    /// заданиями и выход из теста (окно результата или ).
    ///

}

void TestView::readXML()
{
    QXmlStreamReader xml(xmlFile);
    ROI_VectorElement tmpROI;
    QString elementInXML;
    do {
        xml.readNext();
        if(xml.isStartElement())
        {
            if(xml.name() == "ROI")
            {
                // read all attributes
                foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                    if (attr.name().toString() == QLatin1String("type")) {
                        if (attr.value().toString() == "rect")
                            tmpROI.type = ROI_rect;
                    }
                }
                do{
                    xml.readNext();
                    if(!xml.isStartElement())
                        continue;
                    if(xml.name() == "ID")
                    {
                        xml.readNext();
                        tmpROI.ID = xml.text().toInt();
                    }
                    else if(xml.name() == "Position")
                    {
                        foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                            if (attr.name().toString() == QLatin1String("x"))
                                tmpROI.position.setX(attr.value().toFloat());
                            else if (attr.name().toString() == QLatin1String("y"))
                                tmpROI.position.setY(attr.value().toFloat());
                        }
                    }
                    else if(xml.name() == "Point")
                    {
                        QPointF tmpPoint;
                        // проходим по всем атрибутам
                        foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                            if (attr.name().toString() == QLatin1String("x"))
                                tmpPoint.setX(attr.value().toFloat());
                            else if (attr.name().toString() == QLatin1String("y"))
                                tmpPoint.setY(attr.value().toFloat());
                        }
                        tmpROI.points.push_back(tmpPoint); // добавляем точку в стек
                    }
                    else if(xml.name() == "Content")
                    {
                        xml.readNext();
                        qDebug() << "tmpROI.content = xml.text(): " <<  xml.text();
                        tmpROI.content = xml.text().toString();
                        qDebug() << "tmpROI.content = " << tmpROI.content << "\nxml.text().toString() = " << xml.text().toString();
                    }
                    else if(xml.name() == "Style")
                    {
                        xml.readNext();
                        tmpROI.style = xml.text().toString();
                    }
                }while(xml.name() != "ROI");
                drawROI(tmpROI);
                tmpROI.points.clear();
            }
            else if(xml.name() == "BCI_Test")
            {

            }
//            else if(nameInXML == "")
//            {

//            }
//            else if(nameInXML == "")
//            {

//            }
//            else if(nameInXML == "")
//            {

//            }
//            else if(nameInXML == "")
//            {

//            }
//            else if(nameInXML == "")
//            {

//            }
        }


    }while(!xml.atEnd());
    if(xml.hasError())
        qDebug() << "Error while XML reading:" << xml.errorString();

}

/*
 *
 * Evry test has own ID.
 * It needs only for choosing correct analysis algorithm.
 * Simple test XML starts with BCIs_TS [test simple] _ID_name.
 *
*/

