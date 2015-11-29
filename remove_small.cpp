#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath> 
#include <algorithm>
#include <vector>


using namespace cv;
using namespace std;

struct blob_info {
  int size;
  int id;
  int neighbor_id;
  int i;
  int j;
  Vec3b neighbor_color;
  Vec3b color;
  blob_info(int idd) {
    id = idd;
    size = 0;
  }
};


struct point {
  int j;
  int i;
  point(int jj, int ii) {
      i = ii;
      j = jj;
  }
};


int mark_blobs(const Mat& img, Mat& markings) {
  int next_blob_id = 1;
  int rows = img.rows;
  int cols = img.cols;
  
  for (int j = 0; j < rows; j++) {
    for (int i = 0; i < cols; i++) {
      Vec3b color = img.at<Vec3b>(j,i);
      
      // check the neighbors for a marked blob
      int marked = 0;
      
      int ii_min = max(i - 1, 0);
      int ii_max = min(i + 1, cols - 1);
      int jj_min = max(j - 1, 0);
      int jj_max = min(j + 1, rows - 1);
      
      // search across neighboring cells to see
      // if the color matches.
      for (int ii = ii_min; ii <= ii_max; ii++) {
	for (int jj = jj_min; jj <= jj_max; jj++) {
	  if (jj == j && ii == i) 
	    continue; // Ignore the current cell
	  
	  int marked_id = markings.at<int>(jj, ii);
	  
	  if (!marked_id) // check to see if the neighbor was assigned to a blob
	    continue; 
	  	  
	  Vec3b neighboring_color = img.at<Vec3b>(jj, ii);
	  
	  if (color == neighboring_color) {
	    markings.at<int>(j,i) = marked_id;
	    marked = true;
	    goto done_neighbor_check;
	  }
	}
      }
    done_neighbor_check:
      if (!marked) {
	markings.at<int>(j,i) = next_blob_id;
	next_blob_id++;
      }
    }
  }
  return next_blob_id - 1;
}

int get_neighbor(Mat& markings, int j, int i) {
  int cols = markings.cols;
  int rows = markings.rows;
  
  int ii_min = max(i - 1, 0);
  int ii_max = min(i + 1, cols - 1);
  int jj_min = max(j - 1, 0);
  int jj_max = min(j + 1, rows - 1);
  
  int id = markings.at<int>(j, i);
  // search across neighboring cells to see
  // if the color matches.
  for (int ii = ii_min; ii <= ii_max; ii++) {
    for (int jj = jj_min; jj <= jj_max; jj++) {
      int neighbor_id = markings.at<int>(jj, ii);
      if (neighbor_id != id) {
	return neighbor_id;
      }
    }
  }
  return 0;
}


int mark_blob(Mat& img, int j, int i, Mat& markings, int blob_id) {
  vector<point*> points;
  int expanded = 0;
  
  Vec3b color = img.at<Vec3b>(j, i);
  if (markings.at<int>(j,i)) 
    return 0;
  markings.at<int>(j,i) = blob_id;

  points.push_back(new point(j,i));
  int area_size = 0;

  while(points.size()) {
    point* p = points.front();
    int i = p->i;
    int j = p->j;
    int i_s[] = {i - 1, i, i + 1};
    int j_s[] = {j - 1, j, j + 1};
    for (int ii : i_s) {
      for (int jj : j_s) {

	if ((ii == i) && (jj == j))
	  continue;
	
	if ((ii < 0) || (ii >= img.cols) || (jj < 0) || (jj >= img.rows))
	  continue;

	if ((img.at<Vec3b>(jj, ii) == color) && 
	    (markings.at<int>(jj,ii) == 0)) {
	  points.push_back(new point(jj,ii));

	  markings.at<int>(jj,ii) = blob_id;
	}
	
      }
    }
    points.erase(points.begin());
    area_size++;
  }

  for (int i =0; i< points.size();i++) {
    delete points[i];
  }
  points.clear();

  return area_size;
}


struct neighbor {
  int id;
  Vec3b color;
};

void fill_blob_neighbors(neighbor blob_neighbors[], Mat& markings, Mat& image) {  
  int rows = markings.rows;
  int cols = markings.cols;
  
  Mat marking = Mat::zeros(rows, cols, CV_32S);
  
  int blob_id = 1;
  
  for (int j = 0; j < rows; j++) {
    for (int i = 0; i < cols; i++) {
            
      int ii_min = max(i - 1, 0);
      int ii_max = min(i + 1, cols - 1);
      int jj_min = max(j - 1, 0);
      int jj_max = min(j + 1, rows - 1);

      int id = markings.at<int>(j, i);
      int done = 0;
      for (int ii = ii_min; ii <= ii_max; ii++) {
	for (int jj = jj_min; jj <= jj_max; jj++) {
	  int neighbor_id = markings.at<int>(jj, ii);
	  if (neighbor_id != id) {
	    done = 1;
	    blob_neighbors[id].id = neighbor_id;
	    blob_neighbors[id].color = image.at<Vec3b>(jj, ii);
	    break;
	  }
	}
	if (done) 
	  break;
      }
    }
  }
}

void fill_blob_sizes(int blob_sizes[], Mat& markings, int n_blobs) {
  int cols = markings.cols;
  int rows = markings.rows;
  for (int j = 0; j < rows; j++) {
    for (int i = 0; i < cols; i++) {
      blob_sizes[markings.at<int>(j,i)]++;
    }
  }
}

void remove_blobs(int blob_sizes[], neighbor blob_neighbors[], Mat& markings, Mat& image, int area) {
  int cols = markings.cols;
  int rows = markings.rows;
  for (int j = 0; j < rows; j++) {
    for (int i = 0; i < cols; i++) {
      int id = markings.at<int>(j,i);
      if (blob_sizes[id] < area) {
	image.at<Vec3b>(j,i) = blob_neighbors[id].color;
      }
    }
  }
}

int main( int argc, char** argv )
{
  if( argc != 4)
    {
      cout <<" Usage: remove_small area <img> <out>" << endl;
      return -1;
    }

  Mat image;
  image = imread(argv[2], CV_LOAD_IMAGE_COLOR);   // Read the file
  char* output = argv[3];
  int size = atoi(argv[1]);
  namedWindow( "Original", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Original", image );                   // Show our image inside it.
  
  int rows = image.rows;
  int cols = image.cols;
  
  Mat marking = Mat::zeros(rows, cols, CV_32S);
  int blobs = mark_blobs(image, marking);
  int blob_sizes[blobs];
  memset(blob_sizes, 0, sizeof(int)*blobs);
  neighbor* blob_neighbors = new neighbor[blobs];
  memset(blob_neighbors, 0, sizeof(neighbor)*blobs);
  fill_blob_sizes(blob_sizes, marking, blobs);
  fill_blob_neighbors(blob_neighbors, marking, image);
  remove_blobs(blob_sizes, blob_neighbors, marking, image, size);
  cout << "Done" << endl;
  if(! image.data )                              // Check for invalid input
    {
      cout <<  "Could not open or find the image" << std::endl ;
      return -1;
    }
  
  namedWindow( "Post", WINDOW_AUTOSIZE );// Create a window for display.
  imshow( "Post", image );                   // Show our image inside it.
  
  waitKey(0);                                          // Wait for a keystroke in the window
  imwrite(output, image);
  return 0;
}

