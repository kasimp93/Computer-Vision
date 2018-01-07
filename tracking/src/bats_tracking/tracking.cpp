//===================================================
//  tracking.hpp
//  Project :- CS 585 Assignment 4
//
//  Created by Shantanu Bobhate, Muhammad Kasim Patel and Muhammad Zuhayr Raghib on 10/24/17.
//  Copyright © 2017 Shantanu Bobhate. All rights reserved.
//
//  Purpose:
//      - Perform object tracking on a dataset of bats, given segmentation and localization data,
//      - Contains functions used to perform above mentioned task.
//
//  Implementation Details:
//      - Store localization data for all bats in all frames as a nested vector of Vertices.
//      - First approach uses a method that considers localization data for 2 consecutive frames and performs
//          a bipartite matching algorithm to make data associations to identify TRUE object tracks.
//      - Using the found data association, an Alpha-Beta filter is applied to the date to get estimates
//          for the positions of the bats before observing the TRUE measurement.
//===================================================
#include "tracking.hpp"

/*
 * Function: InitializeVelocityMemory
 * --------------------
 *  initializes the 2D array where each Point whose coordinates are assigned the value of V_INIT
 *
 *  parameters: N/A
 *
 *  returns: N/A
 */
void InitializeVelocityMemory() {
    for (int ii = 0; ii < ROWS; ii++) {
        for (int jj = 0; jj < COLS; jj++) {
            memory[ii][jj] = NULL;
        }
    }
}

/*
 * Function: ReadData
 * --------------------
 *  reads the localization data of the bats from a text file and stores it in a nested structure
 *
 *  parameters:
 *      - measurements -> a nested structure that stores the vertices
 *                        for each bat for every frame in the dataset.
 *
 *  returns: N/A
 */
void ReadData(vector <vector <Vertex *>>& measurements) {
    // variables
    ifstream ifs;
    string line, file;
    int x, y;
    
    vector <Vertex *> measurementsForSingleFrame;  // vector to store centroids for a certain frame
    
    for (int ii = FIRST_FILE_NUMBER; ii < LAST_FILE_NUMBER; ii++) {
        file = "Localization/CS585Bats-Localization_frame000000" + to_string(ii) + ".txt";
        ifs.open(file);
        
        measurementsForSingleFrame.clear();
        while (getline(ifs, line)) {
            stringstream ss(line);
            ss >> x;
            ss.ignore();
            ss >> y;
            
            Vertex *newVertex = new Vertex();
            newVertex->position = Point(x, y);
            newVertex->velocity = Point(V_INIT, V_INIT);
            newVertex->estimate = newVertex->position;
            newVertex->truth = newVertex->position;
            if (ii == FIRST_FILE_NUMBER) {
                newVertex->color = Scalar(rand() % 256, rand() % 256, rand() % 256);
            }
            measurementsForSingleFrame.push_back(newVertex);
        }
        measurements.push_back(measurementsForSingleFrame);
        
        ifs.close();
    }
}

/*
 * Function: AlphaBetaFilter
 * --------------------
 *  uses an alpha-beta filter to predict the position in the next frame of a datapoint in the current frame
 *
 *  parameters:
 *      - measurements -> a nested structure that stores the centroids
 *                        for each bat for every frame in the dataset.
 *
 *  returns: N/A
 */
void AlphaBetaFilter(vector <vector <Vertex *>>& measurements) {
    // Initialize the velocity memory structure
    InitializeVelocityMemory();
    
    // Loop for every frame
    for (int ii = 0; ii < measurements.size(); ii++) {
        Mat secondFrame = Mat::zeros(ROWS, COLS, CV_8UC1);
        for (int jj = 0; jj < measurements[ii+1].size(); jj++) {
            secondFrame.at<uchar>(measurements[ii+1][jj]->position.y, measurements[ii+1][jj]->position.x) = 1;
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
            } else {
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
            } else {
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

/*
 * Function: FindClosestPoint
 * --------------------
 *  finds the point closest to the given point in the given region of interest
 *
 *  parameters:
 *      - points -> a structure that holds the points available in the given area
 *
 *  returns: closest point
 */
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

/*
 * Function: EuclideanDistance
 * --------------------
 *  finds the euclidean distance between 2 points
 *
 *  parameters:
 *      - a -> the point in the first frame
 *      - b -> the point in the second frame
 *
 *  returns: the euclidean distance between the 2 points a and b
 */
double EuclideanDistance(Point a, Point b) {
    return sqrt(pow(abs(a.x - b.x), 2) + pow(abs(a.y - b.y), 2));
}
