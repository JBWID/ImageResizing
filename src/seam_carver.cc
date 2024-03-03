#include "seam_carver.hpp"

#include "iostream"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

// Overloaded constructor. Initializes the instance with a deep copy of image.
SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

// Sets the SeamCarver instance’s image_ to be a deep copy of image
void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

// to implement

// Returns the instance’s image
const ImagePPM& SeamCarver::GetImage() const { return image_; }

// Returns the height of the instance’s image
int SeamCarver::GetHeight() const { return height_; }

// Returns the width of the instance’s image
int SeamCarver::GetWidth() const { return width_; }

// Returns the energy of the pixel at (row, col) of the instance’s image. You
// may assume that row and col will always be within bounds.
int SeamCarver::GetEnergy(int row, int col) const {
  // calculating column energy (horizontal)
  Pixel left;
  // handling wrap around cases
  if (col - 1 < 0) {
    left = image_.GetPixel(row, width_ - 1);
  } else {
    left = image_.GetPixel(row, col - 1);
  }
  Pixel right;
  if (col + 1 > width_ - 1) {
    right = image_.GetPixel(row, 0);
  } else {
    right = image_.GetPixel(row, (col + 1));
  }
  int r_col = right.GetRed() - left.GetRed();
  int g_col = right.GetGreen() - left.GetGreen();
  int b_col = right.GetBlue() - left.GetBlue();
  int col_change_squared = r_col * r_col + g_col * g_col + b_col * b_col;

  // calculating row energy (vertical)
  // handing wrap around cases
  Pixel top;
  if (row - 1 < 0) {
    top = image_.GetPixel((height_ - 1), col);
  } else {
    top = image_.GetPixel((row - 1), col);
  }
  Pixel bottom;
  if (row + 1 > height_ - 1) {
    bottom = image_.GetPixel(0, col);
  } else {
    bottom = image_.GetPixel((row + 1), col);
  }
  int r_row = top.GetRed() - bottom.GetRed();
  int g_row = top.GetGreen() - bottom.GetGreen();
  int b_row = top.GetBlue() - bottom.GetBlue();
  int row_change_squared = r_row * r_row + g_row * g_row + b_row * b_row;

  // calculating energy E
  int energy = row_change_squared + col_change_squared;
  return energy;
}

// finding vertical and horizontal seam using memoization algorithm
// Returns the horizontal seam of image_ with the least amount of energy. Notice
// the array returned by this method is on the free store; keep in mind when the
// memory should be free’d.
int* SeamCarver::GetHorizontalSeam() const {
  // make energy 2d array, populate with energies using memoization algorithm
  // make a horizontal array for n number of columns
  int** array = new int*[height_];
  // make rows for every column
  for (int i = 0; i < height_; i++) {
    array[i] = new int[width_];
  }

  // populate rightmost column with actual energy values
  for (int row = 0; row < height_; row++) {
    array[row][width_ - 1] = GetEnergy(row, width_ - 1);
  }

  // calculate and populate for every column leftwards
  for (int col = width_ - 2; col >= 0; col--) {
    for (int row = 0; row < height_; row++) {
      int top = 0;
      int middle = GetEnergy(row, col + 1);
      int bottom = 0;

      // handling wrap around cases
      if (row == 0) {
        top = GetEnergy(height_ - 1, col + 1);
      } else {
        top = GetEnergy(row - 1, col + 1);
      }
      if (row == height_ - 1) {
        bottom = GetEnergy(0, col + 1);
      } else {
        bottom = GetEnergy(row + 1, col + 1);
      }

      int lowest = 0;
      // which one is lowest
      if (top < middle && top < bottom) {
        lowest = top;
      } else if (middle < top && middle < bottom) {
        lowest = middle;
      } else {
        lowest = bottom;
      }

      // set current pixel to lowest energy from right
      array[row][col] = GetEnergy(row, col) + lowest;
    }
  }

  // tracing horizontal seam from leftmost column to rightmost, starting with
  // lowest energy pixel
  int* seam = new int[width_];

  // deciding which pixel to start from if equal, start from topmost
  // since iterating from top to bottom, it will always be topmost
  int starting_row = 0;
  for (int row = 1; row < height_; row++) {
    if (array[row][0] < array[starting_row][0]) {
      starting_row = row;
    }
  }

  // start tracing rightwards
  seam[0] = starting_row;
  int current_row = starting_row;
  for (int col = 1; col < width_; col++) {
    current_row = seam[col - 1];

    // edge case: if equal, always pick middle, then top then bottom
    int lowest = array[current_row][col - 1];
    int change = 0;
    if (current_row > 0 && array[current_row - 1][col - 1] < lowest) {
      lowest = array[current_row - 1][col - 1];
      change = -1;
    }
    if (current_row < height_ - 1 && array[current_row + 1][col - 1] < lowest) {
      change = 1;
    }

    // update seam row based on the lowest energy from the left
    seam[col] = current_row + change;
  }

  // free allocated memory for array
  for (int i = 0; i < height_; i++) {
    delete[] array[i];
  }
  delete[] array;

  return seam;
}

// Returns the vertical seam of image_ with the least amount of energy. Notice
// the array returned by this method is on the free store; keep in mind when the
// memory should be free’d.
int* SeamCarver::GetVerticalSeam() const {
  // make energy 2d array, populate with energies using memoization algorithm
  // make a vertical array for n number of rows
  int** array = new int*[height_];
  // make columns for every row
  for (int i = 0; i < height_; i++) {
    array[i] = new int[width_];
  }

  // populate bottom row with actual energy values
  for (int col = 0; col < width_; col++) {
    array[height_ - 1][col] = GetEnergy(height_ - 1, col);
  }

  // calculate and populate for every row upwards
  for (int row = height_ - 2; row >= 0; row--) {
    for (int column = 0; column < width_; column++) {
      int left = 0;
      int middle = GetEnergy(row + 1, column);
      int right = 0;

      // handling wrap around cases
      if (column == 0) {
        left = GetEnergy(row + 1, width_ - 1);
      } else {
        left = GetEnergy(row + 1, column - 1);
      }
      if (column == width_ - 1) {
        right = GetEnergy(row + 1, 0);
      } else {
        right = GetEnergy(row + 1, column + 1);
      }

      int lowest = 0;
      // which one is lowest
      if (left < middle && left < right) {
        lowest = left;
      }
      if (middle < left && middle < right) {
        lowest = middle;
      }
      if (right < middle && right < left) {
        lowest = right;
      }

      // set current pixel to lowest energy
      array[row][column] = GetEnergy(row, column) + lowest;
    }
  }

  // tracing vertical seam from top row to bottom row, starting with lowest
  // energy pixel
  int* seam = new int[height_];

  // deciding which pixel to start from if equal, start from leftmost
  // since iterating from left to right, it will always be leftmost
  int starting_column = 0;
  for (int column = 0; column < width_; column++) {
    if (array[0][column] < array[0][starting_column]) {
      starting_column = column;
    }
  }

  // start tracing downwards
  seam[0] = starting_column;
  int column = 0;
  for (int row = 1; row < height_; row++) {
    column = seam[row - 1];

    // edge case: if equal, always pick middle, then left then right
    int lowest = array[row][column];
    int change = 0;
    if (column > 0 && array[row][column - 1] < lowest) {
      lowest = array[row][column - 1];
      change = -1;
    }
    if (column < width_ - 1 && array[row][column + 1] < lowest) {
      lowest = array[row][column + 1];
      change = 1;
    }

    // update seam column
    seam[row] = column + change;
  }
  // free allocated memory for array
  for (int i = 0; i < height_; i++) {
    delete[] array[i];
  }
  delete[] array;

  return seam;
}
// Removes one horizontal seam in image_. Creating a public member function of
// the ImagePPM class to carve a seam might help.
void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();

  // create a new 2d array for the new image with 1 less row
  Pixel** new_array = new Pixel*[height_ - 1];
  for (int i = 0; i < height_ - 1; i++) {
    new_array[i] = new Pixel[width_];
  }

  // copy old array, but skip over seam
  for (int row = 0; row < height_; row++) {
    for (int column = 0; column < width_; column++) {
      // copy pixels above the seam
      if (row < seam[column]) {
        new_array[row][column] = image_.GetPixel(row, column);

        // move pixxels upwards if its below the seam
      } else if (row > seam[column]) {
        new_array[row - 1][column] = image_.GetPixel(row, column);
      }
    }
  }

  // update the picture
  image_.SetPixelsAndHeight(new_array, image_.GetHeight() - 1);

  // remove memory used by seam
  delete[] seam;
}

// Removes one vertical seam in image_. Creating a public member function of
// the ImagePPM class to carve a seam might help.
void SeamCarver::RemoveVerticalSeam() {
  int* seam = GetVerticalSeam();

  // create a new 2d array for the new image with 1 less row
  Pixel** new_array = new Pixel*[height_];
  for (int i = 0; i < height_; i++) {
    new_array[i] = new Pixel[width_ - 1];
  }

  // copy old array, but skip over seam
  for (int row = 0; row < height_; row++) {
    int col = 0;
    for (int column = 0; column < width_; column++) {
      // if current pixel is not a part of the seam, copy it
      if (column != seam[row]) {
        new_array[row][col] = image_.GetPixel(row, col);
        col++;
      }
    }
  }

  // update the picture
  image_.SetPixelsAndWidth(new_array, width_ - 1);

  // remove memory used by seam
  delete[] seam;
}