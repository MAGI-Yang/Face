#include <stdio.h>
#include <stdlib.h>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int imageWidth = 160;
int imageHeight = 120;
CascadeClassifier cascade;

IplImage* detectFace(IplImage *pFrame, CascadeClassifier cascade, CvHaarClassifierCascade* classifier);


int main( int argc, char** argv )
{

	cascade.load("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt_tree.xml");
	CvHaarClassifierCascade* classifier=(CvHaarClassifierCascade*)cvLoad("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt_tree.xml", 0, 0, 0);
	CvCapture* pCapture = cvCreateCameraCapture(0);
	cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH, imageWidth);
	cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT, imageHeight);

	IplImage *pFrame = 0;
	IplImage *showFrame = 0;

	if (NULL == pCapture)
	{
		fprintf(stderr, "Can't initialize webcam!\n");
		return 1;
	}
	cvNamedWindow("Video", 1);

	while(1)
	{
		pFrame = cvQueryFrame( pCapture );
		if(!pFrame)break;

		pFrame = cvQueryFrame(pCapture);// query a frame

		if(NULL == pFrame)
		{
			fprintf(stderr, "Can't get a frame!\n" );
			return 1;
		}
		showFrame = detectFace(pFrame, cascade, classifier);
		if(NULL == showFrame)
                {
                        fprintf(stderr, "showFrame is null!\n" );
                        return 1;
                }
		cvShowImage("Video",showFrame);
		char c = cvWaitKey(50);
		if(c == 27)break;
	}

	cvReleaseCapture(&pCapture);
	cvDestroyWindow("Video");
}



IplImage* detectFace(IplImage *pFrame, CascadeClassifier cascade, CvHaarClassifierCascade* classifier)
{
	std::vector<Rect> faces;

	Mat mGray;
	Mat mFrame(pFrame, 0);

	cvtColor( mFrame, mGray, CV_BGR2GRAY );
	equalizeHist( mGray, mGray );

	cascade.detectMultiScale( mGray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(10, 10) );

	for( int i = 0; i < faces.size(); i++ )
	{
		Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
		ellipse( mFrame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
	}

	return pFrame;
	// cvSaveImage("after.jpg", pFrame);
}

