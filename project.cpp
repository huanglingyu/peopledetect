
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

#define ROIpointtop 520
#define ROIpointbottom 770
vector<Rect> movingObjects(0);

void playvideo(string str)
{
    //播影片，按空白鍵選擇要辨識的畫面
    VideoCapture recorder(str); //讀取影片
    //如果沒有這個影片
    if (!recorder.isOpened())
    {
        cout << "can't open " << str << endl;
    }
    //開一個視窗顯示影像
    namedWindow("recorder video", WINDOW_KEEPRATIO);
    Mat frame;
    recorder >> frame;
    bool keepgoing = true;
    while (keepgoing)
    {
        char key = (char)waitKey(30);
        switch (key)
        {
        case ' ': //check this point
            imwrite("background.jpg", frame);
            imwrite("result1_background.jpg", frame);
            recorder >> frame;
            for (int i = 0; i < 10; i++)
                recorder >> frame;
            imshow("recorder video", frame);
            imwrite("checkpic.jpg", frame);
            imwrite("result2_checkpic.jpg", frame);
            keepgoing = false;
            break;

        default:
            recorder >> frame;
            if (frame.empty())
            {
                destroyWindow("recorder video");
                break;
            }
            imshow("recorder video", frame);
        }
    }
    destroyWindow("recorder video");
}
void findmovingthings()
{
    Mat img_back, img_check, img_moving, img_threshold;
    //把兩個灰色圖片相減
    img_back = imread("background.jpg", IMREAD_GRAYSCALE);
    img_check = imread("checkpic.jpg", IMREAD_GRAYSCALE);
    absdiff(img_back, img_check, img_moving);
    imshow("moving", img_moving);
    imwrite("movingstuff.jpg", img_moving);
    imwrite("result3_moving.jpg", img_moving);
    //做二值化
    threshold(img_moving, img_threshold, 20, 255, THRESH_BINARY);
    blur(img_threshold, img_threshold, Size(25, 25));
    imwrite("afterthreshold.jpg", img_threshold);
    imwrite("result4_movingafterthreshold.jpg", img_threshold);
    //找出白色的地方用rect匡出來
    vector<vector<Point> > contours;
    findContours(img_threshold, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    img_back = imread("background.jpg");
    img_moving = imread("background.jpg");
    int j = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        Rect rec = boundingRect(contours[i]);
        rectangle(img_moving, rec, Scalar(255, 255, 255), 3);
        //看每個rect的右下角，是否在斑馬線範圍內
        if (rec.br().y >= ROIpointtop && rec.br().y < ROIpointbottom && (rec.br().x - rec.tl().x > 63 || rec.br().y - rec.tl().y > 127))
        {
            movingObjects.push_back(rec);
            rectangle(img_back, rec, Scalar(255, 255, 255), 3);
            cout << movingObjects[j] << endl;
            j++;
        }
    }
    imwrite("result5_movingObject框出.jpg", img_back);
    imwrite("result5.0_還沒判斷條件前匡出來的東西.jpg", img_moving);
}

int main()
{

    //播影片
    playvideo("testing_video/realcamera_daytime.mov");

    //把兩個圖片做處理找出我們要偵測的地方
    findmovingthings();

    // OpenCV's HOG based Pedestrian Detector
    HOGDescriptor hogDefault(
        Size(64, 128), //winSize
        Size(16, 16),  //blocksize
        Size(8, 8),    //blockStride,
        Size(8, 8),    //cellSize,
        9,             //nbins,
        0,             //derivAperture,
        -1,            //winSigma,
        0,             //histogramNormType,
        0.2,           //L2HysThresh,
        1,             //gammal correction,
        64,            //nlevels=64
        0);            //signedGradient

    // Set the people detector.
    vector<float> svmDetectorDefault = hogDefault.getDefaultPeopleDetector();
    hogDefault.setSVMDetector(svmDetectorDefault);

    vector<Rect> bboxes;
    vector<double> weights;

    float hitThreshold = 1.0;
    Size winStride = Size(8, 8);
    Size padding = Size(32, 32);
    float scale = 1.05;
    float finalThreshold = 2;
    bool useMeanshiftGrouping = 0;
    //判斷是否是人
    Mat img_back, frame;
    char str[25];
    img_back = imread("background.jpg");
    for (int j = 0; j < movingObjects.size(); j++)
    {
        //在特定roi判斷有沒有人
        frame = img_back(movingObjects[j]);
        hogDefault.detectMultiScale(frame, bboxes, weights, 0, winStride, padding,
                                    scale, finalThreshold, useMeanshiftGrouping);
        cout << "here" << endl;
        if (!bboxes.empty())
        {
            vector<Rect>::const_iterator loc = bboxes.begin();
            vector<Rect>::const_iterator end = bboxes.end();
            for (; loc != end; ++loc)
            {
                rectangle(frame, *loc, Scalar(255, 255, 255), 2);
            }
            sprintf(str, "result6_checkframe%1d.jpg", j);
            imwrite(str, frame);
            Mat imgROI = img_back(movingObjects[j]); //指定插入的大小和位置
            addWeighted(imgROI, 0, frame, 1, 0, imgROI);
            imwrite("result7_addtobackgruond.jpg", img_back);
        }
        cout << movingObjects[j] << endl;
    }

    imshow("HOGbigresult", img_back);
    imwrite("result7_HOGresult.jpg", img_back);

    waitKey(0);

    return 0;
}