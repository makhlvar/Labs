#include "../include/Image.hpp"
#include <stdexcept>
#include <algorithm>

Image::Image() : width(0),height(0) {}
Image::Image(int width,int height) :  width(width),height(height), buf(height * width) {}

Color& Image::getPixel(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) throw std::out_of_range("Pixel out of bounds");//в мэйн перехватится
    return buf[y * width + x];
}

const Color& Image::getPixel(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) throw std::out_of_range("Pixel out of bounds");
    return buf[y * width + x];
}

void Image::clear(const Color& c) {
    std::fill(buf.begin(), buf.end(), c);
}

void Image::resize(int new_h, int new_w) {
    std::vector<Color> newBuf(new_h * new_w);
    int minH = std::min(height, new_h);
    int minW = std::min(width, new_w);
    for (int y = 0; y < minH; ++y)
        for (int x = 0; x < minW; ++x)
            newBuf[y * new_w + x] = buf[y * width + x];
    buf = newBuf;
    height = new_h;
    width = new_w;
}

void Image::resizeSSAA(int targetW, int targetH, int factor) {
    std::vector<Color> newBuf(targetW * targetH);
    int highW = targetW * factor;
    int totalSub = factor * factor;

    for (int y = 0; y < targetH; ++y) {
        for (int x = 0; x < targetW; ++x) {
            float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
            for (int sy = 0; sy < factor; ++sy) {
                for (int sx = 0; sx < factor; ++sx) {
                    int hx = x * factor + sx;
                    int hy = y * factor + sy;
                    Color c = buf[hy * highW + hx]; 
                    sumR += c.r; sumG += c.g; sumB += c.b;
                }
            }
            newBuf[y * targetW + x] = Color(sumR / totalSub, sumG / totalSub, sumB / totalSub, 1.0f);
        }
    }
    buf = newBuf;
    width = targetW;
    height = targetH;
}