//
//  tracking_fish.cpp
//  Project :- Assignment 4
//
//  Created by Muhammad Kasim Patel on 10/30/17.
//  Collaborators :- Shantanu Bobhate and Muhammad Zuhayr Raghib
//  Copyright © 2017 Muhammad Kasim Patel. All rights reserved.
//


#include "fish.h"


void run_fish(){

	//vector <vector<Point2f>> cent;

	// input images
	vector<Mat> fish = readFiles();
	RNG rng(12345);
	vector<Mat> segfish = readsegFiles();
	Mat thres_3;
	Mat src1_gray;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;


	for (int i = 0; i < segfish.size(); i++){
		// Load source image and convert it to gray
		Mat src1 = segfish[i];

		// Create Window and display source image
		namedWindow("Source", CV_WINDOW_AUTOSIZE);
		imshow("Source", src1);
		
		// COnverting Image to HSV for getting the segmented part
		Mat hsv_image;
		cvtColor(src1, hsv_image, cv::COLOR_BGR2HSV);
		Mat lower_red_hue_range;
		Mat upper_red_hue_range;
		inRange(hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lower_red_hue_range);
		inRange(hsv_image, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upper_red_hue_range);
		Mat red_hue_mask;
		addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_mask);
		Mat result;
		src1.copyTo(result, red_hue_mask);
		//namedWindow("Source1", CV_WINDOW_AUTOSIZE);
		//imshow("Source1", result);
		
		//Convert Image to BGR again
		Mat bgr_image;
		cvtColor(result, bgr_image, cv::COLOR_HSV2BGR);

		// Convert image to gray
		cvtColor(bgr_image, src1_gray, CV_BGR2GRAY);
		
		// Smoothen Image
		//blur(src1_gray,src1_gray, Size(3, 3));

		//thresholding
		threshold(src1_gray, thres_3, 10, 255, THRESH_BINARY);
		//Canny(src1_gray, thres_3, 100, 100 * 2, 3);
		
		//Perform Erosion and Dilation
		int erosion_size = 1;
		int dilation_size = 1;
		Mat element = getStructuringElement(MORPH_RECT,
			Size(1 * erosion_size + 1, 1 * erosion_size + 1),
			Point(erosion_size, erosion_size));
		//erode(thres_3, thres_3, element);
		//dilate(thres_3, thres_3, element);


		//namedWindow("Contours3", CV_WINDOW_AUTOSIZE);
		//imshow("Contours3", thres_3);

		//Find Contours
		//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		Mat contour_output = Mat::zeros(thres_3.size(), CV_8UC3);
		findContours(thres_3, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

		Rect boundrec;

		/// Find the rotated rectangles and ellipses for each contour
		vector<RotatedRect> minRect(contours.size());
		vector<RotatedRect> minEllipse(contours.size());

		//Draw Contours
		for (int i = 0; i < contours.size(); i++)
		{
			minRect[i] = minAreaRect(Mat(contours[i]));
			if (contours[i].size() > 5)
			{
				minEllipse[i] = fitEllipse(Mat(contours[i]));
			}
			//cout << "The area of contour is: " << contourArea(contours[i]) << endl;

		}
		int c = 0;
		vector<Moments> mu(contours.size());
		vector<Point2f> mc(contours.size());
		//vector <Point2f> c_cent(contours.size());

		/// Draw contours rotated rects +
		for (int i = 0; i < contours.size(); i++) // number of fish
		{
			//c_cent.clear();
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			if (contourArea(contours[i]) > 0){
				c = c + 1;
				mu[i] = moments(contours[i], false);
				mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
				cout << mc[i] << endl;
				//c_cent.push_back(mc[i]);
				//cout << "Centroids: " << mc[i] << endl;
				// drawing centroids
				circle(contour_output, mc[i], 1, Scalar(0, 0, 255), -1, 1, 0);
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				
				
				// drawing contours
				drawContours(contour_output, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
				
				//draw ellipse shapes around contours
				drawContours(contour_output, contours, i, color, CV_FILLED);
				ellipse(contour_output, minEllipse[i], color, 1, 8);
			}			
		}
		//cent.push_back(c_cent);

		namedWindow("Contours2", CV_WINDOW_AUTOSIZE);
		imshow("Contours2", contour_output);
		waitKey(5);

	}
	//return cent;
} // end of runfish()

/*
vector get_measurements(vector <vector <Vertex *>>& measurements, vector <vector<Point2f>> cent) {

	vector <Vertex *> measurementsForSingleFrame;  // vector to store centroids for a certain frame
	cout << "here0" << endl;

	for (int ii = 0; ii < cent.size(); ii++){
			cout << "here1" << endl;
			measurementsForSingleFrame.clear();
			for (int jj = 0; jj < cent[ii].size(); jj++){
				cout << "here2" << endl;

				Vertex *newVertex = new Vertex();
				newVertex->position = cent[ii] [jj];
				newVertex->velocity = Point(V_INIT, V_INIT);
				newVertex->estimate = newVertex->position;
				newVertex->truth = newVertex->position;
				measurementsForSingleFrame.push_back(newVertex);
			}
		measurements.push_back(measurementsForSingleFrame);
	}
		for (auto& i : measurements){
			for (auto& j : i){
				//cout << j << " kutti" << endl;
			}
		}
			

}
*/


/*Read in all images from folder*/
vector<Mat> readFiles(){
	vector<string> files;
	vector<Mat> images;
	string v, vv, vvv;

	for (int i = 10; i <= 99; i++){
		v = "Z:/fish_tank/2014_aq_unprocessed 0";
		vv = to_string(i);
		vvv = v + vv + ".jpg";
		files.push_back(vvv);
		Mat src = imread(vvv, 1);
		images.push_back(src);
		//imshow("fish 1", src);
		waitKey(5);
	}
	for (int i = 10; i <= 99; i++){
		v = "Z:/fish_tank/2014_aq_unprocessed 1";
		vv = to_string(i);
		vvv = v + vv + ".jpg";
		files.push_back(vvv);
		Mat src = imread(vvv, 1);
		images.push_back(src);
		//imshow("fish 2", src);
		waitKey(5);
	}
	return images;
}


/*Read in all segmented images from folder*/
vector<Mat> readsegFiles(){
	vector<string> files;
	vector<Mat> images;
	string v, vv, vvv;

	for (int i = 10; i <= 53; i++){
		v = "Z:/fish_tank_seg/2014_aq_segmented ";
		vv = to_string(i);
		vvv = v + vv + ".jpg";
		files.push_back(vvv);
		Mat src = imread(vvv, 1);
		images.push_back(src);
		//imshow("fish 1", src);
		waitKey(5);
	}
	return images;
}


