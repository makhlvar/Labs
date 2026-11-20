#pragma once
#include "IShape.hpp"
#include "Color.hpp"

// Перечисление для выбора алгоритма отрисовки
enum class LineAlgorithm {
    WU,         // Сглаженная линия (Алгоритм Ву)
    BRESENHAM   // Толстая линия (Модифицированный Брезенхем)
};

class LineSegment : public IShape {
private:
    float x0, y0, x1, y1;      // Координаты концов
    Color color;               // Цвет линии
    float thickness;           // Толщина линии
    LineAlgorithm algorithm;   // Выбранный алгоритм

    // Вспомогательные методы
    void drawWu(Canvas& canvas) const;
    void drawThickBresenham(Canvas& canvas) const;

public:
    LineSegment(float x0, float y0, float x1, float y1,
                Color color, float thickness, LineAlgorithm algo);
    void draw(Canvas& canvas) const override;
};