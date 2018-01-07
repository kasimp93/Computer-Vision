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

#include "fish.h"

// main function
int main()
{

	cout << ":hello";
	vector <vector<Point2f>> cent;
	vector <vector <Vertex *>> measurements;
	/*Fish tank*/
	//run_fish();
	//for (auto& i : cent){
	//	for (auto& j : i){
	//		cout << j.x << " kutti" << endl;
	//	}
	//}
	get_measurements(measurements,cent);

	//waitKey(0);
	return(0);

}