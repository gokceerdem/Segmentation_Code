#ifndef MYLIB_CONSTANTS_H
#define MYLIB_CONSTANTS_H

using namespace cv;
//  File Name : LibConstants.hpp    Purpose : Global Constants for Lib Utils

//namespace LibConstants
//{
  const int pyrHeight = 500;
  const int pyrWidth = 385;
  const int TOTAL_COLOR_Nr = 22;
  const int ALONE_PxNr = 1;
  const int MAX_PxNr_SMALL_AREA =70; //N_1
  const int MIN_PxNr_BIG_AREA = 70;//pyrHeight*pyrWidth*0.01200; //0.01320 OF TOTAL PIXEL NUMBER //N_2
  const int MAX_EXP_NrOf_LABELS = 8000;
  int dilation_size = 3; // adjust with you application
  Mat dilation_element = getStructuringElement( MORPH_ELLIPSE,Size( 2*dilation_size + 1, 2*dilation_size+1 ),Point( dilation_size, dilation_size ) );
  const string name = "Segment-";
  const string type = ".jpg";
  const String imagename = "img3" ; 
  const String quant = "-colorQuant-";
  //const string type = ".jpeg";
  //const String imagename = "Img2" ; 
//}
#endif

// ADD TO .CPP --->> int dilation_size = LibConstants::dilation_size; 
//IF
//  namespace LibConstants
//					{.
//					...
//					}
// USED.
