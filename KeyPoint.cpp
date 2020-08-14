#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv\cv.h>
#include <sys/types.h>
#include <malloc.h>
#include<iostream>

using namespace std;
using namespace cv;	
void Reverse(IplImage*);

IplImage* Key_Point(IplImage* img,int minHessian)
{
			initModule_nonfree();

			//GRAY Conversion

			IplImage* gimg = cvCreateImage(cvGetSize(img),8,1);
			cvCvtColor(img,gimg,CV_RGB2GRAY);

			IplImage* cpygimg = cvCreateImage(cvGetSize(img),8,1);
			cvSet(cpygimg, cvScalar(255));

			//Initializing the input image parameters

			CvSeq *grayimgKeyPoints;
			CvSeq *grayimgDescriptors;
			CvMemStorage *memoryBlock = cvCreateMemStorage();
			CvSURFParams params = cvSURFParams(minHessian,0);

			cvExtractSURF(img,0, &grayimgKeyPoints, &grayimgDescriptors, memoryBlock, params);

			static CvScalar white_color[] ={255,255,255};
			static CvScalar red_color[] ={128,0,255};
			static CvScalar black_color[] ={0,0,0};

			//Keypoint Detection

			for (int i = 0; i < grayimgDescriptors->total; i++)
			{    
				const CvSURFPoint* grayimgpositionKeyPoint = (const CvSURFPoint*) cvGetSeqElem(grayimgKeyPoints, i);
				CvPoint center;
				int radius;

				//Detecting the keypoints having laplacian value +1

				if(grayimgpositionKeyPoint->laplacian>0)
				{
						center.x = cvRound(grayimgpositionKeyPoint->pt.x);
						center.y = cvRound(grayimgpositionKeyPoint->pt.y);
						radius = cvRound(grayimgpositionKeyPoint->size*1.2/9.*2);
						cvCircle(cpygimg, center, 3, black_color[0], 1, 8, 0 );
				}
			}

			return cpygimg;
}