#include <opencv\cv.h>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

IplImage* Binarization(IplImage* img)
{
	Mat im_rgb(img);
	Mat im_gray;
	cvtColor(im_rgb,im_gray,CV_RGB2GRAY);

	Mat img_bw = im_gray > 128;

	IplImage* gimg=cvCreateImage(cvSize(img_bw.cols,img_bw.rows),8,1);
	IplImage ipltemp=img_bw;
	cvCopy(&ipltemp,gimg);
	return gimg;
}