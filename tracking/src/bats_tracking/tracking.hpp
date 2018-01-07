//===================================================
//  tracking.hpp
//  Project :- CS 585 Assignment 4
//
//  Created by Shantanu Bobhate,Muhammad Kasim Patel and Muhammad Zuhayr Raghib on 10/24/17.
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

#ifndef tracking_hpp
#define tracking_hpp

#include <stdio.h>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>

#define FIRST_FILE_NUMBER 750
#define LAST_FILE_NUMBER 901
#define NUMBER_OF_FILES 150
#define MIN_DIST 100
// #define ALTERNATE    // Uncomment to debug weird behavior when limiting matching to one-to-one (errors)

#define DELTA_T 0.1
#define V_INIT 1
#define ALPHA 0.85
#define BETA 0.05

#define ROWS 1025
#define COLS 1025
#define GATE_SIZE 50

using namespace std;
using namespace cv;

/*
 * Variable: velocity_memory
 * --------------------
 *  used to store the current velocity that was updated based on the estimate, beta and delta_t during the ith frame for the succeeding point in the (i+1)th frame.
 *  the vector is indexed by a point using its coordinates (x, y) and the value at the location is a Point describing the previously computed velocity.
 *  in short, it is an efficient way for providing fast storing and lookup across iterations and allowing points in consecutive frames to communicate.
 */
static struct Vertex * memory[ROWS][COLS];


/*
 * Struct: Vertex
 * --------------------
 *  used to describe a node on our bipartite graph
 *
 *  members:
 *      parent      - a pointer to the node's parent node in a flow/track (the previous position of the bat)
 *      child       - a pointer to the node's child in a flow/track (the next position of the bat)
 *      distToChild - the distance between the node and its child (euclidean distance)
 *      position    - the (x,y) coordinate of the point in the image
 */
struct Vertex {
    struct Vertex *parent = NULL;
    struct Vertex *child = NULL;
    double distToChild = 0;
    Point position;
    Point estimate;
    Point truth;
    Point velocity;
    Scalar color;
};

/*
 * Function: InitializeVelocityMemory
 * --------------------
 *  initializes the 2D array where each Point whose coordinates are assigned the value of V_INIT
 *
 *  parameters: N/A
 *
 *  returns: N/A
 */
void InitializeVelocityMemory();

/*
 * Function: ReadData
 * --------------------
 *  reads the localization data of the bats from a text file and stores it in a nested structure
 *
 *  parameters:
 *      - measurements -> a nested structure that stores the centroids
 *                        for each bat for every frame in the dataset.
 *
 *  returns: N/A
 */
void ReadData(vector <vector <Vertex *>>& measurements);

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
void AlphaBetaFilter(vector <vector <Vertex *>>& measurements);

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
Point FindClosestPoint(vector<Point> points, Point point);

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
double EuclideanDistance(Point a, Point b);

#endif /* tracking_hpp */
