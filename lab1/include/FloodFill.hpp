#pragma once
#include "Image.hpp"
#include "Color.hpp"

class FloodFill {
public:
    // seedX, seedY  точка, откуда начинаем заливку
    // fillColor     цвет, в который красим
    // epsilon      погрешность (допуск) цвета
    static void fill(Image& img, int seedX, int seedY, Color fillColor, float epsilon);
};