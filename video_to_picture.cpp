
#include <iostream>
#include <stdio.h>
#include </Users/LingYu/Google 雲端硬碟/project/videotopicture/include/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
using namespace cv;
using namespace std;

int main()
{
    VideoCapture video("video.mp4");
    if (!video.isOpened())
    {
        return -1;
    }
    Size videoSize = Size((int)video.get(CV_CAP_PROP_FRAME_WIDTH), (int)video.get(CV_CAP_PROP_FRAME_HEIGHT));
    namedWindow("video demo", WINDOW_KEEPRATIO);
    Mat videoFrame;

    while (true)
    {
        //這邊是算一定的時間後截圖
        for(int i=0;i<60;i++){
            video >> videoFrame;
            if (videoFrame.empty())
            {
                break;
            }
            imshow("video demo", videoFrame);
            waitKey(100);
            video >> videoFrame;
        }
        imwrite("picture_to_check.jpg",videoFrame);

    }

    return 0;
}