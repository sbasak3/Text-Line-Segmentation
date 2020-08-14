#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv\cv.h>
#include <sys/types.h>
#include <malloc.h>
#include<iostream>

using namespace std;
using namespace cv;

void Reverse(IplImage*);
IplImage* ComponentAnalysis(IplImage*);
IplImage* Key_Point(IplImage*,int);
int Stat_Mode(IplImage*);
int minHessian(IplImage*);

IplImage* SlidingWindow(Mat image)
 { 
	initModule_nonfree();
	int x,y,max1,max2,max,avg_max,item,stat_mode; 
	int length, width,step, channels,i,j,k=0,l=0,m=0,i1,j1,a,b; 
	int gimg_length, gimg_width,gimg_step, gimg_channels;
	int length1, width1,step1, channels1;
	uchar *data; 
	uchar *gimg_data;
	uchar *data1;
	short **bin; 
	short **gimg_bin;
	short **bin1;
	int *arr;

	//Creating a Blank copy of the original Image

	IplImage* gimg=cvCreateImage(cvSize(image.cols,image.rows),8,1);

	//Initializing the Blank Image parameters

	gimg_length=gimg->height;
	gimg_width=gimg->width; 
	gimg_step = gimg->widthStep;
	gimg_channels = gimg->nChannels;
	gimg_data= (uchar *)gimg->imageData;

	//Initialize 2D array of the Blank Image

	gimg_bin=(short**)calloc(gimg_width,sizeof(short*)); 
	for ( i = 0; i < gimg_length; i++ )
	{ 
		gimg_bin[i]=(short*)calloc(gimg_width,sizeof(short)); 
	}
	

	//Window

	Size smallSize (500,500);
	vector < Mat > smallImages ; 

	for ( y = 0 ; y <= image . cols ; y += smallSize . width ) 
	{ 
		if((y+smallSize . width)>image.cols) 
		{ 
			break; 
		}
		for ( x = 0 ; x <= image . rows ; x += smallSize . height) 
		{ 
			if((x+smallSize . height)>image.rows) 
			{ 
				break; 
			} 
			Rect rect = Rect ( y , x , smallSize . width, smallSize . height);
			smallImages . push_back (Mat(image,rect));

			// Mat to IplImage Conversion

			IplImage* img=cvCreateImage(cvSize(Mat(image,rect).cols,Mat(image,rect).rows),8,3);
			IplImage ipltemp=Mat(image,rect); 
			cvCopy(&ipltemp,img);

			//Creating a White copy of the original image

			IplImage* cpygimg = cvCreateImage(cvGetSize(img),8,1);

			//HSV Conversion

			IplImage* himg1 = cvCreateImage(cvGetSize(img),8,3);
			cvCvtColor(img,himg1,CV_RGB2HSV);

			//Creating copy of the HSV image

			IplImage* himg2 = cvCloneImage(himg1);

			//Initializing the 1st HSV image

			length=himg1->height;
			width=himg1->width; 
			step = himg1->widthStep;
			channels = himg1->nChannels;
			data= (uchar *)himg1->imageData;

			bin=(short**)calloc(width,sizeof(short*)); 
			for ( i = 0; i < length; i++ )
			{ 
				bin[i]=(short*)calloc(width,sizeof(short)); 
			}

			arr=(int*)calloc(255,sizeof(int));

			//Converting the image into a 2-D array

			for(i=0;i<length;i++) 
			for(j=0;j<width;j++)
			bin[i][j]=data[i*step+j*channels+2];

			int val=0; 
			for(int i=0;i<length;i++) 
			{ 
				for(int j=0;j<width;j++) 
				{ 
					val=bin[i][j]; 
					arr[val]=arr[val]+1; 
				}
			}

			//Calculating the Lower Peak

			max1=arr[1]; 
			for(i=0;i<=150;i++) 
			{ 
				if(arr[i]>max1)
				{ 
					max1=i; 
				}
			}

			//Calculating the Higher Peak

			max2=arr[150]; 
			for(i=151;i<=255;i++)
			{ 
				if(arr[i]>max2) 
				{ 
					max2=i; 
				} 
			}

			//Average Calculation

			avg_max=(max1+max2)/2;

			//Calculating the Highest Occurance of R

			max=arr[0];
			for(i=0;i<=255;i++) 
			{ 
				if(arr[i]>max)
				{ 
					max=i; 
				} 
			} 

			//Creating the Background Image

			for(i=0;i<length;i++) 
			for(j=0;j<width;j++) 
			data[i*step+j*channels+2]=max;

			free(bin);

			//Initializing the 2nd HSV image

			length=himg2->height; 
			width=himg2->width; 
			step = himg2->widthStep; 
			channels = himg2->nChannels;
			data= (uchar *)himg2->imageData;

			bin=(short**)calloc(width,sizeof(short*)); 
			for ( i = 0; i < length; i++ ) 
			{ 
				bin[i]=(short*)calloc(width,sizeof(short));
			}

			//Converting the image into a 2-D array

			for(i=0;i<length;i++) 
			for(j=0;j<width;j++) 
			bin[i][j]=data[i*step+j*channels+2];

			int c1=0;

			//Calculating the no of good foreground pixels

			for(int i=0;i<length;i++) 
			{ 
				for(int j=0;j<width;j++)
				{ 
					if(bin[i][j]<120) 
						c1++; 
				} 
			}
			
			free(bin);

			//Generating the Foreground Image

			IplImage* himg=cvCreateImage(cvSize(width,length),8,3); 
			cvSub(himg1,himg2,himg);


			//Initializing the Foreground image

			length=himg->height; 
			width=himg->width; 
			step = himg->widthStep; 
			channels = himg->nChannels;
			data= (uchar *)himg->imageData;

			bin=(short**)calloc(width,sizeof(short*)); 
			for ( i = 0; i < length; i++ ) 
			{ 
				bin[i]=(short*)calloc(width,sizeof(short));
			}

			//Converting the image into a 2-D array

			for(i=0;i<length;i++) 
			for(j=0;j<width;j++) 
			bin[i][j]=data[i*step+j*channels+2];

			int c=0; 

			//Calculating the no of foreground pixels

			for(int i=0;i<length;i++) 
			{ 
				for(int j=0;j<width;j++)
				{ 
					if(bin[i][j]>60) 
						c++; 
				} 
			} 



			//Calculating the percentage of the good foreground pixels

			float n;
			n=(float)c1/c*100;

			if(n>20)
			{
				if(n>20 && n<=30)
				m=300;
				if(n>30 && n<=40)
				m=400;
				if(n>40 && n<=50)
				m=500;
				if(n>50 && n<=60)
				m=600;
				if(n>60 && n<=70)
				m=700;
				if(n>70 && n<=100)
				m=800;
				
				cpygimg=Key_Point(img,m);

				//Initializing the Component image

				length1=cpygimg->height; 
				width1=cpygimg->width; 
				step1 = cpygimg->widthStep; 
				channels1 = cpygimg->nChannels;
				data1= (uchar *)cpygimg->imageData;

				bin1=(short**)calloc(width1,sizeof(short*)); 
				for ( i = 0; i < length1; i++ ) 
				{ 
					bin1[i]=(short*)calloc(width1,sizeof(short));
				}

				for(i=0;i<length1;i++) 
				for(j=0;j<width1;j++) 
				bin1[i][j]=data1[i*step1+j*channels1];
			}
			else
			{
				//Initializing the Component image

				length1=cpygimg->height; 
				width1=cpygimg->width; 
				step1 = cpygimg->widthStep; 
				channels1 = cpygimg->nChannels;
				data1= (uchar *)cpygimg->imageData;

				bin1=(short**)calloc(width1,sizeof(short*)); 
				for ( i = 0; i < length1; i++ ) 
				{ 
					bin1[i]=(short*)calloc(width1,sizeof(short));
				}

				for(i=0;i<length1;i++) 
				for(j=0;j<width1;j++) 
				bin1[i][j]=data1[i*step1+j*channels1];

				for(int i=0;i<length;i++) 
				{ 
					for(int j=0;j<width;j++)
					{ 
						if(bin[i][j]>60) 
						bin1[i][j]=0;
					} 
				}

				for(i=0;i<length1;i++) 
				for(j=0;j<width1;j++) 
				data1[i*step1+j*channels1]=bin1[i][j];

				IplImage *binimg=cvCreateImage(cvGetSize(cpygimg),8,1);
				cvThreshold(cpygimg,binimg,128,255,CV_THRESH_BINARY);

				IplImage* erodeimg=cvCreateImage(cvGetSize(cpygimg),8,1);
				IplConvKernel* ee;
				ee = cvCreateStructuringElementEx(2, 4, 1, 3, CV_SHAPE_RECT);
				cvErode(binimg,erodeimg,ee,4);

				length1=erodeimg->height; 
				width1=erodeimg->width; 
				step1 = erodeimg->widthStep; 
				channels1 = erodeimg->nChannels;
				data1= (uchar *)erodeimg->imageData;


				bin1=(short**)calloc(width1,sizeof(short*)); 
				for ( i = 0; i < length1; i++ ) 
				{ 
					bin1[i]=(short*)calloc(width1,sizeof(short));
				}

				for(i=0;i<length1;i++) 
				for(j=0;j<width1;j++) 
				bin1[i][j]=data1[i*step1+j*channels1];

				free(bin);
			}



			//Merging the windows

			for(a=k*smallSize.height, i1=0;		a<(k+1)*smallSize.height, i1<length;		a++, i1++)
			{
				for(b=l*smallSize.width, j1=0;		b<(l+1)*smallSize.width, j1<width;		b++, j1++)
				{
					item=bin1[i1][j1];
					gimg_data[a*gimg_step+b*gimg_channels]=item;
				}
			}

			free(bin1);
			k++;
			cvWaitKey(0); 
		}
		k=0;
		l++;
	}
	return gimg;	
}