
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

int main()
{
    VideoCapture recorder("video.mp4");
    if (!recorder.isOpened())
    {
        return -1;
    }
    Size videoSize = Size((int)recorder.get(CV_CAP_PROP_FRAME_WIDTH), (int)recorder.get(CV_CAP_PROP_FRAME_HEIGHT));
    namedWindow("recorder video", WINDOW_KEEPRATIO);
    Mat frame;  //這邊拿來顯示

    while (true)
    {
        //這邊是算一定的時間後截圖
        for (int i = 0; i < 60; i++)
        {
            recorder >> frame;
            if (frame.empty())
            {
                break;
            }
            imshow("recorder video", frame);
            waitKey(1);
        }
        imwrite("picture_to_check.jpg", frame);
        destroyWindow("recorder video");
        break;
    }
    frame = imread("picture_to_check.jpg");
    cvtColor(frame, frame, CV_RGB2GRAY);

    HOGDescriptor hog;

    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    vector<Rect> found, found_filtered;
    hog.detectMultiScale(frame, found, 0, Size(8, 8), Size(128, 128), 1.05, 2);

    size_t i, j;
    for (i = 0; i < found.size(); i++)
    {
        Rect r = found[i];
        for (j = 0; j < found.size(); j++)
            if (j != i && (r & found[j]) == r)
                break;
        if (j == found.size())
            found_filtered.push_back(r);
    }
    for (i = 0; i < found_filtered.size(); i++)
    {
        Rect r = found_filtered[i];
        r.x += cvRound(r.width * 0.1);
        r.width = cvRound(r.width * 0.8);
        r.y += cvRound(r.height * 0.06);
        r.height = cvRound(r.height * 0.9);
        rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 2);
    }
    namedWindow("Street picture", WINDOW_AUTOSIZE);
    resize(frame, frame, Size(), 0.5, 0.5);
    imshow("Street picture", frame);
    imwrite("defaultHOGoutcome.jpg", frame);
    waitKey(0);

    return 0;
}