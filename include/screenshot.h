#ifndef SCREENSHOT_H_INCLUDED_
#define SCREENSHOT_H_INCLUDED_
#include <vector>
#include <string>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>

/** @class X11Screenshot
    @brief A class for screenshots

    Process x11 image data and save to file.
*/
class X11Screenshot {
    public:
        X11Screenshot();
        X11Screenshot(XImage* image, int new_width = 0, int new_height = 0);
        bool save_to_png(const std::string& path);
        int get_width(void);
        int get_height(void);

       private:
        int width_ = 0;
        int height_ = 0;
        std::vector<std::vector<unsigned char>> image_data_ =
            std::vector<std::vector<unsigned char>>();
        std::vector<std::vector<unsigned char>> process_original(XImage * image);
};

#endif
