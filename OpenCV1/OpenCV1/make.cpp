#include <opencv\cv.h>
#include <opencv\highgui.h>

using namespace cv;

int main() {

	CvCapture* capture_r = cvCaptureFromCAM(0);
	CvCapture* capture_l = cvCaptureFromCAM(1);
	CvSize size = cvSize(640, 480);

	IplImage* green = cvCreateImage(size, 8, 1);
	IplImage* blue = cvCreateImage(size, 8, 1);
	IplImage* r_r = cvCreateImage(size, 8, 1);
	IplImage* r_l = cvCreateImage(size, 8, 1);
	IplImage* img3d = cvCreateImage(size, 8, 3);

	 //cvNamedWindow("Right",CV_WINDOW_NORMAL);
	 //cvNamedWindow("Left",CV_WINDOW_NORMAL);
	cvNamedWindow("Anaglyph", CV_WINDOW_AUTOSIZE);
	while (1)
	{
		IplImage* frame_r = cvQueryFrame(capture_r);
		IplImage* frame_l = cvQueryFrame(capture_l);

		//cvSplit(frame_l, blue, green, r_l, NULL);
		//cvSplit(frame_r, blue, green, r_r, NULL);

		cvSplit(frame_l, blue, green, r_l, NULL);
		cvSplit(frame_r, blue, green, r_r, NULL);


		cvMerge(blue, green, r_l, NULL, img3d);

		//cvShowImage("Right", frame_r);
		//cvShowImage("Left",frame_l);
		cvShowImage("Anaglyph", img3d);
		if ((cvWaitKey(10) & 255) == 27) break;
	}
	//cvReleaseCapture(&capture_r);
	//cvReleaseCapture(&capture_l);
	cvDestroyWindow("Anaglyph");
	return 0;
	
}