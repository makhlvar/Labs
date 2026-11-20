#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include <algorithm>

#include "../include/Image.hpp"
#include "../include/Canvas.hpp"
#include "../include/Rectangle.hpp"
#include "../include/NestedSquares.hpp"
#include "../include/LineSegment.hpp"
#include "../include/Polygon.hpp"
#include "../include/FloodFill.hpp"
#include "../include/Variant.hpp"

// Вспомогательная функция для ввода цвета
Color inputColor(const std::string& prompt) {
    int r, g, b;
    std::cout << prompt << " (0..255 0..255 0..255): ";
    if (!(std::cin >> r >> g >> b)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Неверный формат цвета! Нужно 3 числа через пробел.");
    }
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        throw std::invalid_argument("Значения цвета должны быть от 0 до 255!");
    }
    return Color::from_bytes(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b));
}

// Вспомогательная функция для сохранения файла
void saveImage(Canvas& canvas, const std::string& defaultName) {
    int format;
    std::cout << "Формат файла (1 - PPM P3, 2 - PGM P2): ";
    std::cin >> format;
    
    std::string filename;
    std::cout << "Имя файла для сохранения (например, " << defaultName << "): ";
    std::cin >> filename;

    std::string outDir = "../output/";
    
    if (format == 1) {
        canvas.exportPPMP3(outDir + filename);
    } else if (format == 2) {
        canvas.exportPGMP2(outDir + filename);
    } else {
        throw std::invalid_argument("Неверный формат файла!");
    }
    std::cout << "[Успех]: Данные успешно записаны в файл " << outDir << filename << "\n";
}

int main() {
    try {
        int choice;
        while (true) {
            std::cout << "\n=== Графический генератор Netpbm (C++17 Engine) ===\n";
            std::cout << "1. Отрисовка прямоугольника\n";
            std::cout << "2. Отрисовка вложенных квадратов\n";
            std::cout << "3. Отрезок: алгоритм Ву / Толстый Брезенхем\n";
            std::cout << "4. Алгоритм заливки области (Flood Fill) и Полигон\n";
            std::cout << "5. Вариант 2: Процедурная доска с плитами Трюше\n";
            std::cout << "0. Выход из программы\n";
            std::cout << "Выберите пункт меню: ";

            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Введено не число!");
            }
            if (choice == 0) break;
            if (choice < 1 || choice > 5) {
                std::cout << "Неверный пункт меню.\n";
                continue;
            }

    int w, h;
        std::cout << "\nШирина холста: ";
    if (!(std::cin >> w)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Введено не число вместо ширины!");
    }

    std::cout << "Высота холста: ";
    if (!(std::cin >> h)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Введено не число вместо высоты!");
    }

    if (w <= 0 || h <= 0) throw std::invalid_argument("Размеры холста должны быть > 0");

            // Создаем холст
            Image img(w, h);
            Canvas canvas(img);

            // Выбор цвета фона
            Color bgColor = inputColor("Цвет фона RGB");
            for (int y = 0; y < h; ++y)
                for (int x = 0; x < w; ++x)
                    img.getPixel(x, y) = bgColor;

            switch (choice) {
                case 1: {
                    float cx, cy, rw, rh, th;
                    std::cout << "Центр прямоугольника (cx cy): "; std::cin >> cx >> cy;
                    std::cout << "Ширина и высота прямоугольника (W H): "; std::cin >> rw >> rh;
                    std::cout << "Толщина контура: "; std::cin >> th;
                    
                    Color stroke = inputColor("Цвет контура RGB");
                    Color fill = inputColor("Цвет заливки RGB");
                    
                    int isFilled;
                    std::cout << "Заливать? (1 - да, 0 - нет): "; std::cin >> isFilled;

                    Rectangle r(cx, cy, rw, rh, stroke, fill, th, isFilled == 1);
                    r.draw(canvas);
                    saveImage(canvas, "rectangle.ppm");
                    break;
                }
                case 2: {
                    float cx, cy, S, d; int N;
                    std::cout << "Центр (cx cy): "; std::cin >> cx >> cy;
                    std::cout << "Начальный размер стороны S: "; std::cin >> S;
                    std::cout << "Количество квадратов N: "; std::cin >> N;
                    std::cout << "Шаг уменьшения стороны d: "; std::cin >> d;
                    
                    Color c_start = inputColor("Начальный цвет RGB");
                    Color c_end = inputColor("Конечный цвет RGB");

                    NestedSquares ns(cx, cy, S, N, d, c_start, c_end);
                    ns.draw(canvas);
                    saveImage(canvas, "nested.pgm");
                    break;
                }
                case 3: {
                    float x0, y0, x1, y1, th; int algo;
                    std::cout << "Координаты точки A (x0 y0): "; std::cin >> x0 >> y0;
                    std::cout << "Координаты точки B (x1 y1): "; std::cin >> x1 >> y1;
                    std::cout << "Выберите алгоритм (1 - Ву [Anti-Aliased], 2 - Брезенхем [Толщина]): "; std::cin >> algo;
                    
                    if (algo == 2) {
                        std::cout << "Толщина линии: "; std::cin >> th;
                    } else {
                        th = 1.0f; // Для Ву толщина всегда 1 (пиксель)
                    }

                    Color lineColor = inputColor("Цвет линии RGB");

                    LineSegment l(x0, y0, x1, y1, lineColor, th, (algo == 1) ? LineAlgorithm::WU : LineAlgorithm::BRESENHAM);
                    l.draw(canvas);
                    saveImage(canvas, (algo == 1) ? "line_wu.ppm" : "line_bresenham.pgm");
                    break;
                }
                case 4: {
                    // Полигон
                    int numPoints;
                    std::cout << "Количество вершин полигона: "; std::cin >> numPoints;
                    std::vector<std::pair<float, float>> pts;
                    for (int i = 0; i < numPoints; ++i) {
                        float px, py;
                        std::cout << "Вершина " << i + 1 << " (x y): "; std::cin >> px >> py;
                        pts.push_back({px, py});
                    }
                    float th;
                    std::cout << "Толщина контура: "; std::cin >> th;
                    Color lineColor = inputColor("Цвет контура RGB");
                    
                    Polygon poly(pts, lineColor, th);
                    poly.draw(canvas);

                    // Заливка
                    float seedX, seedY, eps;
                    std::cout << "Точка затравки для FloodFill (x y): "; std::cin >> seedX >> seedY;
                    std::cout << "Допуск цвета (epsilon, например 0.1): "; std::cin >> eps;
                    Color fillColor = inputColor("Цвет заливки RGB");

                    FloodFill::fill(img, static_cast<int>(seedX), static_cast<int>(seedY), fillColor, eps);
                    saveImage(canvas, "floodfill.ppm");
                    break;
                }
                case 5: {
                    int N, M;
                    std::cout << "Количество клеток по X (N): "; std::cin >> N;
                    std::cout << "Количество клеток по Y (M): "; std::cin >> M;
                    Color c0 = inputColor("Базовый цвет 1 RGB");
                    Color c1 = inputColor("Базовый цвет 2 RGB");

                    Variant2Truchet::generate(img, N, M, c0, c1);
                    saveImage(canvas, "truchet.ppm");
                    break;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "\n[Ошибка]: " << e.what() << "\n";
        return 1;
    }
    return 0;
}