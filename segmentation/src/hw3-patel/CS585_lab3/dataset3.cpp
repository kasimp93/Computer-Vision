//
// dataset3.cpp --> code for dataset3
//  
//
//  Created by Muhammad Zuhayr Raghib on 9/30/17.
//
//

#include "dataset3.h"




/*function to execute all relevant functions on the dataset*/

void run_dataset_3(){
	// input images
	vector<Mat> dataset3 = readFiles(); 
	// generate background image
	Mat background_img_gray = background(dataset3); 
	int people = 0;

	/*LOOP THROUGH ALL IMAGES*/
	for (int i = 0; i < dataset3.size() ; i++){
	//for (int i = 150; i < dataset3.size(); i++){
		people = process(dataset3[i], background_img_gray);
		cout << "Number of people in image " << i + 110 << "  =  " << people << endl;
		waitKey(50);
	}
	waitKey(0);
	destroyAllWindows(); // destroy windows before running next function
}


/*Read in all images from folder*/
vector<Mat> readFiles(){
	vector<string> files;
	vector<Mat> images;
	string v, vv, vvv;

	for (int i = 10; i <= 99; i++){
		v = "Z:/CS585-PeopleImages/frame_00";
		vv = to_string(i);
		vvv = v + vv + ".jpg";
		files.push_back(vvv);
		Mat src = imread(vvv, 1);
		images.push_back(src);
		waitKey(5);
	}
	for (int i = 100; i <= 160; i++){
		v = "Z:/CS585-PeopleImages/frame_0";
		vv = to_string(i);
		vvv = v + vv + ".jpg";
		files.push_back(vvv);
		Mat src = imread(vvv, 1);
		images.push_back(src);
		waitKey(5);
	}
	return images;
}



/*Calculate a background mean image*/

Mat background(vector<Mat>& dataset){
	Mat first = dataset[0].clone();  //to save datatype

	Mat temp;

	Mat result = Mat::zeros(first.rows, first.cols, CV_64FC3); //used 64 bit to avoid overflow

	for (int i = 0; i < dataset.size(); i++){
		dataset[i].convertTo(temp, CV_64FC3); // convert to same size as sum
		result = result + temp;
	}

	result = result / dataset.size();
	result.convertTo(result, first.type());
	GaussianBlur(result, result, Size(3, 3), 0, 0); // apply gaussian filter to smoothen image

	Mat background_img_gray; //backgroung gray

	cvtColor(result, background_img_gray, CV_BGR2GRAY);

	return background_img_gray;

}


/*Perform image processing on a single image to count number of people*/
int process(Mat& src, Mat& background_img_gray){

	Mat src_copy = src.clone(); // copy of original image for displaying the contours

	Mat gray; //src gray

	cvtColor(src, gray, CV_BGR2GRAY); //convert to grayscale
	GaussianBlur(gray, gray, Size(3, 3), 0, 0); // apply gaussian filter to smoothen image
	//imshow("gray src", gray);

	Mat img3; //abs diff
	absdiff(gray, background_img_gray, img3);
	//imshow("diff", img3);

	//processing - dilute and erode etc.
	Mat eroded = img3; 

	/*for slice 2*/
	Mat thres_output; // thresholded
	threshold(img3, thres_output, 90, 255, THRESH_BINARY);

	int erosion_size = 1;
	int dilation_size = 3;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3), Point(erosion_size, erosion_size));

	erode(thres_output, thres_output, element);
	dilate(thres_output, thres_output, element);
	//imshow("thres_output_slice 1", thres_output);

	/*for slice 1 and 3*/
	Mat thres_output2; // thresholded
	threshold(img3, thres_output2, 80, 255, THRESH_BINARY);
	//imshow("thres_output_slice 2", thres_output2);
	dilate(thres_output2, thres_output2, element);

	/*for slice 4*/
	Mat thres_output3; // thresholded
	threshold(img3, thres_output3, 90, 255, THRESH_BINARY);
	erode(thres_output3, thres_output3, element);
	dilate(thres_output3, thres_output3, element);
	//imshow("thres_output_slice 2", thres_output2);

	//Each slice is processed differently
	Mat element2 = getStructuringElement(MORPH_RECT, Size(5, 5), Point(erosion_size, erosion_size)); // larger size than before
	
	/*slice 1*/
	Mat slice1 = Mat(thres_output2, Rect(0, 0, 100, thres_output.rows)).clone();
	dilate(slice1, slice1, element2);
	//imshow("slice1", slice1);

	/*slice 2*/
	Mat slice2 = Mat(thres_output, Rect(100, 0, 320, thres_output.rows)).clone();

	dilate(slice2, slice2, element2);
	dilate(slice2, slice2, element2);
	dilate(slice2, slice2, element2);
	//imshow("slice2 ", slice2);

	/*slice 3*/
	Mat slice3 = Mat(thres_output3, Rect(420, 0, 230, thres_output.rows)).clone();
	erode(slice3, slice3, element2);
	dilate(slice3, slice3, element2);
	dilate(slice3, slice3, element2);
	erode(slice3, slice3, element2);
	erode(slice3, slice3, element2);
	//imshow("slice3 ", slice3);

	/*slice 4*/
	Mat slice4 = Mat(thres_output2, Rect(650, 0, 118, thres_output.rows)).clone();
	erode(slice4, slice4, element2);
	//imshow("slice4 ", slice4);


	// Concatenate the sliced images to form a single image for output
	Mat image1, image2, image3;
	hconcat(slice1, slice2, image1);
	hconcat(slice3, slice4, image2);
	hconcat(image1, image2, image3);


	//imshow("image3 ", image3);
	dilate(image3, image3, element);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	//only find the outer contours
	findContours(image3, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));


	Mat contour_output = Mat::zeros(image3.size(), CV_8UC3);

	Rect boundrec;

	//print contours
	int p = 0;
	if (contours.size() > 0){
		for (int i = 0; i < contours.size(); i++){

			/*for slice 4*/
			Point coordinate_i_ofcontour = contours[i].size(); //get coordinates of the contour
			if (coordinate_i_ofcontour.x > 650){ //slice 4 starts at col 650
				drawContours(src_copy, contours, i, Scalar(0, 0, 255), 2, 8, hierarchy);
				boundrec = boundingRect(contours[i]);

				rectangle(src_copy, boundrec, Scalar(0, 255, 0), 1, 8, 0);
				p += 1;

			}

			/*for all other slices*/
			else{
				if (contourArea(contours[i]) > 90){

					drawContours(src_copy, contours, i, Scalar(0, 0, 255), 2, 8, hierarchy);
					boundrec = boundingRect(contours[i]);

					rectangle(src_copy, boundrec, Scalar(0, 255, 0), 1, 8, 0);
					p += 1;
				}
			}
		}
	}
	//cout << "The number of ppl detected is: " << p << endl;

	// Show in a window
	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", src_copy);
	
	return p;

}



