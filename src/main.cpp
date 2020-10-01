#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <cstdlib>
#include <iostream>
#include <iterator>

#include "screenshot.h"

int main(int argc, char** argv) {
  Display* display;
  Window root;

  XWindowAttributes gwa;
  X11Screenshot screenshot;

  display = XOpenDisplay(NULL);
  if (display == NULL) {
    std::cerr << "No display can be aquired" << std::endl;
    exit(1);
  }
  root = DefaultRootWindow(display);

  XGetWindowAttributes(display, root, &gwa);
  int width = gwa.width;
  int height = gwa.height;

  XImage* image =
      XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);
  screenshot = X11Screenshot(image);

  std::vector<std::string> opts;
  std::copy(argv, argv + argc,
            std::back_insert_iterator<std::vector<std::string>>(opts));
  if (opts.size() == 2)
    screenshot.save_to_png(opts.at(1));
  else
    std::cout << "Usage: screenshot file.png" << std::endl;

  XDestroyImage(image);
  XCloseDisplay(display);
  return 0;
}
