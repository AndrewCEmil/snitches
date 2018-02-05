#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {
    vector<CvCapture*> captures;
    for(int i = 0; i < argc - 1; i++) {
        captures.push_back(cvCreateFileCapture(argv[i+1]));
    }

    vector<IplImage*> frames;
    vector<Mat> mats;
    Mat outMat;

    double fps = 0;
    for(int i = 0; i < argc - 1; i++) {
        fps = fps + cvGetCaptureProperty(captures[i], CV_CAP_PROP_FPS);
    }
    fps = fps / (argc - 1);

    namedWindow("video", 1);
    while(1) {
        frames.clear();
        mats.clear();
        for(int i = 0; i < argc - 1; i++) {
            frames.push_back(cvQueryFrame(captures[i]));
            if(!frames[i]) {
                printf("Capture Finished\n");
                goto stop;
            }
            mats.push_back(cvarrToMat(frames[i]));
            if(i == 1) {
                hconcat(mats[i-1], mats[i], outMat);
            } else if(i > 1) {
                hconcat(outMat, mats[i], outMat);
            }
        }

        imshow("video",outMat);
        cvWaitKey(1000/fps);
    }

    stop:
    for(int i = 0; i < argc - 1; i++) {
        cvReleaseCapture(&captures[i]);
    }
    return 0;
}
