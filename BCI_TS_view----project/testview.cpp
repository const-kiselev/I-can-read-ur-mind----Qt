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

void TestView::demoLoad()
{
   _testView = new TestRepresinatationData;
   ROI_VectorElement tmpROI;
//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(318,384));
//   tmpROI.position = QPoint(140,14);
//   tmpROI.content = "Q1. Данная программа должна определить, является ли входное число совершенным (целое положительное число, которое является суммой своих делителей. 6 — совершенное число, т.к. 6 = 1+2+3). Необходимо найти три возможные ошибки в коде.";
//   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(298,48));
//   tmpROI.position = QPoint(475,14);
//   tmpROI.content = "#include <stdio.h>";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(197,50));
//   tmpROI.position = QPoint(466,69);
//   tmpROI.content = "int main(){";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(319,48));
//   tmpROI.position = QPoint(502,125);
//   tmpROI.content = "int n, i=1, sum=0;";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(437,103));
//   tmpROI.position = QPoint(502,182);
//   tmpROI.content = "printf(\"Enter a number: \"); scanf(\"%d\",n);";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(214,49));
//   tmpROI.position = QPoint(502,293);
//   tmpROI.content = "while(i<=n){";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(211,50));
//   tmpROI.position = QPoint(536,348);
//   tmpROI.content = "if(i%n==0){";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(236,106));
//   tmpROI.position = QPoint(595,403);
//   tmpROI.content = "sum=sum+i;\ni++;";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
   tmpROI.NOTforEyeTrackerFixation = true;
//   tmpROI.points.push_back(QPoint(65,104));
//   tmpROI.position = QPoint(497,515);
//   tmpROI.content = "    }\n}";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(807,103));
//   tmpROI.position = QPoint(497,625);
//   tmpROI.content = "if(sum==n)   printf(“%d is a perfect number”, i); \nelse   print(“%d is not a perfect number”);";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(158,40));
//   tmpROI.position = QPoint(495,740);
//   tmpROI.content = "return 0;";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   tmpROI.ID = _testView->ROIsVector.length();
   tmpROI.type = ROI_rect;
   tmpROI.NOTforEyeTrackerFixation = true;
//   tmpROI.points.push_back(QPoint(27,47));
//   tmpROI.position = QPoint(471,792);
//   tmpROI.content = "}";
   tmpROI.style = "";
   _testView->ROIsVector.push_back(tmpROI);
   tmpROI.points.clear();

   draw();
}

void TestView::draw()
{
    QPushButton *pcmd1 = new QPushButton(this);
    pcmd1->setText("Выйти");
    pcmd1->move(width()-200, height()-70);
//    connect(pcmd1, &QPushButton::clicked, this,
//            [=](){emit sendSignal(VIEW_TEST_CLOSE_TEST); });
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
    tmpLabel->setText(printROI.content);
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
    bool ROIelement = false;
    unsigned cmdXML;
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

