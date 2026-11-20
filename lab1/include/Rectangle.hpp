#pragma once
#include "IShape.hpp"
#include "Color.hpp"
#include"LineSegment.hpp"

class Rectangle : public IShape { // Полиморфизм: Rectangle  это тоже IShape
private:
    int x_min, y_min, x_max, y_max;
    Color fillColor;
    Color strokeColor;//цв контура
    float thickness;
    bool isFilled;//флаг заливки

public:
    Rectangle(float cx, float cy, float W, float H,
                Color strokeColor, Color fill, float thickness, bool isFilled);
    void draw(Canvas& canvas) const override;
};