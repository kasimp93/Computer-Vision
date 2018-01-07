//
//  MainShapeAnalysis.hpp
//  Assignment 3
//
//  Created by Shantanu Bobhate on 10/3/17.
//  Copyright © 2017 Shantanu Bobhate. All rights reserved.
//

#ifndef MainShapeAnalysis_hpp
#define MainShapeAnalysis_hpp

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "ShapeAnalysis.h"

using namespace std;
using namespace cv;

void RunShapeAnalysis();
void Erode(Mat& srcImage);
void Dilate(Mat& srcImage);
void ConvertBinary(Mat& image, Mat& binaryImage);

#endif /* MainShapeAnalysis_hpp */
