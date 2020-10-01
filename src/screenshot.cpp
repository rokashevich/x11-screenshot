#include <iostream>
#include <cstdlib>
#include <png.h>
#include <stdexcept>
#include "screenshot.h"

X11Screenshot::X11Screenshot() {};

X11Screenshot::X11Screenshot(XImage *image, int new_width, int new_height) {
  this->width_ = image->width;
  this->height_ = image->height;
  this->image_data_ = this->process_original(image);
};

std::vector<std::vector<unsigned char>> X11Screenshot::process_original(XImage * image) {
    std::vector<std::vector<unsigned char>> image_data;
    std::vector<unsigned char> image_data_row;
    unsigned long red_mask = image->red_mask;
    unsigned long green_mask = image->green_mask;
    unsigned long blue_mask = image->blue_mask;

    for (int y = 0; y < this->height_; y++) {
      for (int x = 0; x < this->width_; x++) {
        unsigned long pixel = XGetPixel(image, x, y);

        unsigned char blue = pixel & blue_mask;
        unsigned char green = (pixel & green_mask) >> 8;
        unsigned char red = (pixel & red_mask) >> 16;

        image_data_row.push_back(red);
        image_data_row.push_back(green);
        image_data_row.push_back(blue);
      }
      image_data.push_back(image_data_row);
      image_data_row.clear();
    }

    return image_data;
};

bool X11Screenshot::save_to_png(const std::string &path) {
  FILE *fp = NULL;
  png_structp png_ptr = NULL;
  png_infop info_ptr = NULL;
  png_bytep row;

  fp = fopen(path.c_str(), "wb");
  if (!fp) {
    std::cout << "Failed to create file " << path << std::endl;
    return false;
  }

  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    std::cout << "Failed to create PNG write structure" << std::endl;
    fclose(fp);
    return false;
  }

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    std::cout << "Failed to create PNG info structure" << std::endl;
    fclose(fp);
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    return false;
  }

  // Setup png lib error handling
  if (setjmp(png_jmpbuf(png_ptr))) {
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return false;
  }

  png_init_io(png_ptr, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(png_ptr, info_ptr, this->width_, this->height_, 8,
               PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
  // set compression level
  png_set_compression_level(png_ptr, PNG_Z_DEFAULT_COMPRESSION);
  // write info header
  png_write_info(png_ptr, info_ptr);
  for (std::vector<std::vector<unsigned char>>::size_type i = 0;
       i != this->image_data_.size(); i++) {
    // build character row from array of characters
    row = (png_bytep) reinterpret_cast<unsigned char *>(
        this->image_data_[i].data());
    // write byterow
    png_write_row(png_ptr, row);
  }
  // end writing file
  png_write_end(png_ptr, NULL);
  // cleanup
  if (fp != NULL) fclose(fp);
  if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
  if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

  return true;
};
