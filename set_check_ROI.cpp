
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
using namespace cv;
using namespace std;

int main()
{
    VideoCapture recorder("video.mp4"); //讀取影片
    //如果沒有這個影片
    if (!recorder.isOpened())
    {
        return -1;
    }
    //開一個視窗顯示影像
    namedWindow("recorder video", WINDOW_KEEPRATIO);
    Mat frame; //這邊拿來顯示
    //開始顯示影像
    while (true)
    {
        //這邊是算一定的時間後截圖
        for (int j = 0; j < 2; j++){
            for (int i = 0; i < 80; i++)
            {
                recorder >> frame;
                if (frame.empty())
                {
                    break;
                }
                imshow("recorder video", frame);
                waitKey(1);
            }
            char str[25];
            sprintf(str, "picture_to_check_%.3d.jpg", j);
            imwrite(str, frame);
        }
        destroyWindow("recorder video");
        break;
    }
    //把frame刪掉
    Mat img1, img2, img3;
    img1 = imread("picture_to_check_000.jpg");
    img2 = imread("picture_to_check_001.jpg");
    absdiff(img1, img2, img3);
    imwrite("after000-001.jpg", img3);
}
