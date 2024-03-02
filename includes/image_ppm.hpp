#ifndef IMAGE_PPM_HPP
#define IMAGE_PPM_HPP

#include <fstream>
#include <iostream>
#include <string>

#include "pixel.hpp"

class ImagePPM {
public:
  // default constructor
  ImagePPM() = default;

  // overloaded constructor
  ImagePPM(const std::string& path);

  // copy constructor.
  ImagePPM(const ImagePPM& source);

  // assignment operator
  ImagePPM& operator=(const ImagePPM& source);

  // destructor
  ~ImagePPM();

  // returns the Pixel at row col. You may assume that row and col
  // will always be within the bounds of image_
  Pixel GetPixel(int row, int col) const;

  // returns the width of the image
  int GetWidth() const { return width_; };

  // returns the height of the image
  int GetHeight() const { return height_; };

  // returns the max color value of the image
  int GetMaxColorValue() const;

  // outputs the image in plain PPM format to os
  friend std::ostream& operator<<(std::ostream& os, const ImagePPM& image);

  // fills in image using the input stream, is. the stream contains
  // an image in plain PPM format
  friend std::istream& operator>>(std::istream& is, ImagePPM& image);

private:
  int height_ = 0;
  int width_ = 0;
  int max_color_value_ = 0;
  Pixel** pixels_ = nullptr;

  // given help function, "clears" the data of the instance
  void Clear();

};

#endif