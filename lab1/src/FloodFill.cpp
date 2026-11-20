#include "../include/FloodFill.hpp"
#include <queue>
#include <stdexcept> 

void FloodFill::fill(Image& img, int seedX, int seedY, Color fillColor, float epsilon) {
    // Если точка затравки вне холста — кидаем исключение!
    if (seedX < 0 || seedX >= img.getWidth() || seedY < 0 || seedY >= img.getHeight()) {
        throw std::out_of_range("Точка затравки FloodFill находится за пределами холста!");
    }

    // Запоминаем цвет, который будем заменять
    Color targetColor = img.getPixel(seedX, seedY);

    // Если уже нужный цвет — ничего не делаем
    if (targetColor.distance(fillColor) <= epsilon) return;

    // Нерекурсивная заливка по принципу BFS (Breadth-First Search)
    // std::queue вместо std::stack, чтобы область росла "волной" и памяти тратилось меньше
    std::queue<std::pair<int, int>> q;//координаты заливаемых точек
    q.push({seedX, seedY});//начало заливки

    while (!q.empty()) {
       // auto [x, y] = q.front();
       std::pair<int,int> xy = q.front();
       int x = xy.first;
       int y = xy.second;
        q.pop();

        if (img.getPixel(x, y).distance(targetColor) <= epsilon) {//сравниваем цвет пикскля и наш
            img.getPixel(x, y) = fillColor;

            // 4 соседа
            if (x > 0) q.push({x - 1, y});
            if (x < img.getWidth() - 1) q.push({x + 1, y});
            if (y > 0) q.push({x, y - 1});
            if (y < img.getHeight() - 1) q.push({x, y + 1});
        }
    }
}