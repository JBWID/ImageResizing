#include "image_ppm.hpp"

// Overloaded constructor. Initializes the ImagePPM based on a plain PPM file at
// the given path
ImagePPM::ImagePPM(const std::string& path) {
  std::ifstream input_file(path);
  input_file >> *this;
  input_file.close();
}

// Copy constructor. Initializes the ImagePPM as a deep copy of the source
// ImagePPM
ImagePPM::ImagePPM(const ImagePPM& source) { *this = source; }

// Copy assignment operator. Set this to be a deep copy of source.
ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixels_[row] = new Pixel[width_];

    for (int col = 0; col < width_; col++) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }

  return *this;
}

// Destructor. Frees all free store memory used by the instance.
ImagePPM::~ImagePPM() { Clear(); }

void ImagePPM::Clear() {
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }

  delete[] pixels_;

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
}

std::istream& operator>>(std::istream& is, ImagePPM& image) {
  image.Clear();
  std::string line;
  // ignore magic number line
  getline(is, line);
  // check to see if there's a comment line
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }
  // parse width and height
  int space = line.find_first_of(' ');
  image.width_ = std::stoi(line.substr(0, space));
  image.height_ = std::stoi(line.substr(space + 1));
  // get max color value
  getline(is, line);
  image.max_color_value_ = std::stoi(line);
  // init and fill in Pixels array
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = std::stoi(line);
      getline(is, line);
      int green = std::stoi(line);
      getline(is, line);
      int blue = std::stoi(line);

      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }
  return is;
}

// Returns the pixel in the image at (row, col).
Pixel ImagePPM::GetPixel(int row, int col) const { return pixels_[row][col]; }

// Returns the max color value of the image (this is not always 255!)
int ImagePPM::GetMaxColorValue() const { return max_color_value_; }

// helper function to remove seam
void ImagePPM::SetPixelsAndHeight(Pixel** new_pixels, int new_height) {
  Clear();
  pixels_ = new_pixels;
  height_ = new_height;
}

// outputs the image in plain PPM format to os
std::ostream& operator<<(std::ostream& os, const ImagePPM& image) {
  // filetype / magic number
  os << "P3\n";

  // height and width of image
  os << image.GetWidth() << " " << image.GetHeight() << "\n";

  // maximum color value
  os << image.GetMaxColorValue() << "\n";

  // write out pixel data
  for (int row = 0; row < image.GetHeight(); row++) {
    for (int column = 0; column < image.GetWidth(); column++) {
      Pixel current_pixel = image.GetPixel(row, column);
      os << current_pixel.GetRed() << "\n";
      os << current_pixel.GetGreen() << "\n";
      os << current_pixel.GetBlue() << "\n";
    }
  }

  return os;
}