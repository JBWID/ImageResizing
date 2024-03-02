#ifndef SEAM_CARVER_HPP
#define SEAM_CARVER_HPP

#include "image_ppm.hpp"

class SeamCarver {
public:
  // default constructor
  SeamCarver() = default;

  // overloaded constructor
  SeamCarver(const ImagePPM& image);

  // sets the instances' image_ as a DEEP COPY of the image parameter. Same
  // note as above regarding ImagePPM
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
  int* GetHorizontalSeam() const;

  // returns the vertical seam of image_ with the least amount of
  // energy
  int* GetVerticalSeam() const;

  // removes one horizontal seam in image_. example:
  void RemoveHorizontalSeam();

  // removes one vertical seam in image_. example:
  void RemoveVerticalSeam();

private:
  ImagePPM image_;
  int height_ = 0;
  int width_ = 0;
};

#endif