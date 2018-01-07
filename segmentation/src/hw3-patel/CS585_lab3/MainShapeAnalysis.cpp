//
//  MainShapeAnalysis.cpp
//  Assignment 3
//
//  Created by Shantanu Bobhate on 10/3/17.
//  Copyright © 2017 Shantanu Bobhate. All rights reserved.
//

#include "MainShapeAnalysis.hpp"


/*
 * Function to run complete Shape Analysis on Dataset 1
 */
void RunShapeAnalysis() {
    /* First Image */
    Mat image = imread("open-bw-full.png");
    Mat binary = Mat::zeros(image.rows, image.cols, CV_8UC1);
    if (image.empty()) {
        cout << "Error: Could not open image" << endl;
    }
    
    ConvertBinary(image, binary);
    Erode(binary);
    
    ConnectedComponentLabelling(binary, image);
    
    /* Second Image */
    image = imread("open-bw-partial.png");
    if (image.empty()) {
        cout << "Error: Could not open image" << endl;
    }
    
    binary = Mat::zeros(image.rows, image.cols, CV_8UC1);
    ConvertBinary(image, binary);
    Erode(binary)
    ;
    Dilate(binary);
    
    ConnectedComponentLabelling(binary, image);
    
    /* Third Image */
    image = imread("open_fist-bw.png");
    if (image.empty()) {
        cout << "Error: Could not open image" << endl;
    }
    
    binary = Mat::zeros(image.rows, image.cols, CV_8UC1);
    ConvertBinary(image, binary);
    Erode(binary);
    
    ConnectedComponentLabelling(binary, image);
    
    /* Fourth Image */
    image = imread("tumor-fold.png");
    if (image.empty()) {
        cout << "Error: Could not open image" << endl;
    }
    
    binary = Mat::zeros(image.rows, image.cols, CV_8UC1);
    ConvertBinary(image, binary);
    Dilate(binary);
    Dilate(binary);
    Dilate(binary);
    
    ConnectedComponentLabelling(binary, image);
}

/*
 * Function to perform erosion
 */
void Erode(Mat& srcImage)
{
    int height = srcImage.rows-1, width = srcImage.cols-1;
    for (int rowCurr = 1; rowCurr < height; rowCurr++)
    {
        int rowPrev = rowCurr-1, rowNext = rowCurr+1;
        for (int colCurr = 1; colCurr < width; colCurr++)
        {
            int colPrev = colCurr-1, colNext = colCurr+1;
            if (srcImage.at<uchar>(rowCurr, colCurr) == 0)
            {
                if (srcImage.at<uchar>(rowPrev, colPrev) == 255)
                    srcImage.at<uchar>(rowPrev, colPrev) = 150;
                if (srcImage.at<uchar>(rowPrev, colNext) == 255)
                    srcImage.at<uchar>(rowPrev, colNext) = 150;
                if (srcImage.at<uchar>(rowNext, colPrev) == 255)
                    srcImage.at<uchar>(rowNext, colPrev) = 150;
                if (srcImage.at<uchar>(rowNext, colNext) == 255)
                    srcImage.at<uchar>(rowNext, colNext) = 150;
            }
        }
    }
    
    for (int rowCurr = 1; rowCurr < height; rowCurr++)
    {
        for (int colCurr = 1; colCurr < width; colCurr++)
        {
            if (srcImage.at<uchar>(rowCurr, colCurr) == 150)
            {
                srcImage.at<uchar>(rowCurr, colCurr) = 0;
            }
        }
    }
}

/*
 * Function to perform dilation
 */
void Dilate(Mat& srcImage)
{
    int height = srcImage.rows-1, width = srcImage.cols-1;
    for (int rowCurr = 1; rowCurr < height; rowCurr++)
    {
        int rowPrev = rowCurr-1, rowNext = rowCurr+1;
        for (int colCurr = 1; colCurr < width; colCurr++)
        {
            int colPrev = colCurr-1, colNext = colCurr+1;
            if (srcImage.at<uchar>(rowCurr, colCurr) == 255)
            {
                if (srcImage.at<uchar>(rowPrev, colPrev) == 0)
                    srcImage.at<uchar>(rowPrev, colPrev) = 150;
                if (srcImage.at<uchar>(rowPrev, colNext) == 0)
                    srcImage.at<uchar>(rowPrev, colNext) = 150;
                if (srcImage.at<uchar>(rowNext, colPrev) == 0)
                    srcImage.at<uchar>(rowNext, colPrev) = 150;
                if (srcImage.at<uchar>(rowNext, colNext) == 0)
                    srcImage.at<uchar>(rowNext, colNext) = 150;
            }
        }
    }
    
    for (int rowCurr = 1; rowCurr < height; rowCurr++)
    {
        for (int colCurr = 1; colCurr < width; colCurr++)
        {
            if (srcImage.at<uchar>(rowCurr, colCurr) == 150)
            {
                srcImage.at<uchar>(rowCurr, colCurr) = 255;
            }
        }
    }
}

/*
 * Function to convert a 3 channel image to a 1 channel image
 */
void ConvertBinary(Mat& srcImage, Mat& binaryImage) {
    int channels = srcImage.channels();
    for (int row = 0; row < srcImage.rows; row++)
    {
        unsigned char* rowPtr = srcImage.ptr<unsigned char>(row);
        unsigned char* binPtr = binaryImage.ptr<unsigned char>(row);
        for (int col = 0; col < srcImage.cols; col++)
        {
            int r = col*channels + 2;
            int g = col*channels + 1;
            int b = col*channels + 0;
            if(rowPtr[r] == 0 || rowPtr[g] == 0 || rowPtr[b] == 0) binPtr[col] = 0;
            else binPtr[col] = 255;
        }
    }
}
