#include "../include/Canvas.hpp"
#include <fstream>
#include <stdexcept>

Canvas::Canvas(Image& image) : img(image), antialiasingEnabled(false) {
    penColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
}

void Canvas::setPixel(int x, int y, const Color& c) {
    if (x >= 0 && x < img.getWidth() && y >= 0 && y < img.getHeight()) {
        //x >= 0 и y >= 0  отсекают всё, что находится левее и выше области рисования
        //x < img.getWidth() и y < img.getHeight() отсекают всё, что выходит за правую и нижнюю границы
        //Если координаты не попадают в этот диапазон,функция просто игнорирует пиксель,защищая память от выхода за пределы массива
        if (antialiasingEnabled) {
            img.getPixel(x, y) = img.getPixel(x, y).blend(c);
        } else {
            img.getPixel(x, y) = c;
        }
    }
}

void Canvas::mixPixel(int x, int y, const Color& c, float alpha) {
    if (x >= 0 && x < img.getWidth() && y >= 0 && y < img.getHeight()) {
        Color blendedColor = c;
        blendedColor.a = alpha;
        img.getPixel(x, y) = img.getPixel(x, y).blend(blendedColor);
    }
}

void Canvas::exportPGMP2(std::string f) {
    std::ofstream out(f);
    if (!out) throw std::runtime_error("Can't open file: " + f);
    PGMExporter exporter(img);
    exporter.exportTo(out);
}

void Canvas::exportPPMP3(std::string f) {
    std::ofstream out(f);
    if (!out) throw std::runtime_error("Can't open file: " + f);
    PPMExporter exporter(img);
    exporter.exportTo(out);
}