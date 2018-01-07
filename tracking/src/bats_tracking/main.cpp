//
//  main.cpp
//  Assignment 4
//
//  Created by Shantanu Bobhate on 10/21/17.
//  Copyright © 2017 Shantanu Bobhate. All rights reserved.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "tracking.hpp"

using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
    vector <vector <Vertex *>> measurements;
    
    // Retrieve information from files
    ReadData(measurements);
    
    AlphaBetaFilter(measurements);
    
    Mat image;
    Mat trackHistory = Mat::zeros(ROWS, COLS, CV_8UC3);
    namedWindow("Bat Motion Tracking", CV_WINDOW_AUTOSIZE);
    for (int ii = FIRST_FILE_NUMBER; ii < LAST_FILE_NUMBER; ii++) {
        image = imread("CS585-BatImages/Gray/CS585Bats-Gray_frame000000" + to_string(ii) + ".ppm");
    
        vector<Vertex *> measurementsForCurrentFrame = measurements.at(ii - FIRST_FILE_NUMBER);
        
        putText(image, "Red    : Current Point", Point(10, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
        putText(image, "Yellow : Next Estimated Point", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
        putText(image, "Green  : Corrected Estimated Point", Point(10, 45), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
        
        for (int kk = 0; kk < measurementsForCurrentFrame.size(); kk++) {
            Vertex *myVertex = measurementsForCurrentFrame[kk];
            circle(image, myVertex->position, 2, Scalar(0, 0, 255));
            circle(image, myVertex->estimate, 2, Scalar(0, 255, 255));
            circle(image, myVertex->truth, 2, Scalar(0, 255, 0));
            line(trackHistory, myVertex->position, myVertex->truth, myVertex->color);
        }

        imshow("Bat Motion Tracking", image);
        imshow("Tracks", trackHistory);
        waitKey(30);
    }
    
    imshow("Tracks", trackHistory);
    waitKey();
    
    return 0;
}



