#include <opencv\cv.h>
#include <malloc.h>
#include<iostream>


void Reverse(IplImage* img)
{
	int length,width,i,j,k,step,channels;
	short **bin;
	uchar *data;
	

			//Initializing components of the image

			length=img->height;
			width=img->width;
			step = img->widthStep;
			channels = img->nChannels;
			data= (uchar *)img->imageData;

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

			for(i=0;i<length;i++)
			{
				for(j=0;j<width;j++)
				{	
					if(bin[i][j]==255)
					{
						bin[i][j]=0;
					}
					else
					bin[i][j]=255;
				}
			}

			for(i=0;i<length;i++) 
			for(j=0;j<width;j++)
			for(k=0;k<channels;k++)
			data[i*step+j*channels+k]=bin[i][j];

			free(bin);			
}