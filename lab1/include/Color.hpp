#pragma once
#include <cstdint>

struct Color {
    //Каналы цветов ржб и прозрачность(альфа)
    float r, g, b, a;
    
    //конструкторы
    Color();
    Color(float r, float g, float b, float a = 1.0f);
    
    //Создание цвета из байтов(0-255)
    //Удобно для интуитивного задания цветов
    static Color from_bytes(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
    
    //Перевод в градацию серого
    uint8_t to_grayscale() const;
    
    //Создания градиента
    Color lerp(const Color& target, float t) const;
    
    //Альфа смешивание(наложение src поверх текущего цвета)
    Color blend(const Color& src) const;
    
    //Евклидово расстояние между цветами для Floodfill
    float distance(const Color& other) const;
};