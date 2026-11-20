#pragma once
#include <vector>
#include "Color.hpp"
#include <fstream>

class Image {
private:
    int width;
    int height; 
    std::vector<Color> buf;

public:
    Image();
    Image(int width, int height);
    
    int getHeight() const { return height; }
    int getWidth() const { return width; }

    Color& getPixel(int x, int y);
    const Color& getPixel(int x, int y) const;

    Color* getRawData() { return buf.data(); }

    void clear(const Color& c);
    void resize(int new_h, int new_w);
    void resizeSSAA(int targetW, int targetH, int factor);
};