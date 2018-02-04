#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    CvCapture* capture = cvCreateFileCapture("./movie.mov");

    IplImage* frame = NULL;

    if(!capture)
    {
        printf("Video Not Opened\n");
        return -1;
    }

    int width = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
    int height = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);
    double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    int frame_count = (int)cvGetCaptureProperty(capture,  CV_CAP_PROP_FRAME_COUNT);

    printf("Video Size = %d x %d\n",width,height);
    printf("FPS = %f\nTotal Frames = %d\n",fps,frame_count);

    while(1)
    {
        frame = cvQueryFrame(capture);

        if(!frame)
        {
            printf("Capture Finished\n");
            break;
        }

        cvShowImage("video",frame);
        cvWaitKey(10);
    }

    cvReleaseCapture(&capture);
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
