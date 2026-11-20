#include "../include/NestedSquares.hpp"
#include "../include/Rectangle.hpp"
#include "../include/Canvas.hpp"

NestedSquares::NestedSquares(float cx, float cy, float S, int N, float d,
                             Color c_start, Color c_end)
    : cx(cx), cy(cy), S(S), N(N), d(d), c_start(c_start), c_end(c_end) {}

void NestedSquares::draw(Canvas& canvas) const {
    for (int i = 0; i < N; ++i) {
        // S_i = S - 2*i*d.Умножение на 2
        // потому что квадрат уменьшается с двух сторон одновременно
        float S_i = S - 2.0f * static_cast<float>(i) * d;
        if (S_i <= 0.0f) break; // Условие из ТЗ: прекращаем генерацию

        // Коэффициент для интерполяции цвета
        // Если квадрат всего один, делить на N-1 нельзя берем 0.
      float t =(N > 1) ? static_cast<float>(i)/(N-1) : 0.0f;//вычисление коэффициента
    //для интерполяции цвета
    //если квадратов больше 1 (для первого даст 0.0 для последнего 1.0)
    //статик каст чтобы не потерять дробную часть при делении на инт
    //если квадрат всего 1 делить на 0 нельзя поэтому t = 0.0f
        // Интерполяция цвета: при t=0  c_start, при t=1  c_end
        Color currentColor = c_start.lerp(c_end, t);

        // Создаем временный прямоугольник и рисуем
        Rectangle rect(cx, cy, S_i, S_i, currentColor, currentColor,1.0f,true);
        rect.draw(canvas);
    }
}