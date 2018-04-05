#include "src/view/test-view.h"

TestView::TestView(QWidget *parent) : QWidget(parent),
    testLoaded(false), first(true)
{
    setLayout(new QVBoxLayout);
}

ResponseAnswer_ENUM TestView::load(QString inStr)
{ //  в данном методе происходит загрузка теста из файла (в будущем)
    stWidget = new QStackedWidget();
    QVBoxLayout *vbxLayout = new QVBoxLayout;
    setLayout(vbxLayout);
    layout()->addWidget(stWidget);
    xmlFile = new QFile(inStr);
    xmlFile->open(QIODevice::ReadOnly);
    readXML();
    xmlFile->close();
    testLoaded = true;
    draw();

    return VIEW_TEST_VIEW_LOAD_SUCCESS;
}

int TestView::draw()
{

    /// Проработать, в зависимости от логики теста переход между
    /// заданиями и выход из теста (окно результата или ).
    ///
    ///
    if(!first)
    {
        clearLayout();
        queueOfProblemsWidgets.pop_front();
    }
    else
        first = false;
    if(!queueOfProblemsWidgets.count())
        return 1;
    stWidget->addWidget(queueOfProblemsWidgets.front());
    stWidget->setCurrentWidget(queueOfProblemsWidgets.front());
    return 0;
}

void TestView::drawROI(ROI_VectorElement &printROI)
{
    QLabel *tmpLabel;
    QPalette pall;
    QFont f( "Arial", 21, QFont::Normal);
    tmpLabel = new QLabel(queueOfProblemsWidgets.back());
    tmpLabel->move(printROI.position.x(), printROI.position.y());
    tmpLabel->resize(printROI.points.at(0).x(),printROI.points.at(0).y());
//    qDebug() << printROI.points.at(0).x() << " " << printROI.points.at(0).y();
    tmpLabel->setText(printROI.content.replace("\\n", "\n"));
    tmpLabel->setFont( f);
    tmpLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    tmpLabel->setWordWrap(true);
#ifdef NO_EYE_TRACKER_TEST
    tmpLabel->setFrameStyle(QFrame::Box);
    tmpLabel->setLineWidth(1);
#endif
    /// Проработать, в зависимости от логики теста переход между
    /// заданиями и выход из теста (окно результата или ).
    ///

}

void TestView::readXML()
{
    QXmlStreamReader xml(xmlFile);
    do {
        xml.readNext();
        if(xml.isStartElement())
        {

            if(xml.name() == "BCI_Test")
            {

            }
            else if(xml.name() == "Problem")
            {
                readProblemTag(xml);
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

void TestView::readProblemTag(QXmlStreamReader &xml)
{
    ROI_VectorElement tmpROI;
    QString elementInXML;
    queueOfProblemsWidgets.push_back(new QWidget());
    QPushButton *pcmd1 = new QPushButton(queueOfProblemsWidgets.back());
    pcmd1->setText("Далее");
    pcmd1->move(width()-200, height()-70);
    connect(pcmd1, &QPushButton::clicked, this,
            [=](){emit sendSignal(VIEW_TEST_PRESSED_NEXT); });

    xml.readNext();
    do{
        if(xml.name() == "Area")
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
                    //qDebug() << "tmpROI.content = xml.text(): " <<  xml.text();
                    tmpROI.content = xml.text().toString();
                    //qDebug() << "tmpROI.content = " << tmpROI.content << "\nxml.text().toString() = " << xml.text().toString();
                }
                else if(xml.name() == "Style")
                {
                    xml.readNext();
                    tmpROI.style = xml.text().toString();
                }
            }while(xml.name() != "Area");
            drawROI(tmpROI);
            tmpROI.points.clear();

        }
        xml.readNext();
    }while(!xml.isEndElement() || xml.name()!="Problem");

}

void TestView::clearLayout()
{
    if ( queueOfProblemsWidgets.front()->layout() != NULL )
    {
        QLayoutItem* item;
        while ( ( item = queueOfProblemsWidgets.front()->layout()->takeAt( 0 ) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete queueOfProblemsWidgets.front()->layout();
    }
}

/*
 *
 * Evry test has own ID.
 * It needs only for choosing correct analysis algorithm.
 * Simple test XML starts with BCIs_TS [test simple] _ID_name.
 *
*/
