//
//  Segmentation3.cpp --> code for dataset3
//  
//
//  Created by Muhammad Zuhayr Raghib on 9/30/17.
//
//

#include "dataset1.h"


void run_dataset_1(){

	Mat image, gray, a_thresh, prevFrame, tmp;
	Mat grayBG;
	int num[19] = { 14, 15, 16, 17, 18, 19, 22, 23, 24, 25, 26, 27, 33, 34, 35, 36, 37, 38, 39 };


	tmp = imread("Z:/CS585-PianoImages/piano_" + to_string(num[0]) + ".png");
	resize(tmp, tmp, Size(), 0.25, 0.25);
	double height = tmp.rows; double width = tmp.cols;
	Rect roiRect = Rect(240, 100, width - 560, height - 200);
	Mat roi = tmp(roiRect);
	Mat first = roi.clone();

	Mat result = Mat::zeros(first.rows, first.cols, CV_64FC3); //used 64 bit to avoid overflow
	Mat temp;
	roi.convertTo(temp, CV_64FC3); // convert to same size as sum
	result = result + temp;

	for (int index = 1; index < 19; index++) {
		string filename = "Z:/CS585-PianoImages/piano_" + to_string(num[index]) + ".png";
		image = imread(filename);
		resize(image, image, Size(), 0.25, 0.25);
		cout << "Resized Height: " << height << " ; Resized Width: " << width << endl;
		roiRect = Rect(240, 100, width - 560, height - 200);
		roi = image(roiRect);

		roi.convertTo(temp, CV_64FC3); // convert to same size as sum
		result = result + temp;

	}

	result = result / 19;
	result.convertTo(result, first.type());
	
	cvtColor(result, grayBG, CV_BGR2GRAY);
	GaussianBlur(grayBG, grayBG, Size(7, 7), 3);

	//imshow("BG", grayBG);
	waitKey();


	tmp = imread("Z:/CS585-PianoImages/piano_" + to_string(num[0]) + ".png");
	resize(tmp, tmp, Size(), 0.25, 0.25);
	// double height = tmp.rows; double width = tmp.cols;
	roiRect = Rect(240, 100, width - 560, height - 200);
	roi = tmp(roiRect);
	cvtColor(roi, prevFrame, CV_BGR2GRAY);
	Mat diff;
	vector< vector<Point> > contours;
	vector< Vec4i > hierarchy;

	for (int index = 1; index < 18; index++) {
		cout << index << endl;

		string filename = "Z:/CS585-PianoImages/piano_" + to_string(num[index]) + ".png";
		image = imread(filename);
		resize(image, image, Size(), 0.25, 0.25);
		cout << "Resized Height: " << height << " ; Resized Width: " << width << endl;
		roiRect = Rect(240, 100, width - 560, height - 200);
		roi = image(roiRect);



		cvtColor(roi, gray, CV_BGR2GRAY);
		GaussianBlur(gray, gray, Size(7, 7), 3);

		//mySkinDetect(roi, gray, grayBG);

		
		absdiff(gray, grayBG, diff);
		a_thresh = Mat::zeros(roi.rows, roi.cols, CV_8UC1);
		adaptiveThreshold(diff, a_thresh, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, -5);
		//erode(a_thresh, a_thresh, Mat());
		//imshow("Erode", a_thresh);
		dilate(a_thresh, a_thresh, Mat());
		//dilate(a_thresh, a_thresh, Mat());
		//dilate(a_thresh, a_thresh, Mat());
		//imshow("Dilate", a_thresh);



		contours.clear();
		hierarchy.clear();

		findContours(a_thresh.clone(), contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_NONE);
		double maxArea1 = 0, maxArea2 = 0;
		int maxIndex1 = 0, maxIndex2 = 0;
		for (int ii = 0; ii < contours.size(); ii++) {
			double area = contourArea(contours[ii]);
			if (area > maxArea1) {
				maxArea1 = area;
				maxIndex1 = ii;
			}
			else if (area < maxArea1 && area > maxArea2) {
				maxArea2 = area;
				maxIndex2 = ii;
			}
		}

		drawContours(roi, contours, maxIndex1, Scalar(0, 0, 255));
		drawContours(roi, contours, maxIndex2, Scalar(0, 0, 255));


		namedWindow("Region of Interest", CV_WINDOW_AUTOSIZE);
		imshow("Region of Interest", roi);
		//imshow("Adaptive Threshold", a_thresh);
		//imshow("Frame Diff", diff);
		waitKey(500);

	}

	destroyAllWindows();

}