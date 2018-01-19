#include "test-view.h"

TestView::TestView(QWidget *parent) : QWidget(parent),
    testLoaded(false)
{
//    _demo = true;
}

//ResponseAnswer_ENUM TestView::load(QString inStr)
//{ //  в данном методе происходит загрузка теста из файла (в будущем)
//    if(_demo)
//    {
//        testLoaded = true;
//        demoLoad();
//        return VIEW_TEST_VIEW_LOAD_SUCCESS;
//    }
//    return VIEW_TEST_VIEW_LOAD_SUCCESS;
//}

//void TestView::demoLoad()
//{
//    _testView = new TestRepresinatationData;
//   ROI_VectorElement tmpROI;
//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(318,384));
//   tmpROI.position = QPoint(140,14);
//   tmpROI.content = "Q1. This program will indicate whether an input number is a perfect number (a positive integer that is equal to the sum of its proper positive divisors, e.g., 6 is a perfect number because 6=1+2+3). Please find three possible bugs in it.";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(298,48));
//   tmpROI.position = QPoint(475,14);
//   tmpROI.content = "#include <stdio.h>";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(197,50));
//   tmpROI.position = QPoint(466,69);
//   tmpROI.content = "int main(){";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(319,48));
//   tmpROI.position = QPoint(502,125);
//   tmpROI.content = "int n, i=1, sum=0;";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(437,103));
//   tmpROI.position = QPoint(502,182);
//   tmpROI.content = "printf(\"Enter a number: \"); scanf(\"%d\",n);";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(214,49));
//   tmpROI.position = QPoint(502,293);
//   tmpROI.content = "while(i<=n){";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(211,50));
//   tmpROI.position = QPoint(536,348);
//   tmpROI.content = "if(i%n==0){";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(236,106));
//   tmpROI.position = QPoint(595,403);
//   tmpROI.content = "sum=sum+i;\ni++;";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.NOTforEyeTrackerFixation = true;
//   tmpROI.points.push_back(QPoint(65,104));
//   tmpROI.position = QPoint(497,515);
//   tmpROI.content = "    }\n}";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(807,103));
//   tmpROI.position = QPoint(497,625);
//   tmpROI.content = "if(sum==n)   printf(“%d is a perfect number”, i); \nelse   print(“%d is not a perfect number”);";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.points.push_back(QPoint(158,40));
//   tmpROI.position = QPoint(495,740);
//   tmpROI.content = "return 0;";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();

//   tmpROI.ID = _testView->ROIsVector.length();
//   tmpROI.type = ROI_rect;
//   tmpROI.NOTforEyeTrackerFixation = true;
//   tmpROI.points.push_back(QPoint(27,47));
//   tmpROI.position = QPoint(471,792);
//   tmpROI.content = "}";
//   tmpROI.style = "";
//   _testView->ROIsVector.push_back(tmpROI);
//   tmpROI.points.clear();
//}

//void TestView::draw()
//{
//    QLabel *tmpLabel;
//    QPalette pall;
//    QFont f( "Arial", 21, QFont::Normal);
//    foreach(ROI_VectorElement printROI, _testView->ROIsVector){
//        tmpLabel = new QLabel(this);
//        tmpLabel->move(printROI.position.x(), printROI.position.y());
//        tmpLabel->resize(printROI.points.at(0).x(),printROI.points.at(0).y());
//        qDebug() << printROI.points.at(0).x() << " " << printROI.points.at(0).y();
//        tmpLabel->setText(printROI.content);
//        tmpLabel->setFont( f);
//        tmpLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
//        tmpLabel->setWordWrap(true);
//        tmpLabel->setFrameStyle(QFrame::Box);
//        tmpLabel->setLineWidth(1);
//    }
//}
