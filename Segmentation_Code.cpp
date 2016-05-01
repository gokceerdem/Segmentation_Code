// MyProject_1.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <stdio.h>    
#include "HEADER.h"

using namespace std;
using namespace cv;

int labelColors[10000][2]={0};
int myClrs[TOTAL_COLOR_Nr+1][3];
int dummy[10000][3];

vector<Point> contourArray;
vector<vector<Point>> imageContours;
vector<int> segmentClrs;

list<hsvc> col_hash_map;
coor c;

ImData myImData;
time_t tstart, tend, mstart, mend;


stringstream imgstream,mystream,seq_stream,imgstr;
	/*
	//Dynamic array
	int	**I = matrix(myImData.w,myImData.h);
	int	**label = matrix(myImData.w,myImData.h);
	*/
	
	//Static array
	int label[pyrHeight][pyrWidth]={0};
	int I[pyrHeight][pyrWidth]={0};
	int Q[pyrHeight][pyrWidth+1]={0};
	

	
	int EQ[MAX_EXP_NrOf_LABELS]={0};
	int relationLUT[TOTAL_COLOR_Nr+1][TOTAL_COLOR_Nr+1]={0};
	int labelNr;
  

int **matrix(int matrix_width, int matrix_height)//Dynamic array creating
{
   int** ary = new int*[ matrix_height];
  for(int i = 0; i <  matrix_height; ++i){
	  ary[i] = new int[matrix_width];}
 
  // fill
  for(int i = 0; i < matrix_height; ++i){
    for(int j = 0; j < matrix_width; ++j){
		ary[i][j] = 0;}}

   return ary;
}

void LUT(int relationLUT[TOTAL_COLOR_Nr+1][TOTAL_COLOR_Nr+1]){	
	

		for( int t = 0; t <TOTAL_COLOR_Nr+1; t++){
			relationLUT[t][t] = 1;}
		relationLUT[20][18]=1;
		relationLUT[19][20]=1;
		relationLUT[20][19]=1;
		relationLUT[20][16]=1;
		/*relationLUT[16][18]=1;*/
		relationLUT[17][21]=1;
		relationLUT[17][22]=1;
		relationLUT[21][22]=1;
		relationLUT[21][17]=1;
		relationLUT[22][21]=1;
		relationLUT[22][17]=1;
		relationLUT[1][14]=1;
		relationLUT[14][15]=1;
		relationLUT[2][3]=1;
		relationLUT[3][2]=1;

		//relationLUT[24][25]=1;//mx white & white
		//relationLUT[25][24]=1;//white & mx white 
		//relationLUT[23][22]=1;//black & mx black
		//relationLUT[22][23]=1;
		//relationLUT[23][18]=1;//black & dark gray
		//relationLUT[18][23]=1;
		//relationLUT[18][22]=1;//dark gray & mx black
		//relationLUT[22][18]=1;
		//relationLUT[24][19]=1;//mx white & light gray 
		//relationLUT[8][9]=1; // light blue & dark blue  
		//relationLUT[21][3]=1;//yellow & brown
		//relationLUT[3][20]=1;
		
}

void myColors(){int twentyfivecolors[TOTAL_COLOR_Nr+1][3]={
		{33,33,33},
		{255,0,0},
		{255,128,0},
		{255,255,0},
		{219,253,0},
		{102,255,102},
		{0,51,0},
		{0,255,255},
		{153,204,255},
		{0,0,204},
		{153,51,255},
		{76,0,153},
		{204,0,204},
		{255,0,255},
		{255,70,255},
		{255,204,255},
		{64,64,64},
		{192,192,192},
		{102,51,0},
		{0,0,0},
		{47,42,42},
		{255,255,255},
		{240,240,240},
	//int twentyfivecolors[TOTAL_COLOR_Nr+1][3]={
	//	{30,30,30},
	//	{255,0,0},//red				1
	//	{255,128,0},//orange		2
	//	{255,255,0},//yellow		3
	//	{219,253,0},//yellowgreen	4
	//	{102,255,102},//lightgreen	5
	//	{0,51,0},//darkgreen		6
	//	{0,255,255},//cyan			7
	//	{153,204,255},//lightblue	8
	//	{0,0,204},//darkblue		9
	//	{153,51,255},//violet		10
	//	{76,0,153},//purple			11
	//	{149,0,104},//pinkpurple	12
	//	{255,122,102},//pink1(yavruagzi) 13
	//	{199,40,63},//pink2(cherry)	14
	//	{255,204,255},//pink3(lightpink) 15
	//	{255,0,255},//magenta		16
	//	{255,70,255},//pink			17
	//	{64,64,64},//darkgray		18
	//	{192,192,192},//lightgray	19
	//	{58,25,25},//brown1(taba)	20
	//	{45,21,0},//brown			21
	//	{47,42,42},//mixedblack		22
	//	{0,0,0},//black				23
	//	{240,240,240},//mixedwhite	24
	//	{255,255,255},//white		25
	//
};

	for(int i = 0; i<TOTAL_COLOR_Nr+1; i++){
	for(int j = 0; j<3; j++){
		
		myClrs[i][j] = twentyfivecolors[i][j];

	}}
	
}

void colorImg(vector<Mat> channels, Mat ch0, Mat ch1, Mat ch2, int I[pyrHeight][pyrWidth]){
	channels.clear();
	
		 myColors();
		
 for ( int colorindex = 1; colorindex<TOTAL_COLOR_Nr+1; colorindex++)
 {
			 for(int i = 0; i < myImData.h; i++) {
		for(int j = 0; j < myImData.w; j++) {

			int colorval= I[i][j];	

			if (colorval == colorindex){
			ch2.at<uchar>(i,j)= myClrs[colorindex][0];
			ch1.at<uchar>(i,j)= myClrs[colorindex][1];
			ch0.at<uchar>(i,j)= myClrs[colorindex][2];}

				}
					}
 }
	Mat fin_img2;
	channels.push_back(ch0); //b
	channels.push_back(ch1); //g
	channels.push_back(ch2); //r
	merge(channels, fin_img2);

	imgstr << "colored_by_respected_colors_"<< imagename << quant  << type;
	string myfilename = imgstr.str();
	imgstr.str("");
	imwrite(myfilename, fin_img2);
}
	
void Labeling(int &labelNr,int label[pyrHeight][pyrWidth],int I[pyrHeight][pyrWidth],int Q[pyrHeight][pyrWidth+1],int EQ[MAX_EXP_NrOf_LABELS],ImData &myImData){
// Label (0,0) start point
		int L = 0;	
		labelNr =0;
		++L; ++labelNr;
		EQ[L]=(L);
		label[0][0]=L; Q[0][1]=L;

// Label first row 	
		for(int x=1; x<myImData.w; x++){

			int y=0;
			int n1x=x-1; 

		if(I[y][n1x]==I[y][x]){
			label[y][x]=label[y][n1x];
			Q[y][x+1]=label[y][x];}
		if(I[y][n1x]!=I[y][x]){
				++L; ++labelNr;
				EQ[L]=(L);
				label[y][x]= L;
				Q[y][x+1]=L;}
		}
			
// Label first column starting from second row	

				for(int y=1; y<myImData.h; y++){
					if(I[y][0]==I[y-1][0]){
					label[y][0]=label[y-1][0];
					Q[y][1]=label[y][0];}

					if(I[y][0]!=I[y-1][0]){
						++L; ++labelNr;
						EQ[L]=(L);
					label[y][0]=L;
					Q[y][1]=label[y][0];}
				}

//Label the rest of the img
				
			for(int x=1; x<myImData.w; x++){
				for(int y=1; y<myImData.h; y++){
					
					
				int sx= x-1; int sy=y;
				int tx=x;	 int ty=y-1;

					if(I[y][x]==I[sy][sx]	&&	I[y][x]!=I[ty][tx]){
						label[y][x] = label[sy][sx];}

					if(I[y][x]!=I[sy][sx]	&&	I[y][x]==I[ty][tx]){
						label[y][x] = label[ty][tx];}

					if(I[y][x]!=I[sy][sx]	&&	I[y][x]!=I[ty][tx]){
							++L; ++labelNr;
							EQ[L]=(L);
							label[y][x] = L;}

					if(I[y][x]==I[sy][sx]	&&	I[y][x]==I[ty][tx]	&&	label[sy][sx]==label[ty][tx]){
							label[y][x] = label[ty][tx];}

					if(I[y][x]==I[sy][sx]	&&	I[y][x]==I[ty][tx]	&&	label[sy][sx]!=label[ty][tx]){
						int comp = (label[sy][sx]<label[ty][tx]); // Ls < Lt -->  1
																  // Ls > Lt -->  0
						int L1,L2; //L1<L2
						comp ? L1 = label[sy][sx] : L1 = label[ty][tx];
						comp ? L2 = label[ty][tx] : L2 = label[sy][sx];
						
						label[y][x]=L1; 
						EQ[L2]=L1;}
					Q[y][x+1]=label[y][x];

					}}
			for(int i=0; i<myImData.h; i++){
					Q[i][0]=label[i][1];
					
					}

}

void LabelEqualization(int EQ[MAX_EXP_NrOf_LABELS],int label[pyrHeight][pyrWidth],ImData &myImData, int labelColors[10000][2]){
//Equalization of labels
				for(int k =1; k<MAX_EXP_NrOf_LABELS; k++){

				if (EQ[k]==0){break;}
				
				if(EQ[k]!=k){
					EQ[EQ[k]] == EQ[k] ? 1 :  EQ[k]=EQ[EQ[EQ[k]]];
				}

				for(int i = 0; i < myImData.h; i++) {
						for(int j = 0; j < myImData.w; j++) {
			if(label[i][j] == k)
				label[i][j] = EQ[k];
			Q[i][j+1]=label[i][j];

			//labelColors[label[i][j]][0] = I[i][j];
								}
			}
			}
}

void createHash(String dy){
	
	string line;
	ifstream myfile (dy);
	
		while ( getline (myfile,line) )
		{
			stringstream   linestream(line);
			string         data;
			int hl,hh,sl,sh,vl,vh,color_name;// HueLow, HueHigh, SaturationLow, SaturationHigh, ValueLow, ValueHigh
			
			getline(linestream, data, '\t');

			linestream >> hh >> sl >> sh >> vl >> vh >> color_name;
			hsvc new_hsvc;
			new_hsvc.hlow = atoi(data.c_str());
			new_hsvc.hhigh = hh;
			new_hsvc.slow = sl;
			new_hsvc.shigh = sh;
			new_hsvc.vlow = vl;
			new_hsvc.vhigh = vh;
			new_hsvc.col_name = color_name;

			col_hash_map.push_back(new_hsvc);
		
		}
		myfile.close();
	
}

void keepcolors(String clrs){
	int cidx = 0;
	string line;
	ifstream file (clrs);
	
		while ( getline (file,line) )
		{
			stringstream   linestream(line);
			string        data;
			int r,g,b;
			
			getline(linestream, data, '\t');
			linestream  >> g >> b; 
	
				/* Array implementation*/
			dummy[cidx][0]= atoi(line.c_str());
			dummy[cidx][1]= g;
			dummy[cidx][2]= b;

			++cidx;
		
		}
		file.close();
	
}

void init(){	

	//createHash("ColorQuantas_dummy.txt");
	createHash("ColorQuantize.txt");
	keepcolors("SegmentColors.txt");
	mystream << imagename << type;
	string myfilename = mystream.str();
	mystream.str("");
	Mat src = imread(myfilename ,CV_LOAD_IMAGE_COLOR);

	//Mat dst;
	//bilateralFilter (src,dst, 9, 30, 30 );
	//pyrDown( dst, myImData.original, Size(src.cols/2, src.rows/2) );

	Mat dst;
	pyrDown(src, dst, Size(src.cols/2, src.rows/2) );
	bilateralFilter (dst,myImData.original, 9, 30, 30 );

	cvtColor(myImData.original, myImData.intensity, CV_BGR2GRAY); 
	cvtColor(myImData.original, myImData.hsvImg,  CV_BGR2HSV);
	
	myImData.h = myImData.original.rows;
	myImData.w = myImData.original.cols;

	vector<Mat> channels;
	split(myImData.hsvImg,channels);

				myImData.hsv_label = Mat::zeros(myImData.h,myImData.w,CV_8UC1);

	myImData.hsv_filter.push_back(channels[0]);
		myImData.hsv_filter.push_back(channels[1]);
			myImData.hsv_filter.push_back(channels[2]);


	
LUT(relationLUT);
					
}

int main(){tstart = time(0);
init();
				/*int val_h1 = myImData.hsv_filter.at(0).at<uchar>(131,229);
				int val_s1 = myImData.hsv_filter.at(1).at<uchar>(131,229);
				int val_v1 = myImData.hsv_filter.at(2).at<uchar>(131,229);

				int val_h2 = myImData.hsv_filter.at(0).at<uchar>(90,168);
				int val_s2 = myImData.hsv_filter.at(1).at<uchar>(90,168);
				int val_v2 = myImData.hsv_filter.at(2).at<uchar>(90,168);

				int val_h3 = myImData.hsv_filter.at(0).at<uchar>(120,206);
				int val_s3 = myImData.hsv_filter.at(1).at<uchar>(120,206);
				int val_v3 = myImData.hsv_filter.at(2).at<uchar>(120,206);

				int val_h4 = myImData.hsv_filter.at(0).at<uchar>(196,126);
				int val_s4 = myImData.hsv_filter.at(1).at<uchar>(198,126);
				int val_v4 = myImData.hsv_filter.at(2).at<uchar>(198,126);

				int val_h11 = myImData.hsv_filter.at(0).at<uchar>(144,232);
				int val_s11 = myImData.hsv_filter.at(1).at<uchar>(144,232);
				int val_v11 = myImData.hsv_filter.at(2).at<uchar>(144,232);

				int val_h21 = myImData.hsv_filter.at(0).at<uchar>(191,96);
				int val_s21 = myImData.hsv_filter.at(1).at<uchar>(191,96);
				int val_v21 = myImData.hsv_filter.at(2).at<uchar>(191,96);

				int val_h31 = myImData.hsv_filter.at(0).at<uchar>(189,80);
				int val_s31 = myImData.hsv_filter.at(1).at<uchar>(189,80);
				int val_v31 = myImData.hsv_filter.at(2).at<uchar>(189,80);

				int val_h41 = myImData.hsv_filter.at(0).at<uchar>(188,90);
				int val_s41 = myImData.hsv_filter.at(1).at<uchar>(188,90);
				int val_v41 = myImData.hsv_filter.at(2).at<uchar>(188,90);
*/ 


 int count[TOTAL_COLOR_Nr];
 std::fill( count, count+TOTAL_COLOR_Nr, 0);

			for(int k = 0; k < ( myImData.h * myImData.w); k++){
				int x = k %  (myImData.w);
				int y = (k - x) % (myImData.w - 1);
			bool flag=false;

			for (list<hsvc>::iterator it=col_hash_map.begin(); it != col_hash_map.end(); ++it){

					
				int val_h = myImData.hsv_filter.at(0).at<uchar>(y,x);
				int val_s = myImData.hsv_filter.at(1).at<uchar>(y,x);
				int val_v = myImData.hsv_filter.at(2).at<uchar>(y,x);
            

				if(val_h >= it->hlow && val_h <= it->hhigh && val_s >= it->slow && 
					val_s <= it->shigh && val_v >= it->vlow && val_v <= it->vhigh){		
						
						c.x = x;
						c.y = y;
						c.col_info = it->col_name; ++count[(c.col_info-1)];
						myImData.hsv_col_info.push_back(c);	
						//myImData.hsv_label.at<uchar>(y,x) = c.col_info;
						I[y][x]=c.col_info;
						
						flag=true; break;		
					}

			}
		}


Labeling(labelNr,label, I, Q ,EQ, myImData);
			
LabelEqualization(EQ, label, myImData, labelColors);


  /*Merge small components with their nearest component*/

    std::unordered_map<int, int> occurrences;
 
    for (int i = 0; i < myImData.h; ++i){
		for(int j = 0; j < myImData.w; ++j){

		++occurrences[label[i][j]];}}

   for (int i = 0; i < myImData.h; ++i){
		for(int j = 0; j < myImData.w; ++j){

		if(occurrences[label[i][j]] < MAX_PxNr_SMALL_AREA) {
			EQ[label[i][j]] = Q[i][j];
			Q[i][j+1] = Q[i][j];

		}	
		}}
  // LabelEqualization(EQ, label, myImData,labelColors);

			vector<int> nIndx;
			int indx=1;

			while (indx!=labelNr+1){
			contourArray.clear();
			for(int i = 0; i < myImData.h; i++) {
		for(int j = 0; j < myImData.w; j++) {

			int val=label[i][j];		
			if (val == indx){
				contourArray.push_back(Point(j,i));
				}
		}
			}
			if(contourArray.empty() == false){
			
				myImData.connComp.push_back(contourArray);
			
			  int clrv = I[contourArray.at(0).y][contourArray.at(0).x];

			segmentClrs.push_back(clrv);
				
			if(contourArray.size()> MIN_PxNr_BIG_AREA){
				nIndx.push_back(myImData.connComp.size()-1);	
			}

			}			
			++indx;
			}


			//See all segments

			//for(int ms= 0; ms<myImData.connComp.size();ms++){
			//		Mat c_Img=Mat::zeros(myImData.h,myImData.w,CV_8UC1);
			//	for (int d =0; d<myImData.connComp.at(ms).size(); d++){
			//		Point component = myImData.connComp.at(ms).at(d);
			//		c_Img.at<uchar>(component.y,component.x)=255;
			//
			//	}
			//			imshow("c_Img", c_Img); 
			//				
			//			cout<<I[myImData.connComp.at(ms).at(0).y][myImData.connComp.at(ms).at(0).x]<<endl;	waitKey(0);
			//}
			


 			for (int nfc = 0; nfc< nIndx.size(); nfc++ ){

				int numberofcomponents = nIndx.at(nfc);
				Mat component_Img = Mat::zeros(myImData.h,myImData.w,CV_8UC1);
				Mat dilated_component_Img,dst;

				// Create binary image of big segment
			for(int comp =0; comp < myImData.connComp.at(numberofcomponents).size(); comp++){
			Point component = myImData.connComp.at(numberofcomponents).at(comp);
			component_Img.at<uchar>(component.y,component.x)=255;}
			//imshow("component_Img", component_Img);cout<<segmentClrs.at(numberofcomponents)<<endl;	waitKey(0);
			// Obtain dilated image of segment
			dilate(component_Img,dilated_component_Img,dilation_element);

			// Obtain adjacent parts
			cv::bitwise_xor(component_Img,dilated_component_Img,dst);
			//imshow("dst", dst); //adjacent img
			//	waitKey(0);

		
			//save adjacent image

	/*		seq_stream << imagename << "_" <<nfc <<type;
			string seq_name = seq_stream.str();
			seq_stream.str("");
			imwrite(seq_name,dst);
	*/
		
		
			vector<Point> nonZeroCoordinates;		//keep adjacent pixels in here
			findNonZero(dst, nonZeroCoordinates);

		int	ColorNr1 = segmentClrs.at(numberofcomponents);
		int ColorNr2;
		int newLabel = label[myImData.connComp.at(numberofcomponents).at(0).y][myImData.connComp.at(numberofcomponents).at(0).x];

		for(int g = 0; g<nonZeroCoordinates.size(); g++){
	
		Point AdjPoint	= nonZeroCoordinates.at(g);
			ColorNr2	= I[AdjPoint.y][AdjPoint.x];
			if(relationLUT[ColorNr1][ColorNr2] == 1){
	
			EQ[label[AdjPoint.y][AdjPoint.x]] = newLabel; 

			}

 		}
	
}

LabelEqualization(EQ, label, myImData, labelColors);
/*	To see the most current components again push back components and visualise */
			
			vector<Mat> channels;
			Mat ch0 =  Mat::zeros(myImData.h,myImData.w,CV_8UC1); //B
			Mat ch1 =  Mat::zeros(myImData.h,myImData.w,CV_8UC1); //G 
			Mat ch2 =  Mat::zeros(myImData.h,myImData.w,CV_8UC1); //R

		myImData.connComp.clear();

				int nindx=1;
			while (nindx!=labelNr+1){
				
			contourArray.clear();
			for(int i = 0; i < myImData.h; i++) {
		for(int j = 0; j < myImData.w; j++) {

			int nval=label[i][j];		
			if (nval == nindx){
			
				contourArray.push_back(Point(j,i));
			
			ch2.at<uchar>(i,j)= dummy[(nindx)][0];
			ch1.at<uchar>(i,j)= dummy[(nindx)][1];
			ch0.at<uchar>(i,j)= dummy[(nindx)][2];

			}
		}
			}
			if(contourArray.empty() == false){
				myImData.connComp.push_back(contourArray);
			}			
			++nindx;
			}
			Mat fin_img;
	channels.push_back(ch0); //b
	channels.push_back(ch1); //g
	channels.push_back(ch2); //r
	merge(channels, fin_img);
	/*
			for(int ms= 0; ms<myImData.connComp.size();ms++){
					Mat c_Img=Mat::zeros(myImData.h,myImData.w,CV_8UC1);
				for (int d =0; d<myImData.connComp.at(ms).size(); d++){
					Point component = myImData.connComp.at(ms).at(d);
					c_Img.at<uchar>(component.y,component.x)=255;
			
				}
						imshow("c_Img", c_Img); 
							
						cout<<I[myImData.connComp.at(ms).at(0).y][myImData.connComp.at(ms).at(0).x]<<endl;	waitKey(0);
			}
			*/
		colorImg(channels,  ch0, ch1,  ch2,  I);
	/*imshow("Colored Segments", fin_img);*/


imgstream << name<< imagename << type;
string filename = imgstream.str();
imgstream.str("");
imwrite(filename, fin_img);


//	mend = time(0);		
//	cout << "Merging components took "<< difftime(mend, mstart) <<" second(s)."<< endl;
	cout << myImData.connComp.size() <<endl;


		/*
		//If dynamic array used
		delete[] I;
		delete[] label;*/
				tend = time(0); 
cout << "It took "<< difftime(tend, tstart) <<" second(s)."<< endl;
 			waitKey(0);
			return 0;
}

