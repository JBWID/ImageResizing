#include "seam_carver.hpp"

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
  Pixel left = image_.GetPixel(row, (col - 1));
  Pixel right = image_.GetPixel(row, (col + 1));
  int r_col = left.GetRed() + right.GetRed();
  int g_col = left.GetGreen() - right.GetGreen();
  int b_col = left.GetBlue() - right.GetBlue();
  int col_change_squared = r_col * r_col + g_col * g_col + b_col * b_col;

  // calculating row energy (vertical)
  Pixel top = image_.GetPixel((row - 1), col);
  Pixel bottom = image_.GetPixel((row + 1), col);
  int r_row = top.GetRed() - bottom.GetRed();
  int g_row = top.GetGreen() - bottom.GetGreen();
  int b_row = top.GetBlue() - bottom.GetBlue();
  int row_change_squared = r_row * r_row + g_row * g_row + b_row * b_row;

  // calculating energy E
  int energy = row_change_squared + col_change_squared;
  return energy;
}

// Returns the horizontal seam of image_ with the least amount of energy. Notice
// the array returned by this method is on the free store; keep in mind when the
// memory should be free’d.
int* SeamCarver::GetHorizontalSeam() const {}

// Returns the vertical seam of image_ with the least amount of energy. Notice
// the array returned by this method is on the free store; keep in mind when the
// memory should be free’d.
int* SeamCarver::GetVerticalSeam() const {}

// Removes one horizontal seam in image_. Creating a public member function of
// the ImagePPM class to carve a seam might help.
void SeamCarver::RemoveHorizontalSeam() {}

// Removes one vertical seam in image_. Creating a public member function of the
// ImagePPM class to carve a seam might help.
void SeamCarver::RemoveVerticalSeam() {}