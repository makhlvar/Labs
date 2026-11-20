#include "../include/LineSegment.hpp"
#include "../include/Canvas.hpp"
#include <cmath>
#include <algorithm>

//В списке инициализации каждому полю класса присваивается значение из соответствующего аргумента.
LineSegment::LineSegment(float x0, float y0, float x1, float y1,
                         Color color, float thickness, LineAlgorithm algo)
    : x0(x0), y0(y0), x1(x1), y1(y1),
      color(color), thickness(thickness), algorithm(algo) {}


 //принимает холст по ссылке (чтобы рисовать на оригинале, а не на копии)     
void LineSegment::draw(Canvas& canvas) const {
    if (algorithm == LineAlgorithm::WU) drawWu(canvas);//Простой if: если пользователь выбрал алгоритм Ву
    // вызываем drawWu. Иначе drawThickBresenham
    //Это и есть полиморфизм в действии: снаружи один метод draw, а внутри выбирается нужный алгоритм
    else drawThickBresenham(canvas);
}

//Алг Брезенхема для толстой линии
//обычный Брезенхем для центральной линии
// На каждом шаге также рисуем "профиль" те отрезок перпендикулярно направлению шириной thickness
//Это дает толстую линию без разрывов
void LineSegment::drawThickBresenham(Canvas& canvas) const {
    if (thickness < 1.0f) return;
    canvas.setAntialiasing(false); // Для сплошной толстой линии не нужно сглаживание

    int ix0 = static_cast<int>(std::round(x0));
    int iy0 = static_cast<int>(std::round(y0));
    int ix1 = static_cast<int>(std::round(x1));
    int iy1 = static_cast<int>(std::round(y1));

    int dx = std::abs(ix1 - ix0);
    int dy = std::abs(iy1 - iy0);
    int sx = (ix0 < ix1) ? 1 : -1;
    int sy = (iy0 < iy1) ? 1 : -1;
    int err = dx - dy; //число счетчик которое помогает делать в нужное время шаг вверх
    //                   а не только в сторону

    float len = std::sqrt(static_cast<float>(dx * dx + dy * dy));
    if (len == 0.0f) {
        // Нулевая линия — рисуем одну точку
        canvas.setPixel(ix0, iy0, color);
        return;
    }

    //Нормаль к линии вдоль нее двигаемся для отрисовки толщины
    float nx = -dy / len;
    float ny =  dx / len;
    float half_t = (thickness - 1.0f) / 2.0f;

    int steps = std::max(dx, dy); // Сколько раз шагнуть по главной оси

    for (int i = 0; i <= steps; ++i) {
        //Рисуем перпендикулярный профиль
        int x_start = static_cast<int>(std::round(ix0 - half_t * nx));
        int y_start = static_cast<int>(std::round(iy0 - half_t * ny));
        int x_end   = static_cast<int>(std::round(ix0 + half_t * nx));
        int y_end   = static_cast<int>(std::round(iy0 + half_t * ny));

        int pdx = std::abs(x_end - x_start);
        int pdy = std::abs(y_end - y_start);
        int psx = (x_start < x_end) ? 1 : -1;
        int psy = (y_start < y_end) ? 1 : -1;
        int perr = pdx - pdy;
        int px = x_start, py = y_start;
        int psteps = std::max(pdx, pdy);

        for (int j = 0; j <= psteps; ++j) {
            canvas.setPixel(px, py, color);
            int pe2 = 2 * perr;
            if (pe2 > -pdy) { perr -= pdy; px += psx; }
            if (pe2 <  pdx) { perr += pdx; py += psy; }
        }

        //Шаг основной линии
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; ix0 += sx; }
        if (e2 <  dx) { err += dx; iy0 += sy; }
    }
}

//By слаженная линия
//для каждого шага по X красим два соседних пикселя по Y
//с прозрачностью, вычисленной по дробной части.Так края линии "размазываются".
void LineSegment::drawWu(Canvas& canvas) const {
    float x0_ = x0, y0_ = y0, x1_ = x1, y1_ = y1;

    // Если линия крутая (dy > dx), мы меняем оси местами,
    // чтобы итерироваться по той оси, где больше шагов.
    const bool steep = std::abs(y1_ - y0_) > std::abs(x1_ - x0_);
    if (steep) {
        std::swap(x0_, y0_);
        std::swap(x1_, y1_);
    }
    if (x0_ > x1_) {
        // Гарантируем, что рисуем слева направо
        std::swap(x0_, x1_);
        std::swap(y0_, y1_);
    }

    float dx = x1_ - x0_;
    float dy = y1_ - y0_;
    float gradient = (dx == 0.0f) ? 1.0f : dy / dx;

    // Дробная и "обратная дробная" часть для расчета прозрачности
    auto fpart  = [](float v) { return v - std::floor(v); };
    auto rfpart = [&](float v) { return 1.0f - fpart(v); };

    //Первая точка (с учетом xgap, чтобы конец был ровным)
    int xend = static_cast<int>(std::round(x0_));
    float yend = y0_ + gradient * (xend - x0_);
    float xgap = rfpart(x0_ + 0.5f);
    int xpxl1 = xend;
    int ypxl1 = static_cast<int>(std::floor(yend));

    if (steep) {
        canvas.mixPixel(ypxl1,     xpxl1, color, rfpart(yend) * xgap);
        canvas.mixPixel(ypxl1 + 1, xpxl1, color,  fpart(yend) * xgap);
    } else {
        canvas.mixPixel(xpxl1, ypxl1,     color, rfpart(yend) * xgap);
        canvas.mixPixel(xpxl1, ypxl1 + 1, color,  fpart(yend) * xgap);
    }

    float intery = yend + gradient; //Y для следующего шага

    //Вторая точка аналогично первой
    xend = static_cast<int>(std::round(x1_));
    yend = y1_ + gradient * (xend - x1_);
    xgap = fpart(x1_ + 0.5f);
    int xpxl2 = xend;
    int ypxl2 = static_cast<int>(std::floor(yend));

    if (steep) {
        canvas.mixPixel(ypxl2,     xpxl2, color, rfpart(yend) * xgap);
        canvas.mixPixel(ypxl2 + 1, xpxl2, color,  fpart(yend) * xgap);
    } else {
        canvas.mixPixel(xpxl2, ypxl2,     color, rfpart(yend) * xgap);
        canvas.mixPixel(xpxl2, ypxl2 + 1, color,  fpart(yend) * xgap);
    }

    //Основной цикл
    if (steep) {
        for (int x = xpxl1 + 1; x < xpxl2; ++x) {
            int ipart = static_cast<int>(std::floor(intery));
            canvas.mixPixel(ipart,     x, color, rfpart(intery));
            canvas.mixPixel(ipart + 1, x, color,  fpart(intery));
            intery += gradient;
        }
    } else {
        for (int x = xpxl1 + 1; x < xpxl2; ++x) {
            int ipart = static_cast<int>(std::floor(intery));
            canvas.mixPixel(x, ipart,     color, rfpart(intery));
            canvas.mixPixel(x, ipart + 1, color,  fpart(intery));
            intery += gradient;
        }
    }
}