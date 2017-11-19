
#include <iostream>
#include <stdio.h>
#include </Users/LingYu/Google 雲端硬碟/project/videotopicture/include/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
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
    namedWindow("video demo", WINDOW_KEEPRATIO);
    Mat frame;

    while (true)
    {
        //這邊是算一定的時間後截圖
        for(int i=0;i<60;i++){
            recorder >> frame;
            if (frame.empty())
            {
                break;
            }
            imshow("video demo", frame);
            waitKey(100);
            video >> frame;
        }
        imwrite("picture_to_check.jpg",frame);

    }

    return 0;
}