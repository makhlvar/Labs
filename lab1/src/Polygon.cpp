#include "../include/Polygon.hpp"
#include "../include/LineSegment.hpp"

Polygon::Polygon(const std::vector<std::pair<float, float>>& vertices,
                 Color color, float thickness)
    : vertices(vertices), color(color), thickness(thickness) {}

void Polygon::draw(Canvas& canvas) const {
    if (vertices.size() < 2) return;

    for (size_t i = 0; i < vertices.size(); ++i) {
        float x0 = vertices[i].first;
        float y0 = vertices[i].second;

        // Замыкаем многоугольник: (i+1) % size после последней точки даст 0
        size_t next = (i + 1) % vertices.size();
        float x1 = vertices[next].first;
        float y1 = vertices[next].second;

        // Рисуем ребро толстым Брезенхемом
        LineSegment line(x0, y0, x1, y1, color, thickness, LineAlgorithm::BRESENHAM);
        line.draw(canvas);
    }
}