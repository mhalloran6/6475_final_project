#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath> 
#include <algorithm>

using namespace cv;
using namespace std;


void check_box_and_fill(Mat& img, int j, int i, int half_window) {
  int rows = img.rows;
  int cols = img.cols;
  int cn = 3;
  
  if (j < half_window || i < half_window) {
    return;
  }
  if (i + half_window > cols - 1) {
    return;
  }
  if (j + half_window > rows - 1) {
    return;
  }

  unsigned char *input = (unsigned char*)(img.data);
  int is_boxed = 1;
  
  int index = (j - half_window) * img.step + (i - half_window) * cn;
  Vec3b color = img.at<Vec3b>(j - half_window, i - half_window);
  

  for (int jj = j - half_window; jj < j + half_window; jj++) {
    Vec3b color1 = img.at<Vec3b>(jj, i - half_window);
    Vec3b color2 = img.at<Vec3b>(jj, i + half_window);
    if (color1 != color || color2 != color) {
      is_boxed = 0;
    }
  }

  for (int ii = i - half_window; ii < i + half_window; ii++) {
    Vec3b color1 = img.at<Vec3b>(j - half_window, ii);
    Vec3b color2 = img.at<Vec3b>(j + half_window, ii);
    if (color1 != color || color2 != color) {
      is_boxed = 0;
    }
  }

  if (is_boxed) {
    for (int jj = j - half_window; jj < j + half_window; jj++) {
      for (int ii = i - half_window; ii < i + half_window; ii++) {
	img.at<Vec3b>(jj, ii) = color;
      }
    }
  }
}


int main( int argc, char** argv )
{
  if( argc != 3)
    {
      cout <<" Usage: box_fill <input> <output>" << endl;
      return -1;
    }
  char* output = argv[2];
  Mat image;
  image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

  namedWindow( "Original", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Original", image );                   // Show our image inside it.
  
  int rows = image.rows;
  int cols = image.cols;

  for (int j = 0; j < rows; j++) {
    cout << j << endl;
    for (int i = 0; i < cols; i++) {
      for (int k = 1; k < 10; k++) {
	check_box_and_fill(image, j, i, k);
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
  
  //waitKey(0);                                          // Wait for a keystroke in the window
  imwrite(output, image);
  return 0;
}

