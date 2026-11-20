#include "../include/Rectangle.hpp"
#include "../include/Canvas.hpp"
#include "../include/LineSegment.hpp"

Rectangle::Rectangle(float cx, float cy, float W, float H, 
                     Color stroke, Color fill, float th, bool filled)
    : fillColor(fill), strokeColor(stroke), thickness(th), isFilled(filled) {
    // Из ТЗ: x_min = cx - W/2, y_min = cy - H/2 (и симметрично для max)
    x_min = static_cast<int>(cx - W / 2.0f);
    y_min = static_cast<int>(cy - H / 2.0f);
    x_max = static_cast<int>(cx + W / 2.0f);
    y_max = static_cast<int>(cy + H / 2.0f);
}

void Rectangle::draw(Canvas& canvas) const {
    if(isFilled){
    // Проходим по всем пикселям прямоугольника
        for (int y = y_min; y <= y_max; ++y) {
            for (int x = x_min; x <= x_max; ++x) {
            // Canvas сам отсечет то, что вышло за пределы холста
                canvas.setPixel(x, y, fillColor);
        }
     }
    }
    float fx_min = static_cast<float>(x_min);
    float fy_min = static_cast<float>(y_min);
    float fx_max = static_cast<float>(x_max);
    float fy_max = static_cast<float>(y_max);

    LineSegment top(fx_min,fy_min,fx_max,fy_min,strokeColor,thickness,LineAlgorithm::BRESENHAM);
    top.draw(canvas);
    LineSegment right(fx_max,fy_min,fx_max,fy_max,strokeColor,thickness,LineAlgorithm::BRESENHAM);
    right.draw(canvas);
    LineSegment bottom(fx_max,fy_max,fx_min,fy_max,strokeColor,thickness,LineAlgorithm::BRESENHAM);
    bottom.draw(canvas);
    LineSegment left(fx_min,fy_max,fx_min,fy_min,strokeColor,thickness,LineAlgorithm::BRESENHAM);
    left.draw(canvas);


}