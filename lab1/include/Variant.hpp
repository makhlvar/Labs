#pragma once
#include "Image.hpp"
#include "Color.hpp"

class Variant2Truchet {
public:
    // N и M — количество клеток по X и Y
    static void generate(Image& img, int N, int M, Color c0, Color c1);
};