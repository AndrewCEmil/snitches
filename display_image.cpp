#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv ) {
    CvCapture* capture0 = cvCreateFileCapture("./movie0.mov");
    CvCapture* capture1 = cvCreateFileCapture("./movie1.mov");

    IplImage* frame0 = NULL;
    IplImage* frame1 = NULL;
    Mat frameMat0;
    Mat frameMat1;
    Mat bigMat;

    if(!capture0 || !capture1) {
        printf("Video Not Opened\n");
        return -1;
    }

    double fps0 = cvGetCaptureProperty(capture0, CV_CAP_PROP_FPS);
    double fps1 = cvGetCaptureProperty(capture1, CV_CAP_PROP_FPS);
    double fps = (fps0 + fps1) /2.0;

    namedWindow("video", 1);
    while(1) {
        frame0 = cvQueryFrame(capture0);
        frame1 = cvQueryFrame(capture1);
        frameMat0 = cvarrToMat(frame0);
        frameMat1 = cvarrToMat(frame1);

        if(!frame0 || !frame1) {
            printf("Capture Finished\n");
            break;
        }

        hconcat(frameMat0, frameMat1, bigMat);
        imshow("video",bigMat);
        cvWaitKey(1000/fps);
    }

    cvReleaseCapture(&capture0);
    cvReleaseCapture(&capture1);
    return 0;
}

/*
int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", image );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
*/
