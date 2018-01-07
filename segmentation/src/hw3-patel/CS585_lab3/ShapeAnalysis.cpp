//
//  ShapeAnalysis.cpp
//  
//
//  Created by Shantanu Bobhate on 9/26/17.
//
//

#include "ShapeAnalysis.h"

/*
 * An array of colors to be used to color different components;
 */
static Vec3b labelColors[25] = { Vec3b(0, 255, 0), Vec3b(0, 0, 255), Vec3b(255, 255, 0), Vec3b(255, 0, 255), Vec3b(0, 255, 255),
					Vec3b(55, 0, 0), Vec3b(0, 55, 0), Vec3b(0, 0, 55), Vec3b(55, 55, 0), Vec3b(55, 0, 55), Vec3b(0, 55, 55), Vec3b(55, 55, 55), 
					Vec3b(255, 55, 0), Vec3b(55, 255), Vec3b(255, 0, 55), Vec3b(55, 0, 255), Vec3b(0, 255, 55), Vec3b(0, 55, 255), Vec3b(255, 55, 55), 
					Vec3b(255, 255, 55), Vec3b(55, 255, 55), Vec3b(55, 55, 255), Vec3b(255, 55, 225), Vec3b(55, 255, 255) };

/*
 * Struct to contain information about the Connected Components
 */
struct ConnectedComponent {
	int label = -1;
	int area = 0;
    Point startPoint = Point(0,0);
	int perimeter = 0;
    Point centroid = Point(0,0);
	double orientation = 0;
	double circularity = 0;
	double compactness = 0;
};

/*
 * Function to find and label Connected Components
 */
void ConnectedComponentLabelling(Mat& srcImage, Mat& dstImage)
{
	int label = 1;
	vector<Point> stack;
	int componentCount = 0;
	imshow("Original", srcImage);
    
    vector<ConnectedComponent> components;

	int y = 0, x = 0;
	int y_lim = dstImage.rows, x_lim = dstImage.cols;

	while (x != x_lim - 1 || y != y_lim - 1) {
		uchar pixel = srcImage.at<uchar>(y, x);
		
		if (pixel == 0)   // if the pixel is black
		{
			// Declare a new component object with current label
			ConnectedComponent component;
			component.label = label;

			srcImage.at<uchar>(y, x) = label;	// Change value to current label
            Point startPoint = Point(x, y);
			stack.push_back(startPoint);       // Push pixel onto the stack
			// Search for connected neighbours
			// ... Continue searching until no more valid neighbours are found
			while (stack.size() != 0) {
				Point currentPixel = (Point) stack.back();                  // Get last added neighbour (DFS)
                stack.pop_back();                                           // Remove point from stack1
				component.area++;                                           // Count this pixel in the component's area
				PutOnStackNeighbours(srcImage, stack, currentPixel, label);	// Add this pixels neighbours to the stack
			}
            component.startPoint.x = startPoint.x; component.startPoint.y = startPoint.y;

			// Completed component
            if (component.area < 500) {
                for (int y = 0; y < srcImage.rows; y++) {
                    for (int x = 0; x < srcImage.cols; x++) {
                        if (srcImage.at<uchar>(y, x) == label) {
                            srcImage.at<uchar>(y, x) = 255;
                            dstImage.at<Vec3b>(y, x) = Vec3b(255, 255, 255);
                        }
                    }
                }
            } else {
                for (int y = 0; y < srcImage.rows; y++) {
                    for (int x = 0; x < srcImage.cols; x++) {
                        if (srcImage.at<uchar>(y, x) == label) {
                            dstImage.at<Vec3b>(y, x) = (Vec3b) labelColors[label-1];
                        }
                    }
                }
                
                components.push_back(component);
                componentCount++;
                // Change label since next object is not connected
                label++;
            }
		}

		// Image iteration
		if (x == x_lim - 1 && y != y_lim - 1)
		{
			x = 0;
			y++;
		}
		else 
		{
			x++;
		}
	}

	cout << "Done Labelling Connected Components " << componentCount << " found" << endl;
    putText(dstImage, to_string(componentCount) + " found", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,0));
	imshow("Processed", dstImage);
    imwrite("output" + to_string(label) + ".png", dstImage);
	waitKey();
    
    destroyAllWindows();
    
    for (ConnectedComponent cc : components) {
        Mat ColorImage = Mat(srcImage.rows, srcImage.cols, CV_8UC3, Scalar(255,255,255));
        cc.perimeter = BoundaryFollowing(srcImage, dstImage, cc.startPoint);
        Point centroid = FindCentroid(srcImage, dstImage, cc.label, cc.area);
        cc.centroid.x = centroid.x; cc.centroid.y = centroid.y;
        cc.orientation = FindOrientation(srcImage, dstImage, cc.label, cc.centroid.x, cc.centroid.y);
        cc.circularity = FindCircularity(srcImage, cc.label, cc.area, cc.centroid.x, cc.centroid.y);
        cc.compactness = FindCompactness(cc.perimeter, cc.area);
        cout << "Component label       : " << cc.label << endl
             << "Component area        : " << cc.area << endl
             << "Component perimeter   : " << cc.perimeter << endl
             << "Component centroid    : " << "("<< cc.centroid.x << ", " << cc.centroid.y << ")" << endl
             << "Component orientation : " << cc.orientation << " degrees wrt the x-axis" << endl
             << "Component circularity : " << cc.circularity << endl
             << "Component compactness : " << cc.compactness << endl
             << endl;
        
        Mat skeleton = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);
        MedialAxisTransform(srcImage, skeleton, cc.label);
        imshow("Skeleton", skeleton);
        imwrite("skeleton" + to_string(cc.label) + ".png", skeleton);
        waitKey();
    }
    
    imshow("Final", dstImage);
    waitKey();
}


/*
 * Function to check and add valid neighbours onto the stack
 */
void PutOnStackNeighbours(Mat& srcImage, vector<Point>& stack, Point currentPixel, int& label)
{
    int currentX, currentY;
    
	currentX = currentPixel.x - 1; currentY = currentPixel.y - 1;
	LabelPixel(srcImage, stack, currentX, currentY, label);
    
	currentX = currentPixel.x; currentY = currentPixel.y - 1;
	LabelPixel(srcImage, stack, currentX, currentY, label);
    
	currentX = currentPixel.x + 1; currentY = currentPixel.y - 1;
	LabelPixel(srcImage, stack, currentX, currentY, label);
    
	currentX = currentPixel.x + 1; currentY = currentPixel.y;
	LabelPixel(srcImage, stack, currentX, currentY, label);
    
	currentX = currentPixel.x + 1; currentY = currentPixel.y + 1;
	LabelPixel(srcImage, stack, currentX, currentY, label);
    
	currentX = currentPixel.x; currentY = currentPixel.y + 1;
	LabelPixel(srcImage, stack, currentX, currentY, label);
    
	currentX = currentPixel.x - 1; currentY = currentPixel.y + 1;
	LabelPixel(srcImage, stack, currentX, currentY, label);
    
	currentX = currentPixel.x - 1; currentY = currentPixel.y;
	LabelPixel(srcImage, stack, currentX, currentY, label);
}

/*
 * Function validate a pixel and change its label
 */
void LabelPixel(Mat& srcImage, vector<Point>& stack, int currentX, int currentY, int& label)
{
    // Make sure the neighbour is inside the image frame
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        // Is the neighbour of interest?
        if (srcImage.at<uchar>(currentY, currentX) == 0)
        {
			srcImage.at<uchar>(currentY, currentX) = label; // Set label
            stack.push_back(Point(currentX, currentY));     // Add pixel to the stack
        }
    }
}

/*
 * Algorithm to find the boundary a component
 */
int BoundaryFollowing(Mat& srcImage, Mat& dstImage, Point P_start) {
    int label = srcImage.at<uchar>(P_start.y, P_start.x);
    // ConnectedComponent comp = (ConnectedComponent) components.at(label-1);
    cout << "found a pixel which is " << P_start.x << ", " << P_start.y << endl;
    
    int perimeter = 1;
    
    dstImage.at<Vec3b>(P_start.y, P_start.x) = Vec3b(0,0,255);
    Point b = Point(P_start.x-1, P_start.y);
    Point bPrev = Point(0,0);
    while (srcImage.at<uchar>(b.y, b.x) != label) {
        bPrev.x = b.x; bPrev.y = b.y;
        WhereToGoNext(P_start, b);
    } // Breaks when finds next pixel on boundary in clockwise direction
    Point P = Point(0,0);
    P.x = b.x; P.y = b.y;
    perimeter++;
    b.x = bPrev.x; b.y = bPrev.y;
    if (P.x >= 0 && P.x < srcImage.cols && P.y >= 0 && P.y < srcImage.rows)
        dstImage.at<Vec3b>(P.y, P.x) = Vec3b(255,0,0);
    imshow("Boundary", dstImage);
    waitKey(30);
    
    int startVisits = 0;
    while (startVisits < 2) {
        while (srcImage.at<uchar>(b.y, b.x) != label) {
            bPrev.x = b.x; bPrev.y = b.y;
            WhereToGoNext(P, b);
        } // Breaks when finds next pixel on boundary in clockwise direction
        P.x = b.x; P.y = b.y;
        perimeter++;
        b.x = bPrev.x; b.y = bPrev.y;
        cout << P.x << ", " << P.y << endl;
        if (P.x >= 0 && P.x < srcImage.cols && P.y >= 0 && P.y < srcImage.rows)
            dstImage.at<Vec3b>(P.y, P.x) = Vec3b(255,0,0);
        
        if (P.x == P_start.x && P.y == P_start.y) startVisits++;
    }
    
    cout << "Boundaries found" << endl;
    
    imshow("Boundary", dstImage);
    waitKey();
    
    return perimeter;
}

/*
 * Function to help figure out where to go next
 *  in the boundary tracking algorithm
 */
void WhereToGoNext(Point& P, Point& b) {
    int x_diff = P.x - b.x, y_diff = P.y - b.y;
    
    //cout << b.x << ", " << b.y << endl;
    
    if (x_diff > 0 && y_diff == 0) {        // CASE b left of P
        b.y--;
        //cout << "Go Up" << endl;
    } else if (x_diff > 0 && y_diff > 0) {  // Case b top-left of P
        b.x++;
        //cout << "Go Right" << endl;
    } else if (x_diff == 0 && y_diff > 0) { // Case b top of P
        b.x++;
        //cout << "Go Right" << endl;
    } else if (x_diff < 0 && y_diff > 0) {  // Case b top-right of P
        b.y++;
        //cout << "Go Down" << endl;
    } else if (x_diff < 0 && y_diff == 0) { // Case b right of P
        b.y++;
        //cout << "Go Down" << endl;
    } else if (x_diff < 0 && y_diff < 0) {  // Case b bottom-right of P
        b.x--;
        //cout << "Go Left" << endl;
    } else if (x_diff == 0 && y_diff < 0) { // Case b bottom of P
        b.x--;
        //cout << "Go Left" << endl;
    } else if (x_diff > 0 && y_diff < 0) {  // Case b bottom-left of P
        b.y--;
        //cout << "Go Up" << endl;
    } else {
        cout << "Where the fuck is b?" << endl;
    }
}

Point FindCentroid(Mat& srcImage, Mat& dstImage, int label, int area) {
    int sum_x = 0, sum_y = 0;
    for (int y = 0; y < srcImage.rows; y++)
    {
        for (int x = 0; x < srcImage.cols; x++)
        {
            if (srcImage.at<uchar>(y, x) == label) {
                sum_x += x;
                sum_y += y;
            }
        }
    }
    int x_mean = (int) sum_x / area;
    int y_mean = (int) sum_y / area;
    
    circle(dstImage, Point(x_mean, y_mean), 3, Scalar(0,0,0));
    
    return Point(x_mean, y_mean);
}

/*
 * Function to find the orientation of the an object and draw the axis of least inertia
 */
double FindOrientation(Mat& srcImage, Mat& dstImage, int label, int x_mean, int y_mean) {
    double mew_11 = 0, mew_20 = 0, mew_02 = 0;
    for (int y = 0; y < srcImage.rows; y++)
    {
        for (int x = 0; x < srcImage.cols; x++)
        {
            if (srcImage.at<uchar>(y, x) == label) {
                int diff_x = x - x_mean;
                int diff_y = y - y_mean;
                mew_20 += (diff_x*diff_x);
                mew_11 += (diff_x*diff_y);
                mew_02 += (diff_y*diff_y);
            }
        }
    }
    
    double theta = 0.5*atan((2*mew_11)/(mew_20 - mew_02));
    
    double g = -(x_mean*sin(theta)) + (y_mean*cos(theta));
    int x_new1 = 0;
    int y_new1 = (g + (x_new1*sin(theta))) / cos(theta);
    int x_new2 = srcImage.cols;
    int y_new2 = (g + (x_new2*sin(theta))) / cos(theta);
    
    line(dstImage, Point(x_new1, y_new1), Point(x_new2, y_new2), Scalar(0, 0, 0));
    
    
    return theta;
}

/*
 * Function to find the Emin/Emax ratio for an object
 */
double FindCircularity(Mat& srcImage, int label, int area, int x_mean, int y_mean) {
    int a = 0, b = 0, c = 0;
    for (int y = 0; y < srcImage.rows; y++)
    {
        for (int x = 0; x < srcImage.cols; x++)
        {
            if (srcImage.at<uchar>(y, x) == label) {
                int diff_x = x - x_mean;
                int diff_y = y - y_mean;
                a += (diff_x*diff_x);
                b += (diff_x*diff_y);
                c += (diff_y*diff_y);
            }
        }
    }
    
    b = 2*b;
    
    double Emin = ((a+c)/2) - (((a+c)/2)*((a-c)/sqrt(pow(a-c,2)+pow(b,2)))) - ((b/2)*(b/(sqrt(pow(a-c,2)+pow(b,2)))));
    double Emax = ((a+c)/2) - (((a+c)/2)*(-(a-c)/sqrt(pow(a-c,2)+pow(b,2)))) - ((b/2)*(-b/(sqrt(pow(a-c,2)+pow(b,2)))));
    
    double circularity = Emin/Emax;

    return circularity;
}

/*
 * Function to find the compactness of an object
 */
double FindCompactness(int perimeter, int area) {
    return (perimeter*perimeter)/(4*3.14159265358979323846*area);
}

/*
 * Function to compute the Medial Axis Transform of an object (Skeleton)
 */
void MedialAxisTransform(Mat& srcImage, Mat& dstImage, int label) {
    // STEP 1: Set all background pixels to 0 and object pixels to 255 (~inf)
    for (int y = 0; y < srcImage.rows; y++) {
        for (int x = 0; x < srcImage.cols; x++) {
            if (srcImage.at<uchar>(y, x) == label)
                dstImage.at<uchar>(y, x) = 255;
        }
    }
    
    // STEP 2: Scan image from left to right and top to bottom
    for (int y = 0; y < srcImage.rows; y++) {
        for (int x = 0; x < srcImage.cols; x++) {
            Point me = Point(x, y);
            int myVal = FindMyNewValIter1(dstImage, me);
            dstImage.at<uchar>(y, x) = myVal;
        }
    }
    
    // STEP 2: Scan image from right to left and bottom to top
    for (int y = srcImage.rows-1; y > -1; y--) {
        for (int x = srcImage.cols-1; x > -1; x--) {
            Point me = Point(x, y);
            int myVal = FindMyNewValIter2(dstImage, me);
            dstImage.at<uchar>(y, x) = myVal;
        }
    }
}

/*
 * Helper function for Medial Axis Transform
 */
int FindMyNewValIter1(Mat& srcImage, Point currentPixel)
{
    int currentX, currentY;
    int minVal = 255;
    
    currentX = currentPixel.x; currentY = currentPixel.y;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val < minVal) minVal = val;
    }
    
    currentX = currentPixel.x - 1; currentY = currentPixel.y - 1;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val+2 < minVal) minVal = val + 2;
    }
    
    currentX = currentPixel.x; currentY = currentPixel.y - 1;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val+1 < minVal) minVal = val + 1;
    }
    
    currentX = currentPixel.x + 1; currentY = currentPixel.y - 1;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val+2 < minVal) minVal = val + 2;
    }
    
    currentX = currentPixel.x - 1; currentY = currentPixel.y;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val+1 < minVal) minVal = val + 1;
    }
 
    return minVal;
}

/*
 * Helper function for Medial Axis Transform
 */
int FindMyNewValIter2(Mat& srcImage, Point currentPixel)
{
    int currentX, currentY;
    int minVal = 255;
    
    currentX = currentPixel.x; currentY = currentPixel.y;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val < minVal) minVal = val;
    }
    
    currentX = currentPixel.x + 1 ; currentY = currentPixel.y;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val+1 < minVal) minVal = val + 1;
    }
    
    currentX = currentPixel.x - 1; currentY = currentPixel.y + 1;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val+2 < minVal) minVal = val + 2;
    }
    
    currentX = currentPixel.x; currentY = currentPixel.y + 1;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val+1 < minVal) minVal = val + 1;
    }
    
    currentX = currentPixel.x + 1; currentY = currentPixel.y + 1;
    if (currentX >= 0 && currentY >= 0 && currentX < srcImage.cols && currentY < srcImage.rows)
    {
        int val = srcImage.at<uchar>(currentY, currentX);
        if (val+2 < minVal) minVal = val + 2;
    }
    
    return minVal;
}









