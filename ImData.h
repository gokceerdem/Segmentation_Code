#include <vector>
#include <list>
#include "coor.h"
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

struct ImData
{
	Mat original; //original image
	Mat intensity;	  //grayscale image
	Mat hsvImg;	  // 3 channel hsv img
	
	vector<Mat> filter;	//filter outputs
	vector<Mat> hsv_filter; // used for splitting hsv channels
	list<coor> hsv_col_info; // HSV color quantization information of the image
								// keeps coordinates and color info together
	vector<vector<Point>> connComp;
	Mat hsv_label; // 1-22 aras� de�erlerin piksellere atand��� image
	int h;	//height
	int w;	//width
};
