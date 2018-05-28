#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

static void help(char* progName)
{
    cout << endl
        <<  "Usage:"                                << endl
        << progName << " [input_video] [out_dir]"  << endl << endl;
}

static Mat do_dft(Mat I) {
    Mat padded;                            //expand input image to optimal size
    int m = getOptimalDFTSize( I.rows );
    int n = getOptimalDFTSize( I.cols ); // on the border add zero values
    copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));

    cvtColor(padded, padded, cv::COLOR_BGR2GRAY);
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    //Mat planes[] = {padded, Mat::zeros(padded.size(), CV_8U)};
    Mat complexI;
    merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

    dft(complexI, complexI);            // this way the result may fit in the source matrix

    // compute the magnitude and switch to logarithmic scale
    // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
    split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
    magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
    Mat magI = planes[0];

    magI += Scalar::all(1);                    // switch to logarithmic scale
    log(magI, magI);

    // crop the spectrum, if it has an odd number of rows or columns
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

    // rearrange the quadrants of Fourier image  so that the origin is at the image center
    int cx = magI.cols/2;
    int cy = magI.rows/2;

    Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
    Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
    Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
    Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right

    Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
    q2.copyTo(q1);
    tmp.copyTo(q2);

    normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
                                            // viewable image form (float between values 0 and 1).

    magI.convertTo(magI, CV_8UC3, 255.0);
    return magI;
}

int main( int argc, char** argv ) {
    help(argv[0]);

    const char* filename = argv[1];
    const char* outdir = argv[2];

    CvCapture* capture = cvCreateFileCapture(filename);

    vector<IplImage*> frames;
    IplImage* frame;
    Mat mat;
    Mat outMat;

    double fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);

    namedWindow("video", 1);

    int count = 0;
    while(1) {
        frame = cvQueryFrame(capture);
        if(!frame) {
            break;
        }
        mat = cvarrToMat(frame);
        Mat transformed = do_dft(mat);

        ostringstream os;
        os << outdir << "/" << count << "frame.jpeg";
        imwrite(os.str(), transformed);
        imshow("video",transformed);
        cvWaitKey(1000/fps);
        count++;
    }

    cvReleaseCapture(&capture);
    return 0;
}
