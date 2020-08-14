#include <opencv\cv.h>
#include <malloc.h>
#include<iostream>


int minHessian(IplImage* img)
{
	int length,width,i,j,k,step,channels,minHessian;
	short **bin;
	long long int sum,sum_squares,n;
	double mean;
	long double variance,SD;
	uchar *data;
	
			//GRAY Conversion

			IplImage* gimg = cvCreateImage(cvGetSize(img),8,1);
			cvCvtColor(img,gimg,CV_RGB2GRAY);

			//Initializing components of the image

			length=gimg->height;
			width=gimg->width;
			step = gimg->widthStep;
			channels = gimg->nChannels;
			data= (uchar *)gimg->imageData;

			//Initializing the 2-D array

			bin=(short**)calloc(length,sizeof(short*));
			for ( i = 0; i < length; i++ )
			{
				bin[i]=(short*)calloc(width,sizeof(short));
			}

			//Converting the image into a 2-D array

			for(i=0;i<length;i++)
			for(j=0;j<width;j++)
			for(k=0;k<channels;k++)
			bin[i][j]=data[i*step+j*channels+k];

			//Standard Deviation of the Gray Values

			sum = 0; 
			sum_squares = 0;
			for(i=0;i<length;i++)
			{
				for(j=0;j<width;j++)
				{
					sum += bin[i][j];
					sum_squares += bin[i][j] * bin[i][j];
				}
			}
			n=length*width;
			if (n > 0) 
			{
				mean = (double)sum / n;
				variance = (long double)(((sum_squares-(2*mean*sum))/n)+(mean*mean));
				SD = sqrt(variance);
			}
			
			//Initializing the minHessian Parameter

			minHessian=0;
			if(SD<=20)
			{
				minHessian=400;
			}
			else
			{
				minHessian=((SD-20)*160)+400;
			}

			free(bin);
			return minHessian;
}