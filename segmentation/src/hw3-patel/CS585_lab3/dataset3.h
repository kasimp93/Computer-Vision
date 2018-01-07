/*
*
* dataset3.h
*
* Contains functions related to Segmentation3 (for dataset 3).
*
* @authors		 : Shantanu Bobhate, Muhammad Kasim Patel and Muhammad Zuhayr Raghib
* @date_created : September 26th, 2017
*
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <opencv2\core\core.hpp>
#include <opencv2\imgcodecs\imgcodecs.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace std;
using namespace cv;


vector<Mat> readFiles();
Mat background(vector<Mat>&);

int process(Mat& src, Mat& background);


void run_dataset_3();