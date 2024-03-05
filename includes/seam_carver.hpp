#ifndef SEAM_CARVER_HPP
#define SEAM_CARVER_HPP

#include "image_ppm.hpp"

class SeamCarver {
public:
  // default constructor
  SeamCarver() = default;

  // overloaded constructor
  SeamCarver(const ImagePPM& image);

  // sets the instances' image_ as a DEEP COPY of the image parameter.
  void SetImage(const ImagePPM& image);

  // returns the instance's image_
  const ImagePPM& GetImage() const;

  // returns the image's height
  int GetHeight() const;

  // returns the image's width
  int GetWidth() const;

  // returns the energy of the pixel at row col in image_
  int GetEnergy(int row, int col) const;

  // returns the horizontal seam of image_ with the least amount of
  // energy
  // returns {1, 0, 1, 2}
  int* GetHorizontalSeam() const;

  // returns the vertical seam of image_ with the least amount of
  // energy
  int* GetVerticalSeam() const;

  // removes one horizontal seam in image_. example:
  void RemoveHorizontalSeam();

  // removes one vertical seam in image_. example:
  void RemoveVerticalSeam();

  int GetColEnergy(int row, int col) const;

  int GetRowEnergy(int row, int col) const;

  int** HorizontalInitPopulateEnergyArray() const;

  int* TraceHorizontalSeam(int** array) const;

  int** VerticalInitPopulateEnergyArray() const;

  int* TraceVerticalSeam(int** array) const;

  int** InitEnergyArray() const;

private:
  ImagePPM image_;
  int height_ = 0;
  int width_ = 0;
  static const int kMaxEnergy = 2147483647;
};

#endif