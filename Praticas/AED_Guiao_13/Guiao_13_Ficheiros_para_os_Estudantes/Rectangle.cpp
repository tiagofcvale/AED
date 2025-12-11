//
// Algoritmos e Estruturas de Dados - 2024/2025
//
// J. Madeira - April/May 2022
//
// COMPLETE the code, according to Rectangle.h
//

#include "Rectangle.h"

#include <cassert>
#include <iostream>
#include <string>

#include "Point.h"

Rectangle::Rectangle(void) 
  : width_(1.0), height_(1.0), Figure(Point(0.0,0.0),"black"){
}

Rectangle::Rectangle(Point center, const std::string& color, double width,
                     double height)
                      : Figure(center,color), height_(height > 0.0 ? height : 1.0), width_(width > 0.0 ? width : 1.0) {
}

Rectangle::Rectangle(double x, double y, const std::string& color, double width,
                     double height) 
                      : Figure(Point(x,y),color), height_(height > 0.0 ? height : 1.0), width_(width > 0.0 ? width : 1.0) {
}

double Rectangle::GetHeight(void) const { return height_; }
void Rectangle::SetHeight(double length) {
  height_ = (length > 0.0 ? length : 1.0);
}

double Rectangle::GetWidth(void) const { return width_; }
void Rectangle::SetWidth(double length) {
  width_ = (length > 0.0 ? length : 1.0);
}

std::string Rectangle::GetClassName(void) const { return "Rectangle"; }

double Rectangle::Area(void) const {
  return width_ * height_;
}

double Rectangle::Perimeter(void) const {
  return width_ * 2 + height_ * 2; 
}

std::ostream& operator<<(std::ostream& os, const Rectangle& obj) {
    os << obj.GetCenter().GetX() << " "
       << obj.GetCenter().GetY() << " "
       << obj.GetColor() << " "
       << obj.width_ << " "
       << obj.height_;

    return os;
}

std::istream& operator>>(std::istream& is, Rectangle& obj) {
    double x, y, w, h;
    std::string color;

    // Ler valores pela mesma ordem em que os escrevemos
    is >> x >> y >> color >> w >> h;

    // Atualizar o objeto
    obj.SetCenter(Point(x, y));
    obj.SetColor(color);

    // Garantir dimensões válidas
    obj.width_  = (w > 0.0 ? w : 1.0);
    obj.height_ = (h > 0.0 ? h : 1.0);

    return is;
}
