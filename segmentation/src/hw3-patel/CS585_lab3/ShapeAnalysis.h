/*
*
* ShapeAnalysis.h
*
* Contains functions related to Shape Analysis.
*
* @authors		 : Shantanu Bobhate, Kasim Patel and Zuhayr Raghib
* @date_created : September 26th, 2017
*
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

/*
 * Shape Analysis Task:
 *
 *		i.   Implement a connected component labelling algorithm to given data and give the different components different colors. (DO NOT USE OpenCV function to avoid losing 5 points). [DONE]
 *		ii.	 Use erosion or filter components using size to reduce the number of detected components. [DONE] -- Needs to be refined
 *		iii. Implement a boundary following algorithm and apply to the relevant objects. [DONE]
 *		iv.	 For each component compute its area, orientation and circularity (E_min/E_max ratio). Also count the number of boundary pixels (perimeter) and compute the compactness ratio. [DONE]
 *		v.	 Implement a skeleton finding algorithm. [DONE]
 */

void ConnectedComponentLabelling(Mat& srcImage, Mat& dstImage);
void PutOnStackNeighbours(Mat& srcImage, vector<Point>& stack, Point currentPixel, int& label);
void LabelPixel(Mat& srcImage, vector<Point>& stack, int currentX, int currentY, int& label);
int BoundaryFollowing(Mat& srcImage, Mat& dstImage, Point startPoint);
void WhereToGoNext(Point& P, Point& b);
Point FindCentroid(Mat& srcImage, Mat& dstImage, int label, int area);
double FindOrientation(Mat& srcImage, Mat& dstImage, int label, int x_mean, int y_mean);
double FindCircularity(Mat& srcImage, int label, int area, int x_mean, int y_mean);
double FindCompactness(int perimeter, int area);
void MedialAxisTransform(Mat& srcImage, Mat& dstImage, int label);
int FindMyNewValIter1(Mat& srcImage, Point currentPixel);
int FindMyNewValIter2(Mat& srcImage, Point currentPixel);
