//
//  tracking_fish.cpp
//  Project :- Assignment 4
//
//  Created by Muhammad Kasim Patel, Shantanu Bobhate and Muhammad Zuhayr Raghib on 10/30/17.
//  Copyright © 2017 Muhammad Kasim Patel. All rights reserved.
//


#include "fish.h"


vector <vector<Point2f>> run_fish(){


	// input images
	vector<Mat> fish = readFiles();
	RNG rng(12345);
	vector<Mat> segfish = readsegFiles();
	Mat thres_3;
	Mat src1_gray;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	vector <vector<Point2f>> cent;


	for (int i = 0; i < segfish.size(); i++){
		// Load source image and convert it to gray
		Mat src1 = segfish[i];

		// Create Window and display source image
		namedWindow("Source", CV_WINDOW_AUTOSIZE);
		//imshow("Source", src1);

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
		namedWindow("Source1", CV_WINDOW_AUTOSIZE);
		//imshow("Source1", result);

		//Convert Image to BGR again
		Mat bgr_image;
		cvtColor(result, bgr_image, cv::COLOR_HSV2BGR);

		// Convert image to gray
		cvtColor(bgr_image, src1_gray, CV_BGR2GRAY);

		//thresholding
		threshold(src1_gray, thres_3, 10, 255, THRESH_BINARY);

		namedWindow("Contours3", CV_WINDOW_AUTOSIZE);
		//imshow("Contours3", thres_3);

		//Find Contours
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
	

		}
		int c = 0;
		vector<Moments> mu(contours.size());
		vector<Point2f> mc(contours.size());
		vector <Point2f> c_cent(contours.size());

		/// Draw contours rotated rects +
		c_cent.clear();

		for (int i = 0; i < contours.size(); i++) // number of fish
		{
			Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			if (contourArea(contours[i]) > 0){
				c = c + 1;
				mu[i] = moments(contours[i], false);
				mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);

				c_cent.push_back(mc[i]);

				// drawing centroids
				circle(contour_output, mc[i], 1, Scalar(0, 0, 255), -1, 1, 0);
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));


				// drawing contours
				drawContours(contour_output, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());

				//draw ellipse shapes around contours
				drawContours(contour_output, contours, i, color);
				ellipse(contour_output, minEllipse[i], color, 1, 8);
				
			}
		}

		cout << c_cent.size() << endl;

		cent.push_back(c_cent);

		
		namedWindow("Contours2", CV_WINDOW_AUTOSIZE);
		//imshow("Contours2", contour_output);
		waitKey(5);
		
	}

	return cent;
} // end of runfish()

double EuclideanDistance(Point a, Point b) {
	return sqrt(pow(abs(a.x - b.x), 2) + pow(abs(a.y - b.y), 2));
}

Point FindClosestPoint(vector<Point> points, Point point) {
	int minDist = MIN_DIST;
	int minIdx = 0;
	for (int ii = 0; ii < points.size(); ii++) {
		int dist = EuclideanDistance(point, points[ii]);
		if (dist < minDist) {
			minDist = dist;
			minIdx = ii;
		}
	}

	return points[minIdx];
}

void InitializeVelocityMemory() {
	for (int ii = 0; ii < ROWS; ii++) {
		for (int jj = 0; jj < COLS; jj++) {
			memory[ii][jj] = NULL;
		}
	}
}

void AlphaBetaFilter1(vector <vector <Vertex *>>& measurements) {
	// Initialize the velocity memory structure
	InitializeVelocityMemory();
	//cout << "Size" << measurements.size() << endl;
	// Loop for every frame
	for (int ii = 0; ii < measurements.size()-1; ii++) {
		Mat secondFrame = Mat::zeros(ROWS, COLS, CV_8UC1);
		for (int jj = 0; jj < measurements[ii + 1].size(); jj++) {
			secondFrame.at<uchar>(measurements[ii + 1][jj]->position.y, measurements[ii + 1][jj]->position.x) = 1;
		}

		// Make prediction for every point
		for (int jj = 0; jj < measurements[ii].size(); jj++) {
			Vertex *currentMeasurement = measurements[ii][jj];

			int xk_1 = currentMeasurement->position.x;
			int yk_1 = currentMeasurement->position.y;
			Vertex *parentMeasurement = memory[xk_1][yk_1];

			int vk_1_x = V_INIT, vk_1_y = V_INIT;
			if (parentMeasurement != NULL) {
				vk_1_x = parentMeasurement->velocity.x;
				vk_1_y = parentMeasurement->velocity.y;
				currentMeasurement->color = parentMeasurement->color;
			}
			else {
				currentMeasurement->color = Scalar(rand() % 255, rand() % 255, rand() % 255)
					;
			}

			//cout << "Previous Velocity was (" << vk_1_x << ", " << vk_1_y << ")" << endl;

			int xk, yk, vk_x, vk_y, rk_x, rk_y;
			int z_x, z_y;

			// AlphaBetaFilter - Predict based on current position and velocity
			xk = xk_1 + (DELTA_T * vk_1_x);
			yk = yk_1 + (DELTA_T * vk_1_y);
			vk_x = vk_1_x;
			vk_y = vk_1_y;

			currentMeasurement->estimate.x = xk;
			currentMeasurement->estimate.y = yk;

			// AlphaBetaFilter - Greedy method to get least error point (Truth)
			vector<Point> pointsInGate;
			for (int x = xk - GATE_SIZE; x < xk + GATE_SIZE; x++) {
				for (int y = yk - GATE_SIZE; y < yk + GATE_SIZE; y++) {
					if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
						if (secondFrame.at<uchar>(y, x) == 1) pointsInGate.push_back(Point(x, y));
					}
				}
			}

			if (pointsInGate.empty()) { // If no points close by just make estimate same as current point
				z_x = xk_1;
				z_y = yk_1;
			}
			else {
				Point p = FindClosestPoint(pointsInGate, Point(xk, yk));
				z_x = p.x;
				z_y = p.y;
			}

			rk_x = z_x - xk;
			rk_y = z_y - yk;
			xk += (ALPHA * rk_x);
			yk += (ALPHA * rk_y);
			vk_x += (BETA * rk_x) / DELTA_T;
			vk_y += (BETA * rk_y) / DELTA_T;

			currentMeasurement->truth.x = xk;
			currentMeasurement->truth.y = yk;
			currentMeasurement->velocity.x = vk_x;
			currentMeasurement->velocity.y = vk_y;

			//cout << "Updated Velocity is (" << vk_x << ", " << vk_y << ")" << endl;

			// Store the velocity in the velocity memory
			memory[z_x][z_y] = currentMeasurement;
		}
	}
}







vector <vector <Vertex *>> get_measurements(vector <vector<Point2f>> &cent) {
	vector <vector <Vertex *>> measurements;
	vector <Vertex *> measurementsForSingleFrame;  // vector to store centroids for a certain frame

	//for (auto& i : measurements){
	//	for (auto& j : i){
	//		cout << j->position << endl;
	//	}
	//	cout << "COUNT" << count << endl;
	//	count += 1;
	//}
	for (int ii = 0; ii < cent.size(); ii++){
		measurementsForSingleFrame.clear();
		for (int jj = 0; jj < cent[ii].size(); jj++){
			Vertex *newVertex = new Vertex();
			newVertex->position = cent[ii][jj];
			
			newVertex->velocity = Point(V_INIT, V_INIT);
			newVertex->estimate = newVertex->position;
			newVertex->truth = newVertex->position;
			measurementsForSingleFrame.push_back(newVertex);
		}
		measurements.push_back(measurementsForSingleFrame);
	}

	//for (auto& i : measurements){
	//	for (auto& j : i){
	//		cout << j->position << endl;
	//	}
	//}
	//cout << measurements.size() << " kutti" << endl;
	return measurements;
}








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


