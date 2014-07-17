#include <stdio.h>
#include <stdlib.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int imageWidth = 320;
int imageHeight = 240;
CascadeClassifier cascade;

void detectFace(IplImage *pFrame, CascadeClassifier cascade, CvHaarClassifierCascade* classifier);

int main(int argc, char **argv)
{

	cascade.load("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt_tree.xml");
	CvHaarClassifierCascade* classifier=(CvHaarClassifierCascade*)cvLoad("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt_tree.xml", 0, 0, 0);
	CvCapture* pCapture = cvCreateCameraCapture(0);
	cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_WIDTH, imageWidth);
	cvSetCaptureProperty(pCapture, CV_CAP_PROP_FRAME_HEIGHT, imageHeight);

	IplImage *pFrame = 0;

	if (NULL == pCapture)
	{
		fprintf(stderr, "Can't initialize webcam!\n");
		return 1;
	}

	pFrame = cvQueryFrame(pCapture);// query a frame

	if(NULL == pFrame)
	{
		fprintf(stderr, "Can't get a frame!\n" );
		return 1;
	}

	const char *pImageFileName = "webcam.jpg";
	cvSaveImage(pImageFileName, pFrame);

	detectFace(pFrame, cascade, classifier);

	cvReleaseCapture(&pCapture);    // free memory

	return 0;
}

void detectFace(IplImage *pFrame, CascadeClassifier cascade, CvHaarClassifierCascade* classifier)
{
	std::vector<Rect> faces;

	Mat mGray;
	Mat mFrame(pFrame, 0);
	// CvMemStorage* facesMemStorage=cvCreateMemStorage(0);

	cvtColor( mFrame, mGray, CV_BGR2GRAY );
	equalizeHist( mGray, mGray );

	// IplImage pGray = IplImage(mGray);

	cascade.detectMultiScale( mGray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(10, 10) );

	for( int i = 0; i < faces.size(); i++ )
	{
		Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
		ellipse( mFrame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
	}

	// CvSeq* faces = cvHaarDetectObjects(
	// 							pGray, classifier, facesMemStorage, 1.1, 3,
	// 							CV_HAAR_DO_CANNY_PRUNING,
	// 							cvSize(5, 5)
	// 							);
	//
	// if(faces != NULL)
	// {
	// 	for(int i = 0; i < faces->total; ++i)
	// 	{
	// 	// Setup two points that define the extremes of the rectangle,
	// 	// then draw it to the image
	// 		CvPoint point1, point2;
	// 		CvRect* rectangle = (CvRect*)cvGetSeqElem(faces, i);
	// 		point1.x = rectangle->x;
	// 		point2.x = rectangle->x + rectangle->width;
	// 		point1.y = rectangle->y;
	// 		point2.y = rectangle->y + rectangle->height;
	// 		cvRectangle(pFrame, point1, point2, CV_RGB(255,0,0), 3, 8, 0);
	// 	}
	// }

	cvSaveImage("after.jpg", pFrame);

	// cvReleaseMemStorage(&facesMemStorage);
	// cvReleaseImage(&pFrame);
	// cvReleaseHaarClassifierCascade(&classifier);
}



