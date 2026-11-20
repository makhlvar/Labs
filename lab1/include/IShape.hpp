#pragma once
class Canvas;

class IShape {
public:
    virtual ~IShape() = default; // Виртуальный деструктор  обязателен для полиморфизма
    virtual void draw(Canvas& canvas) const = 0; // Чисто виртуальный метод
};