#include<stdio.h>
#include<conio.h>
#include<opencv\highgui.h>
#include<opencv/cv.h>
#include"dirent.h"

void Reverse(IplImage* img);

using namespace cv;

IplImage* ComponentAnalysis(IplImage *img)
{
	CvMemStorage *mem;
	mem = cvCreateMemStorage(0);
	CvSeq *contours = 0;
	CvSeq *ptr;
	int length,width,step,channels,i,j;
	uchar *data;
 
	int ncontour=cvFindContours(img, mem, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	int cnt=1;
	int max=0;
	int sum=0;
	float avg_area;

	//Binarization of the modified Image

  	length=img->height; 
	width=img->width; 
	step = img->widthStep; 
	channels = img->nChannels;
	data= (uchar *)img->imageData;

	for(i=0;i<length;i++) 
	{
		for(j=0;j<width;j++) 
		{
			if(data[i*step+j*channels+0]==0 || data[i*step+j*channels+0]==255)
			{
				continue;

			}
			else
			data[i*step+j*channels+0]=255;
		}
	}

	

	//=================SIZE OF MAX COMPONENT AND THE AVERAGE OF ALL COMPONENTS SIZE=======================================//
	for (ptr =contours; ptr != NULL; ptr = ptr->h_next)
	{
  		double reg = fabs(cvContourArea(ptr, CV_WHOLE_SEQ));
		sum+=reg;
		if(max<reg)
		max=reg;
	}
	avg_area=(float)sum/ncontour;

	//==================================WORKING WITH THE COMPONENTS=====================================================//
 	for (ptr =contours; ptr != NULL; ptr = ptr->h_next)
	{
  		double reg = fabs(cvContourArea(ptr, CV_WHOLE_SEQ));
		if(reg<avg_area*10)
		{
				CvRect rectEst = cvBoundingRect( ptr, 0 );
				CvPoint pt1,pt2;
				pt1.x = rectEst.x;
				pt1.y = rectEst.y;
				pt2.x = rectEst.x+ rectEst.width;
                pt2.y = rectEst.y+ rectEst.height;
				int thickness = -5 ;
                cvRectangle( img, pt1, pt2, CV_RGB(0,0,0), thickness );
							
		}		
	}
	Reverse(img);
	return img;
}