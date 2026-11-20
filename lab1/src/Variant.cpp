#include "../include/Variant.hpp"
#include "../include/Canvas.hpp"
#include "../include/LineSegment.hpp"
#include <cmath>

void Variant2Truchet::generate(Image& img, int N, int M, Color c0, Color c1) {
    Canvas canvas(img);
    int cellW = img.getWidth() / N;
    int cellH = img.getHeight() / M;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            int startX = i * cellW;
            int startY = j * cellH;

            // 1. Шахматная заливка фона ячейки
            Color bgColor = ((i + j) % 2 == 0) ? c0 : c1;
            for (int y = 0; y < cellH; ++y)
                for (int x = 0; x < cellW; ++x)
                    img.getPixel(startX + x, startY + y) = bgColor;

            // 2. Ориентация по правилу из ТЗ: (i * j) % 2
            bool orientation = ((i * j) % 2 == 0);
            Color lineColor = Color::from_bytes(255, 255, 255);

            // 3. Дуги аппроксимируем короткими линиями Ву — будет гладко
            const int segments = 15; // Чем больше — тем глаже дуга
            float R = static_cast<float>(cellW) / 2.0f;

            if (orientation) {
                // Дуги из левого верхнего и правого нижнего углов
                float cx1 = static_cast<float>(startX), cy1 = static_cast<float>(startY);
                float cx2 = static_cast<float>(startX + cellW), cy2 = static_cast<float>(startY + cellH);

                for (int s = 0; s < segments; ++s) {
                    float a1 = (static_cast<float>(s) / segments) * 1.5708f;     // 0..PI/2
                    float a2 = (static_cast<float>(s + 1) / segments) * 1.5708f;

                    LineSegment seg1(cx1 + R * std::cos(a1), cy1 + R * std::sin(a1),
                                     cx1 + R * std::cos(a2), cy1 + R * std::sin(a2),
                                     lineColor, 1.0f, LineAlgorithm::WU);
                    seg1.draw(canvas);

                    LineSegment seg2(cx2 - R * std::cos(a1), cy2 - R * std::sin(a1),
                                     cx2 - R * std::cos(a2), cy2 - R * std::sin(a2),
                                     lineColor, 1.0f, LineAlgorithm::WU);
                    seg2.draw(canvas);
                }
            } else {
                // Дуги из правого верхнего и левого нижнего углов
                float cx1 = static_cast<float>(startX + cellW), cy1 = static_cast<float>(startY);
                float cx2 = static_cast<float>(startX),         cy2 = static_cast<float>(startY + cellH);

                for (int s = 0; s < segments; ++s) {
                    float a1 = (static_cast<float>(s) / segments) * 1.5708f;
                    float a2 = (static_cast<float>(s + 1) / segments) * 1.5708f;

                    LineSegment seg1(cx1 - R * std::cos(a1), cy1 + R * std::sin(a1),
                                     cx1 - R * std::cos(a2), cy1 + R * std::sin(a2),
                                     lineColor, 1.0f, LineAlgorithm::WU);
                    seg1.draw(canvas);

                    LineSegment seg2(cx2 + R * std::cos(a1), cy2 - R * std::sin(a1),
                                     cx2 + R * std::cos(a2), cy2 - R * std::sin(a2),
                                     lineColor, 1.0f, LineAlgorithm::WU);
                    seg2.draw(canvas);
                }
            }
        }
    }
}