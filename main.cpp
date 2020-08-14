//KEYPOINT

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv\cv.h>
#include <sys/types.h>
#include <malloc.h>
#include "dirent.h"
#include <iostream>
#include <conio.h>

using namespace cv;
using namespace std;
 
int main()
{
		initModule_nonfree();

		int max,max1,m,erosion_param;
		double alpha = 0.5,beta; 

		int minHessian(IplImage*);
		int Stat_Mode(IplImage*);
		void Reverse(IplImage*);
		IplImage* Key_Point(IplImage*,int);
		IplImage* SlidingWindow(Mat);

		//Input image

		string inputDirectory = "E:\\Input";	//Set Input Directory as per your PC
		string outputDirectory = "E:\\Output";  //Set Output Directory as per your PC
		DIR *directory = opendir (inputDirectory.c_str());
		struct dirent *_dirent = NULL;
		if(directory == NULL)
		{
			printf("Cannot open Input Folder\n");
			return 1;
		}
		while((_dirent = readdir(directory)) != NULL)
		{
			string fileName = inputDirectory + "\\" + string(_dirent->d_name);
			Mat rawImage = imread(fileName.c_str());

			if(rawImage.data == NULL)
			{
				printf("Cannot Open Image\n");
				continue;
			}


			IplImage* img=cvCreateImage(cvSize(rawImage.cols,rawImage.rows),8,3);
			IplImage ipltemp=rawImage;
			cvCopy(&ipltemp,img);

			IplImage* gimg = cvCreateImage(cvGetSize(img),8,1);
			cvCvtColor(img,gimg,CV_RGB2GRAY);

			//Creating a White copy of the original image

			IplImage* cpygimg = cvCloneImage(gimg);

			//KeyPoint Detection

			m=minHessian(img);
			cpygimg=Key_Point(img,m);

			//Initializing the erode image
	
			IplImage* erodeimg=cvCloneImage(cpygimg);

			//Applying the erosion function

			cvErode(cpygimg,erodeimg,NULL,2);

			max=Stat_Mode(erodeimg);

			//Applying erosion function in the smooth image

			if(max<=500)
				erosion_param=10;
			else
				erosion_param=5;

			cvErode(erodeimg,erodeimg,NULL,erosion_param);

			//Applying Canny Operator

			IplImage* cannyimg=cvCloneImage(erodeimg);
			cvCanny(erodeimg, cannyimg, 10, 100, 3);

			//Reverse the Canny image
		
			Reverse(cannyimg);

			//Applying Sobel Operator

			IplImage* sobelimg=cvCloneImage(cannyimg);
			cvSobel( cannyimg, sobelimg, 0, 1, 3 );
	
			//Reverse the Sobel image
		
			Reverse(sobelimg);

			//Converting the sobel image from GRAY to RGB

			IplImage* sobelimg_color = cvCreateImage(cvGetSize(sobelimg),8,3);
			cvCvtColor(sobelimg,sobelimg_color,CV_GRAY2RGB);

			//Merging the images

			IplImage* dst = cvCreateImage(cvGetSize(gimg),8,3);
			beta = ( 1.0 - alpha );
			cvAddWeighted( img, alpha, sobelimg_color, beta, 0.0, dst);

			//Display the final image

			cvShowImage("Example",dst);

			//Write the final image

			fileName = outputDirectory + "\\" +string(_dirent->d_name);
			cvSaveImage(fileName.c_str(), dst);
			cvWaitKey(0);
		}
		closedir(directory);
		getch();
		return 0;
}




















//SLIDING WINDOW


//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv\cv.h>
//#include <sys/types.h>
//#include <malloc.h>
//#include "dirent.h"
//#include <iostream>
//
//using namespace cv;
//using namespace std;
// 
//int main()
//{
//		initModule_nonfree();
//
//		int max,max1,m,erosion_param;
//		double alpha = 0.5,beta; 
//
//		int minHessian(IplImage*);
//		int Stat_Mode(IplImage*);
//		void Reverse(IplImage*);
//		IplImage* Key_Point(IplImage*,int);
//		IplImage* SlidingWindow(Mat);
//
//		//Input image
//
//		string inputDirectory = "E:\\Input";	//Set Input Directory as per your PC
//		string outputDirectory = "E:\\Output";  //Set Output Directory as per your PC
//		DIR *directory = opendir (inputDirectory.c_str());
//		struct dirent *_dirent = NULL;
//		if(directory == NULL)
//		{
//			printf("Cannot open Input Folder\n");
//			return 1;
//		}
//		while((_dirent = readdir(directory)) != NULL)
//		{
//			string fileName = inputDirectory + "\\" + string(_dirent->d_name);
//			Mat rawImage = imread(fileName.c_str());
//
//			if(rawImage.data == NULL)
//			{
//				printf("Cannot Open Image\n");
//				continue;
//			}
//
//			IplImage* img=cvCreateImage(cvSize(rawImage.cols,rawImage.rows),8,3);
//			IplImage ipltemp=rawImage;
//			cvCopy(&ipltemp,img);
//
//			IplImage* gimg=cvCreateImage(cvSize(rawImage.cols,rawImage.rows),8,1);
//			gimg=SlidingWindow(rawImage);
//			
//			IplImage* erodeimg=cvCloneImage(gimg);
//			cvErode(gimg,erodeimg,NULL,2);
//
//			max=Stat_Mode(erodeimg);
//
//			//Applying Canny Operator
//
//			IplImage* cannyimg=cvCloneImage(erodeimg);
//			cvCanny(erodeimg, cannyimg, 10, 100, 3);
//			
//			//Reverse the Canny image
//		
//			Reverse(cannyimg);
//
//			//Applying Sobel Operator
//
//			IplImage* sobelimg=cvCloneImage(cannyimg);
//			cvSobel( cannyimg, sobelimg, 0, 1, 3 );
//	
//			//Reverse the Sobel image
//		
//			Reverse(sobelimg);
//
//			//Converting the sobel image from GRAY to RGB
//
//			IplImage* sobelimg_color = cvCreateImage(cvGetSize(sobelimg),8,3);
//			cvCvtColor(sobelimg,sobelimg_color,CV_GRAY2RGB);
//
//			//Merging the images
//
//			IplImage* dst = cvCreateImage(cvGetSize(img),8,3);
//			beta = ( 1.0 - alpha );
//			cvAddWeighted( img, alpha, sobelimg_color, beta, 0.0, dst);
//
//			//Display the final image
//
//			cvShowImage("Example",dst);
//
//			//Write the final image
//
//			fileName = outputDirectory + "\\" +string(_dirent->d_name);
//			cvSaveImage(fileName.c_str(), dst);
//			cvWaitKey(0);
//		}
//		closedir(directory);
//		return 0;
//}