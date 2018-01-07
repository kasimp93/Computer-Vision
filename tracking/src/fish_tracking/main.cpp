#include "fish.h"

// main function
int main()
{
	/*Fish tank*/
	vector <vector<Point2f>> cent = run_fish();
	vector <vector <Vertex *>> measurements = get_measurements(cent);
	
	AlphaBetaFilter1(measurements);

	Mat image;
	Mat trackHistory = Mat::zeros(ROWS, COLS, CV_8UC3);
	namedWindow("Fish Tracking", CV_WINDOW_AUTOSIZE);
	
	for (int ii = 10; ii <= 53; ii++){
		image = imread("Z:/fish_tank_seg/2014_aq_segmented " + to_string(ii) + ".jpg");
		//imshow("Fish Tracking", image);
		vector<Vertex *> measurementsForCurrentFrame = measurements[ii - 10];

		int count = 0;

		//for (auto& i : measurements){
		//	for (auto& j : i){
		//		cout << j->position << endl;
		//	}
		//	cout << "COUNT" <<count << endl;
		//	count += 1;
		//}
		putText(image, "Red    : Current Point", Point(10, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
		putText(image, "Yellow : Next Estimated Point", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255));
		putText(image, "Green  : Corrected Estimated Point", Point(10, 45), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
		cout << measurementsForCurrentFrame.size() << endl;
		for (int kk = 0; kk < measurementsForCurrentFrame.size(); kk++) {
			
			Vertex *myVertex = measurementsForCurrentFrame[kk];
			circle(image, myVertex->position, 2, Scalar(0, 0, 255));
			circle(image, myVertex->estimate, 2, Scalar(0, 255, 255));
			circle(image, myVertex->truth, 2, Scalar(0, 255, 0));
			line(trackHistory, myVertex->position, myVertex->truth, myVertex->color);



			imshow("Tracks", trackHistory);
			

		}		
		imshow("Bat Motion Tracking", image);
		imshow("Tracks", trackHistory);
		waitKey(0);                   

	}

	//imshow("Tracks", trackHistory);
	waitKey(0);
	return 0;
}




/*

int main(){

int kutti =0 ;


if(kutti == 0){
run kasim main functuion
}

else if{
run shantanui code







*/