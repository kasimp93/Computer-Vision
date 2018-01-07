#include <iostream>
#include <vector>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include <opencv2\core\core.hpp>
#include <opencv2\imgcodecs\imgcodecs.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <fstream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#define MIN_DIST 100
// #define ALTERNATE    // Uncomment to debug weird behavior when limiting matching to one-to-one (errors)

#define DELTA_T 0.1
#define V_INIT 1
#define ALPHA 0.85
#define BETA 0.05

#define ROWS 1025
#define COLS 1025
#define GATE_SIZE 50



/*
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
*/





//void get_measurements(vector <vector <Vertex *>>& measurements, vector <vector<Point2f>> cent);

vector<Mat> readsegFiles();
vector<Mat> readFiles();
void run_fish();
