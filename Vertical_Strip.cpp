#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv\cv.h>
#include <sys/types.h>
#include <malloc.h>
#include<iostream>


IplImage* Vertical_Stripe(IplImage* img)
{
			int length,width,step,channels,cpygimg_length,cpygimg_width,cpygimg_step,cpygimg_channels,no,a,b,i,j,k,l=0,m,c,*arr;
			short **bin,**cpygimg_bin; 
			uchar *data,*cpygimg_data;

			IplImage* cpygimg = cvCreateImage(cvGetSize(img),8,1);
			cvSet(cpygimg, cvScalar(255));

			//Initializing the image and the 2D array

			length=img->height;
			width=img->width; 
			step = img->widthStep;
			channels = img->nChannels;
			data= (uchar *)img->imageData;

			bin=(short**)calloc(length,sizeof(short*)); 
			for ( i = 0; i <length; i++ )
			{ 
				bin[i]=(short*)calloc(width,sizeof(short)); 
			}


			for(i=0;i<length;i++) 
			{
				for(j=0;j<width;j++)
				{
					bin[i][j]=data[i*step+j*channels];
				}
			}

			//Initializing the copy image and the 2D array

			cpygimg_length=cpygimg->height;
			cpygimg_width=cpygimg->width; 
			cpygimg_step = cpygimg->widthStep;
			cpygimg_channels = cpygimg->nChannels;
			cpygimg_data=(uchar *)cpygimg->imageData;

			cpygimg_bin=(short**)calloc(cpygimg_length,sizeof(short*)); 
			for ( i = 0; i <cpygimg_length; i++ )
			{ 
				cpygimg_bin[i]=(short*)calloc(cpygimg_width,sizeof(short)); 
			}

			for(i=0;i<cpygimg_length;i++) 
			{
				for(j=0;j<cpygimg_width;j++)
				{
					cpygimg_bin[i][j]=cpygimg_data[i*cpygimg_step+j*cpygimg_channels];
				}
			}

			//Initializing the 1D array for every Stripe

			arr=(int*)calloc(length,sizeof(int));

			//Grid Division

			no=30;
		
			for(i=0;i<length;i++)						
			{
				for(j=0;j<width;j++)
				{
					for(k=1;k<=no;k++)
					{
						if(j==k*(width/no))
						{
							bin[i][j]=1;
						}
					}
				}
			}

			//Gridwise coloring
		
			l=1;
			m=width/no;
		
			while((l*m)<width)
			{
				for(i=0;i<length;i++)
				{
					c=0;
					for(j=(l-1)*m;j<=l*m;j++)
					{
						if(bin[i][j]==0)
						{
							c++;
						}
					
					}
					arr[i]=c;
				}

				for(a=0;a<length;a++)
				{
					if(arr[a]==0)
					{
						for(b=(l-1)*m;b<=l*m;b++)
						{
							cpygimg_bin[a][b]=0;
						}
					}
				}
				l++;
			}

			for(i=0;i<length;i++) 
			for(j=0;j<width;j++)
			data[i*step+j*channels]=bin[i][j];


			for(i=0;i<cpygimg_length;i++) 
			for(j=0;j<cpygimg_width;j++)
			cpygimg_data[i*cpygimg_step+j*cpygimg_channels]=cpygimg_bin[i][j];
			
			free(bin);
			free(cpygimg_bin);

			return cpygimg;
}