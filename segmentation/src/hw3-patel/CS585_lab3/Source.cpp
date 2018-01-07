/*
 @authors		 : Shantanu Bobhate, Muhammad Kasim Patel and Muhammad Zuhayr Raghib
 @date_created   : September 26th, 2017

CS585 Image and Video Computing Fall 2017
Assignment 3
--------------

*/
#include <iostream>
#include <vector>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include <opencv2\core\core.hpp>
#include <opencv2\imgcodecs\imgcodecs.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>


#include "dataset1.h"
#include "dataset2.h"
#include "dataset3.h"
#include "MainShapeAnalysis.h"
#include "ShapeAnalysis.h"

using namespace cv;
using namespace std;

// main function
int main()
{

	/*Fish tank*/

	run_dataset_2();


	
	waitKey(0);
	return(0);
}





void ConvertGrayscale(Mat& image, Mat& grayImage)
{
	int channels = image.channels();
	for (int row = 0; row < image.rows; row++)
	{
		unsigned char* rowPtr = image.ptr<unsigned char>(row);
		unsigned char* grayPtr = grayImage.ptr<unsigned char>(row);
		for (int col = 0; col < image.cols; col++)
		{
			int r = col*channels + 2;
			int g = col*channels + 1;
			int b = col*channels + 0;
			int gray = (rowPtr[r] + rowPtr[g] + rowPtr[b]) / 3;
			grayPtr[col] = gray;
		}
	}
}

