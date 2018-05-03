#include "src/view/test-view.h"

TestView::TestView(QWidget *parent) : QWidget(parent),
    testLoaded(false), first(true)
{
    //setLayout(new QVBoxLayout);
}

ResponseAnswer_ENUM TestView::load(QString inStr)
{
    stWidget = new QStackedWidget();
    QVBoxLayout *vbxLayout = new QVBoxLayout(this);
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
        QJsonObject jsonAnswer;
        clearLayout();
        qDebug() << ".count()"<< queueOfProblemsWidgets.count() << queueOfAnswersWidgets.count();
        if(queueOfProblemsWidgets.count() == queueOfAnswersWidgets.count())
        {
            jsonAnswer = getAllInputFieldsInWidget(queueOfProblemsWidgets.front(), false);
            queueOfProblemsWidgets.pop_front();
            if(queueOfAnswersWidgets.front() ==nullptr)
                queueOfAnswersWidgets.pop_front();
        }
        else{
            jsonAnswer = getAllInputFieldsInWidget(queueOfAnswersWidgets.front(), true);
            queueOfAnswersWidgets.pop_front();
            //emit sendSignal(VIEW_TEST_ANSWERS_d, );
        }
        if(jsonAnswer.length())
            emit sendSignal(VIEW_TEST_ANSWERS_d, JSONtoStr(jsonAnswer));
    }
    else
        first = false;
    if(queueOfProblemsWidgets.isEmpty() && queueOfAnswersWidgets.isEmpty())
        return 1;
    if(queueOfProblemsWidgets.count() == queueOfAnswersWidgets.count())
    {
        stWidget->addWidget(queueOfProblemsWidgets.front());
        stWidget->setCurrentWidget(queueOfProblemsWidgets.front());
    }
    else if(queueOfAnswersWidgets.front() !=nullptr)
    {
        stWidget->addWidget(queueOfAnswersWidgets.front());
        stWidget->setCurrentWidget(queueOfAnswersWidgets.front());
    }

    return 0;
}

void TestView::drawArea(ROI_VectorElement &printROI, bool inEntryForm)
{
    QLabel *tmpLabel;
    QPalette pall;
    QFont f( "Arial", 21, QFont::Normal);
    if (inEntryForm)
        tmpLabel = new QLabel(queueOfAnswersWidgets.back());
    else
        tmpLabel = new QLabel(queueOfProblemsWidgets.back());
    tmpLabel->move(printROI.position.x(), printROI.position.y());
    tmpLabel->resize(printROI.points.at(0).x(),printROI.points.at(0).y());
//    qDebug() << printROI.points.at(0).x() << " " << printROI.points.at(0).y();
    if (printROI.inputType != "")
    {
        QWidget *entryFormElement;
        if (printROI.inputType == "text")
        {
            // todo: есть что еще сделать. Необходимо тщательно проработать вопрос с размерами
            // формы!!!!!!
            if (inEntryForm)
                entryFormElement = new QLineEdit(queueOfAnswersWidgets.back());
            else
                entryFormElement = new QLineEdit(queueOfProblemsWidgets.back());
            entryFormElement->setProperty("inputID", printROI.inputID);
            entryFormElement->setProperty("Problem index", queueOfProblemsWidgets.count() -1);
            entryFormElement->setProperty("Answer widget", inEntryForm);
            entryFormElement->setProperty("Type", "text");
            listOfAnswerFields.append(entryFormElement);
            f.setPixelSize(14);
            tmpLabel->setText(printROI.content.replace("\\n", "\n"));
            tmpLabel->setFont(f);
            tmpLabel->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
            entryFormElement->move(printROI.position.x(),
                                   printROI.position.y()+printROI.points.at(0).y());
            entryFormElement->setMinimumWidth(printROI.points.at(0).x());
            tmpLabel->setBuddy(entryFormElement);
        }
        else if (printROI.inputType == "radio")
        {}
        else if (printROI.inputType == "button")
        {}
        else if (printROI.inputType == "checkbox")
        {}

    }
    else
    {
        tmpLabel->setText(printROI.content.replace("\\n", "\n"));
        tmpLabel->setFont(f);
        tmpLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    }
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
        }


    }while(!xml.atEnd());
    if(xml.hasError())
        qDebug() << "Error while XML reading:" << xml.errorString();

}

void TestView::readProblemTag(QXmlStreamReader &xml)
{
    ROI_VectorElement tmpROI;
    bool inEntryForm = false;
    QString elementInXML;
    queueOfProblemsWidgets.push_back(new QWidget());
    queueOfProblemsWidgets.back()->setProperty("Problem index", queueOfProblemsWidgets.count()-1);
    queueOfAnswersWidgets.push_back(nullptr);

    xml.readNext();
    do{
        if(xml.name() == "Area")
        {
            // read all attributes
            foreach(const QXmlStreamAttribute &attr, xml.attributes()) {
                if (attr.name().toString() == QLatin1String("eyeTrackingIgnore"))
                {
                    if (attr.value().toString() == "true")
                        tmpROI.NOTforEyeTrackerFixation = true;
                    else
                        tmpROI.NOTforEyeTrackerFixation = false;
                }
                else if (attr.name().toString() == QLatin1String("type")) {
                    if (attr.value().toString() == "rect")
                        tmpROI.type = ROI_rect;
                }
                else if (attr.name().toString() == QLatin1String("ROI"))
                {
                    if (attr.value().toString() == "false")
                        tmpROI.ROI = false;
                    else
                        tmpROI.ROI = true;
                }
                else if (attr.name().toString() == QLatin1String("inputID"))
                    tmpROI.inputID = attr.value().toInt();
                else if (attr.name().toString() == QLatin1String("inputType"))
                {
                    if (attr.value().toString() == "text")
                        tmpROI.inputType = "text";
                    else if (attr.value().toString() == "radio")
                        tmpROI.inputType = "radio";
                    else if (attr.value().toString() == "button")
                        tmpROI.inputType = "button";
                    else if (attr.value().toString() == "checkbox")
                        tmpROI.inputType = "checkbox";
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
                else if(xml.name() == "Content")
                {
                    xml.readNext();
                    //qDebug() << "tmpROI.content = xml.text(): " <<  xml.text();
                    tmpROI.content = xml.text().toString();
                    //qDebug() << "tmpROI.content = " << tmpROI.content << "\nxml.text().toString() = " << xml.text().toString();
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
                else if(xml.name() == "Style")
                {
                    xml.readNext();
                    tmpROI.style = xml.text().toString();
                }
            }while(xml.name() != "Area");
            drawArea(tmpROI, inEntryForm);
            tmpROI.points.clear();
        }
        else if(xml.name() == "EntryForm" && xml.isStartElement())
        {
            queueOfAnswersWidgets.pop_back();
            queueOfAnswersWidgets.push_back(new QWidget());
            inEntryForm = true;
            queueOfAnswersWidgets.back()->setProperty("Problem index", queueOfProblemsWidgets.count()-1);
        }
        else if(xml.name() == "EntryForm" && xml.isEndElement())
        {
            QPushButton *pcmd2 = new QPushButton(queueOfAnswersWidgets.back());
            pcmd2->setText("Далее");
            pcmd2->move(width()-200, height()-70);
            connect(pcmd2, &QPushButton::clicked, this,
                    [=](){emit sendSignal(VIEW_TEST_PRESSED_NEXT); });
            inEntryForm = false;
        }
        else if(xml.name() == "Title" && xml.isStartElement()){
            xml.readNext();
            queueOfProblemsWidgets.back()->setProperty("Title", xml.text().toString());
        }
        xml.readNext();
    }while(!xml.isEndElement() || xml.name()!="Problem");

    QPushButton *pcmd1 = new QPushButton(queueOfProblemsWidgets.back());
    pcmd1->setText("Далее");
    pcmd1->move(width()-200, height()-70);
    connect(pcmd1, &QPushButton::clicked, this,
            [=](){emit sendSignal(VIEW_TEST_PRESSED_NEXT); });
}

void TestView::clearLayout()
{
    if(queueOfProblemsWidgets.isEmpty())
        return;
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

QJsonObject TestView::getAllInputFieldsInWidget(QWidget *wdg, bool answerWidget)
{
    QJsonObject jsonProblem, jsonInput;
    int widgetIndex = wdg->property("Problem index").toInt();
    foreach (QWidget* input, listOfAnswerFields) {
        if(widgetIndex == input->property("Problem index").toInt() &&
                input->property("Answer widget").toBool() == answerWidget)
        {

            if(input->property("Type") == "text")
                jsonInput["data"] =  ((QLineEdit*)input)->text();
            else if(input->property("Type") == "radio")
            {

            }
            else if(input->property("Type") == "button")
            {

            }
            else if(input->property("Type") == "checkbox")
            {

            }
            jsonProblem[input->property("inputID").toString()] = JSONtoStr(jsonInput);
        }
    }
    if(jsonProblem.length())
    {
        jsonProblem["Problem index"] = widgetIndex;
        jsonProblem["Answer widget"] = answerWidget;
    }
    qDebug() << "widgetIndex= "<< widgetIndex;
    return jsonProblem;
}


