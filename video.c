#include <stdio.h>
#include <stdlib.h>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

int main( int argc, char** argv )
{

	IplImage* pFrame = NULL;

	CvCapture* pCapture = cvCreateCameraCapture(-1);

	cvNamedWindow("Video", 1);

	while(1)
	{
		pFrame = cvQueryFrame( pCapture );
		if(!pFrame)break;
		cvShowImage("Video",pFrame);
		char c = cvWaitKey(33);
		if(c == 27)break;
	}
	cvReleaseCapture(&pCapture);
	cvDestroyWindow("Video");
}
