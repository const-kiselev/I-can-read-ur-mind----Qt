#include "tests-controller.h"

TestsController::TestsController(QObject *parent) : QObject(parent)
{
    activeTestID = -1;
    loadedPageID = -1;
}

int TestsController::init()
{
    return 0;
}

int TestsController::addBCI(BCI_TYPE in_bciType)
{
    activeBCIs.append(in_bciType);
    // автоматически проверяем тесты и нейроинтерфейсы:
    checkTestsAndBCIs();
    //return findTests(in_bciType);
    return 0;
}

ResponseAnswer_ENUM TestsController::loadTest(int ID) // todo: посмотреть
{
    if(activeTestID!=-1)
        return MODEL_TESTS_CONTROLLER_THERE_IS_ACTIVE_TEST;
//    if(_testsList.length()<=ID || ID<0)
//        return MODEL_TESTS_CONTROLLER_NO_SUCH_TEST_ID; //
    bool found = false;
    foreach(TestFileData testData, testsFilesList){
        if(testData.ID == ID){
            found = true;
            testData.testModel->setPath(testData.file);
            if(testData.testModel->load()!=MODEL_TESTS_CONTROLLER_LOADED)
                return MODEL_TESTS_CONTROLLER_TEST_NOT_LOADED;
            connect(testData.testModel, SIGNAL(loadedSignal()),
                    this, SLOT(testLoaded()));
            break;
        }
    }
    if(!found)
        return MODEL_TESTS_CONTROLLER_NO_SUCH_TEST_ID;

//    if (_testsList.at(ID)._testModel->load()!=MODEL_TESTS_CONTROLLER_LOADED) // загрузка данных ROIs для использования в потоковом анализе данных айТрекера
//        return MODEL_TESTS_CONTROLLER_TEST_NOT_LOADED;
    loadedPageID = ID;
    qDebug() << "loadedPageID = " << loadedPageID;
    return MODEL_TESTS_CONTROLLER_LOADED;
}

ResponseAnswer_ENUM TestsController::startTest()
{
    if(activeTestID!=-1)
        return MODEL_TESTS_CONTROLLER_THERE_IS_ACTIVE_TEST;
    if(loadedPageID==-1)
        return MODEL_TESTS_CONTROLLER_NO_LOADED_TEST;
    activeTestID = loadedPageID;
    return MODEL_TESTS_CONTROLLER_SUCCESS_TEST_START;
}

ResponseAnswer_ENUM TestsController::finishTest()
{
    foreach(TestFileData testData, testsFilesList){
        if(testData.ID == activeTestID)
            testData.testModel->unload();
    }
    activeTestID = -1;
    loadedPageID = -1;
    return OK_ANSWER;
}

QString TestsController::getTestXMLfileLink()
{
    if(loadedPageID==-1)
        return MODEL_TESTS_CONTROLLER_NO_LOADED_TEST;
    foreach(TestFileData testData, testsFilesList){
        if(testData.ID == loadedPageID)
            return testData.file->fileName();
    }
}

int TestsController::getActiveTestID()
{
    return activeTestID;
}

void TestsController::setStreamForActiveTest(QTextStream *inStream)
{
    if(activeTestID == -1)
        return;
    foreach(TestFileData testData, testsFilesList){
        if(testData.ID == activeTestID)
            testData.testModel->setStream(inStream);
    }
}

QList<ViewTestElement> TestsController::getListOfTests()
{
    QList<ViewTestElement> resList;
    ViewTestElement tmpViewTestElement;
    qDebug() << "QList<ViewTestElement> TestsController::getListOfTests(): ";
    foreach(TestFileData testData, testsFilesList)
    {
        tmpViewTestElement.ID = testData.ID;
        // todo: необходимо будет переделать. Так как должно сохранятся именно название теста, а не имя файла!!!
        tmpViewTestElement.name = testData.file->fileName();
        resList.append(tmpViewTestElement);
    }

//    int len = _testsList.length();
//    for(int i=0; i<len; i++){
//        tmp.ID = _testsList.at(i).ID;
//        tmp.name = "ТЕСТ 1";
//        tmp._bciType = _testsList.at(i)._bciType;
//        res.push_back(tmp);
//    }
    return resList;
}

int TestsController::addTest(QString FileName)
{
    // TODO: реализовать проверку на валидность
    TestFileData tmp;
    tmp.ID = testsFilesList.count();
    tmp.file = new QFile(FileName);
    // TODO: реализовать проверку на необходимые нейроинтерфейсы для данного теста
    tmp.neededBCIs.append(BCI_TYPE_EYE_TRACKER);
    testsFilesList.append(tmp); // добавляем тестовый файл в лист с тестовыми файлами
    // автоматически проверяем тесты и нейроинтерфейсы:
    checkTestsAndBCIs();
    return 0;
}

void TestsController::testLoaded()
{

}

void TestsController::setGazePointForAnalysis(double inX, double inY)
{
    if(activeTestID == -1)
        return;
    foreach(TestFileData testData, testsFilesList){
        if(testData.ID == activeTestID)
            testData.testModel->setPointToROIsAnalysis(QPoint(inX*widgetSize.x(), inY*widgetSize.y()));
    }
}



void TestsController::addTestModel(int ID)
{
    qDebug() << "TestsController::addTestModel(int ID)" << ID;
    TestModel *tmpTestModel = new TestModel;
    ViewTestElement tmpViewTestElement;
    for(int i=0; i<testsFilesList.length(); i++)
    {
        if(testsFilesList.at(i).ID == ID)
        {
            (testsFilesList[i]).testModel = tmpTestModel;
            tmpViewTestElement.ID = ID;
            tmpViewTestElement.name = testsFilesList.at(i).file->fileName(); // TODO: переделать на название теста .name
            emit sendSignal(MODEL_TEST_WAS_ADDED_d, tmpViewTestElement.converToJSONstr());
            break;
        }
    }
//    foreach(TestFileData &testData, testsFilesList){
//        if(testData.ID == ID){
//            //qDebug() << "testData.ID = " << testData.ID;
//            //testData.testModel = tmpTestModel;
////            tmpViewTestElement.ID = testData.ID;
////            tmpViewTestElement.name = testData.name;
////            emit sendSignal(MODEL_TEST_WAS_ADDED_d, tmpViewTestElement.converToJSONstr());
////            break;
//        }
//    }
}

void TestsController::deleteTestModel(int ID)
{
    foreach(TestFileData testData, testsFilesList){
        if(testData.ID == ID){
            delete testData.testModel;
            testData.testModel = nullptr;
            QJsonObject tmpJson;
            tmpJson["ID"] = ID;
            emit sendSignal(MODEL_TEST_WAS_DELETED_d, JSONtoStr(tmpJson));
            break;
        }
    }
}

void TestsController::checkTestsAndBCIs()
{
    foreach(TestFileData currentFile, testsFilesList)
    {
        /// нельзя проверять, есть ли testModel для теста. Так как если есть, а нейро-
        /// интерфейс был отключен, то тест не должен быть активным!
        bool create_model = false;
        // проверка на доступность всех нейроинтерфейсов для тестов!
        foreach (BCI_TYPE neededBCI, currentFile.neededBCIs) {
            bool found = false;
            foreach(BCI_TYPE BCI, activeBCIs){
                if(neededBCI==BCI)
                {
                    found = true;
                    continue;
                }
            }
            if(found)
                create_model = true;
            else
                break;
        }
        // проверка на создание
        if(create_model){
            // если такой модель еще не создана, то создаем
            if(!currentFile.testModel)
                addTestModel(currentFile.ID);
        }
        else{
            // а если не нужно создавать, но модель существует, то удаляем.
            // так как необходимый для нее нейроинтерфейс был отключен
            if(currentFile.testModel)
                deleteTestModel(currentFile.ID);
        }
    }
}

////////////////////////////////////////////////

TestModel::TestModel(QObject *parent): QObject(parent)
{
    loaded = false;
}


ResponseAnswer_ENUM TestModel::load()
{
    qDebug() << "TestModel::load() loaded = " << loaded;
    if(loaded)
        return MODEL_TESTS_CONTROLLER_LOADED;
    qDebug() << "TestModel::load()";
    xmlFile->open(QIODevice::ReadOnly);
    readXML();
    xmlFile->close();
    loaded = true;
    return MODEL_TESTS_CONTROLLER_LOADED;

}

ResponseAnswer_ENUM TestModel::unload()
{
    _ROIsVector.clear();
    loaded=false;
    return OK_ANSWER;
}

void TestModel::setPointToROIsAnalysis(QPoint inPoint)
{
    /// происходит проход по всем элементам вектора с ROI-облостями
    qDebug() << "TestModel::setPointToROIsAnalysis " << inPoint;
    foreach (ROI_TestModel tmpROI, _ROIsVector) {
        if( (inPoint.x() >= tmpROI.position.x()) &&
                (inPoint.y() >= tmpROI.position.y()) )
        {
            if( (inPoint.x() <= tmpROI.points.at(0).x()) &&
                    (inPoint.y() <= tmpROI.points.at(0).y()) )
            {
                qDebug() << "Point in ROI with ID " << tmpROI.ID;
                if(tmpROI.NOTforEyeTrackerFixation)
                    return;
                if(!outPutStream)
                    qDebug() << "Point in ROI with ID " << tmpROI.ID;
                else{
                    *outPutStream << "ROI: " << tmpROI.ID << "\n"; // todo: необходимо модернизировать
                    qDebug() << "Point in ROI with ID " << tmpROI.ID;
                }
            }
        }
    }

}

void TestModel::readXML()
{
    QXmlStreamReader xml(xmlFile);
    ROI_TestModel tmpROI;
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
                        else if (attr.value().toString() == "eyeTrackingIgnore")
                        {
                            if(attr.value().toString() == "true")
                                tmpROI.NOTforEyeTrackerFixation = true;
                        }
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
                    }/*
                    else if(xml.name() == "Content")
                    {
                        xml.readNext();
                        qDebug() << "tmpROI.content = xml.text(): " <<  xml.text();
                        tmpROI.content = xml.text().toString();
                        qDebug() << "tmpROI.content = " << tmpROI.content << "\nxml.text().toString() = " << xml.text().toString();
                    }*//*
                    else if(xml.name() == "Style")
                    {
                        xml.readNext();
                        tmpROI.style = xml.text().toString();
                    }*/
                }while(xml.name() != "ROI");
                _ROIsVector.append(tmpROI);
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

    foreach (ROI_TestModel tmpROI, _ROIsVector) {
        qDebug() << "ID = " << tmpROI.ID <<  " pos.x="<< tmpROI.position.x() << " pos.y="<<tmpROI.position.y()
                 << " points[0].x="<< tmpROI.points.at(0).x() << "points[0].y="  <<tmpROI.points.at(0).y();
    }

}
