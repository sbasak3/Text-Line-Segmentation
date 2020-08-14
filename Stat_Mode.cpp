#include<iostream>
#include <opencv\cv.h>
#include <malloc.h>


int Stat_Mode(IplImage* img)
{
	
			int length,width,i,j,i1,j1,k,l,c,number,*arr,max,step,channels,x;
			short **bin;
			uchar *data;
			
			//Initializing components of the erode image

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

			//Initializing the 1-D array

			arr=(int*)calloc(width,sizeof(int));
	
			//Converting the image into a 2-D array

			for(i=0;i<length;i++) 
			for(j=0;j<width;j++)
			for(k=0;k<channels;k++)
			bin[i][j]=data[i*step+j*channels+k];

			//Binary Conversion

			for(i=0;i<length;i++)
			{
				for(j=0;j<width;j++)
				{
					if(bin[i][j]==255)
					{
						bin[i][j]=0;
					}
					else
					bin[i][j]=1;
				}
			}

			//Calculating Statistical Mode
	
			for(i=0;i<length;i++)
			{
				c=0;
				for(j=0;j<width;j++)
				{
			
     				if(bin[i][j]==1 && bin[i][j+1]==0)
					{
						for(k=j+1;k<width;k++)
						{
							if(bin[i][k]==0)
							c++;
							else
							{
								arr[c]=arr[c]+1;
								c=0;
								break;
							}
						}
					}
				}
			}		
	
			max=arr[1];
			for(i=1;i<width;i++)
			{
				if(arr[i]>max)
				{
					max=i;
				}	
			}


			//Applying Smoothing Function

			j=0;
			i=0;
			while(i<length)
			{
				number=bin[i][j];
				if(number==1)
				{
					i1=i;
					j1=j;
					j++;
					if(j==width)
					{
						j=0;
						i++;
					}
					c=0;
					while(bin[i][j]==1)
					{
						i1=i;
						j1=j;
						j++;
						if(j==width)
						{
							j=0;
							i++;
						} 
						if(i==length)
						break;
					}
           
					while(i<length && bin[i][j]==0)
					{
						c++;
						j++;
						if(j==width)
						{
							j=0;
							i++;
						}
						if(i==length)
						break;
					}
           
					if( i< length && c<=2*(max))
					{
						k=i1;
						l=j1;
						do
						{
							bin[k][l]=1;
							l++;
							if(l==width)
							{
								l=0;
								k++;
							}
							if(k==length)
							break;
						}while(bin[k][l]==0);
					}     
				}
				else
				{
					j++;
					if(j==width)
					{
						j=0;
						i++;
					}
				}
			}

			//Gray Scale Conversion

			for(i=0;i<length;i++)
			{
				for(j=0;j<width;j++)
				{
					if(bin[i][j]==0)
					{
						bin[i][j]=255;
					}
					else
					bin[i][j]=0;
				}
			}

			//2-D array to IplImage conversion

			for(i=0;i<length;i++) 
			for(j=0;j<width;j++)
			for(k=0;k<channels;k++)
			data[i*step+j*channels+k]=bin[i][j];

			free(bin);
			free(arr);
			return max;
}