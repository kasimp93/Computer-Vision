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



#define MIN_DIST 15
// #define ALTERNATE    // Uncomment to debug weird behavior when limiting matching to one-to-one (errors)


#define ROWS 450
#define COLS 451
#define DELTA_T 0.4
#define V_INIT 0.01
#define ALPHA 0.85
#define BETA 0.05
#define GATE_SIZE 75

using namespace cv;
using namespace std;


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

static struct Vertex * memory[ROWS][COLS];


vector<Mat> readsegFiles();
vector<Mat> readFiles();
vector <vector<Point2f>> run_fish();
vector <vector <Vertex *>> get_measurements(vector <vector<Point2f>> &cent);
Point FindClosestPoint(vector<Point> points, Point point);
double EuclideanDistance(Point a, Point b);
void InitializeVelocityMemory();
void AlphaBetaFilter1(vector <vector <Vertex *>>& measurements);


