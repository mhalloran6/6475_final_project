#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath> 
#include <algorithm>

using namespace cv;
using namespace std;

void smooth_horiz(Mat& img, int j, int i, int ywindow, int xwindow, double xstep) {
  int rows = img.rows;
  int cols = img.cols;
  
  int x_max = i + (int)(((double) ywindow) * xstep) + xwindow;
  
  if ((i < 1) || (x_max > cols - 1) || (j + ywindow > rows - 1)) {
    return;
  }
  
  int lower_left_i = i + (double) ywindow * xstep;
  //  cout << "ul:" << j << "," << i << endl;
  //  cout << "ll:" << j + ywindow << "," << lower_left_i << endl;
  
  Vec3b upper_left = img.at<Vec3b>(j, i);
  Vec3b upper_left_minus = img.at<Vec3b>(j, i -1);
  Vec3b lower_left = img.at<Vec3b>(j + ywindow, lower_left_i);
  Vec3b lower_left_minus = img.at<Vec3b>(j + ywindow, lower_left_i -1);
  Vec3b color = upper_left;
  Vec3b trans_color = upper_left_minus;
  
  // check to make sure we are on a color transition upper left
  int transition = ( upper_left != upper_left_minus);
  int transition2 = ( lower_left != lower_left_minus);
  
  if (!(transition && transition2)) { return; }
  if (upper_left != lower_left) { return; }
  if (upper_left_minus != lower_left_minus) { return; }
  
  
  // check the right
  int right_passed = 1;
  for (int k = 0; k < ywindow; k++) {
    int jj = j + k;
    int ii = i + xwindow + ((double) k) * xstep;
    //cout << "jj:" << jj << ",ii:"<<ii<<endl;
    Vec3b color2 = img.at<Vec3b>(jj, ii);
    if (color2 != color) {
      right_passed = 0;
    }
  }

  // check the left
  int left_passed = 1;
  for (int k = 0; k < ywindow; k++) {
    int jj = j + k;
    int ii = i  + ((double) k) * xstep;
    //cout << "jj:" << jj << ",ii:"<<ii<<endl;
    Vec3b color2 = img.at<Vec3b>(jj, ii);
    if (color2 != color) {
      left_passed = 0;
    }
  }

  if (!left_passed && !right_passed) {
    return;
  }

  // check the top of the trap
  for (int ii = i; ii < i + xwindow; ii++) {
    Vec3b color2 = img.at<Vec3b>(j, ii);
    if (color2 != color) {
      return;
    }
  }

  // check the bottom of the trap
  int btm_start = i + (int)(xstep * (double) ywindow);
  int btm_end = btm_start + xwindow;

  for (int ii = btm_start; ii < btm_end; ii++) {
    Vec3b color2 = img.at<Vec3b>(j + ywindow, ii);
    if (color2 != color) {
      return;
    }
  }
  
  // fill the trap
  for (int jj = j; jj < j + ywindow; jj++) {
    int start = i + xstep * (double) (jj - j);
    int end = i + xstep * (double) (jj - j) + xwindow;
    //cout << "y:" << jj << " start:" << start << " end:" << end << endl;
    for (int ii = start; ii < end;  ii++) {
      //Vec3b color2 = img.at<Vec3b>(jj, ii);
      //if ((color2 == color) || (color2 == trans_color)) {
      img.at<Vec3b>(jj, ii) = color;
      //}
    }
  }
}







int main( int argc, char** argv )
{
  if( argc != 2)
    {
      cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
      return -1;
    }

  Mat image;
  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

  namedWindow( "Original", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Original", image );                   // Show our image inside it.
  
  int rows = image.rows;
  int cols = image.cols;
  if (0) {
  for (int j = 0; j < rows; j++) {
    cout << j << endl;
    for (int i = 0; i < cols; i++) {
      for (int k = 1; k < 30; k++) {
	check_box_and_fill(image, j, i, k);
      }
    }
  }}

  for (int j = 0; j < rows; j++) {
    cout << j << endl;
    for (int i = 0; i < cols; i++) {
      
      for (int jj = 1; jj < 100; jj+=10) {
	for (int ii = 1; ii < 100; ii+=10) {
	  for (double step = -20; step < 20; step += 2) {
	    smooth_horiz(image, j, i, jj, ii, step); 
	  }
	}
      }
    }
  }
  if (0) {
    for (int j = 0; j < rows; j++) {
      for (int i = 0; i < cols; i++) {
	for (int k = 1; k < 10; k++) {
	  check_box_and_fill(image, j, i, k);
	}
      }
    }
  }
  
  if(! image.data )                              // Check for invalid input
    {
      cout <<  "Could not open or find the image" << std::endl ;
      return -1;
    }
  
  namedWindow( "Post", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Post", image );                   // Show our image inside it.
  
  waitKey(0);                                          // Wait for a keystroke in the window
  return 0;
}

