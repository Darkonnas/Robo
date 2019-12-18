#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include "Music.h"

class Vector2D {
    int8_t x_, y_;
  public:
    Vector2D(const int8_t& x = -4, const int8_t& y = 0): x_(x), y_(y) {}
    Vector2D(const Vector2D& Obj): x_(Obj.x_), y_(Obj.y_) {}
    Vector2D& operator=(const Vector2D& Obj) {
      if (this == &Obj)
        return *this;
      x_ = Obj.x_;
      y_ = Obj.y_;
      return *this;
    }
    inline int8_t& x() {
      return x_;
    }
    inline int8_t& y() {
      return y_;
    }
};

class Tile {
    Pitches Pitch_;
    Vector2D Position_, Size_;
  public:
    Tile(const Pitches& Pitch =  Pitches::Null, const Vector2D& Position = Vector2D(), const Vector2D& Size = Vector2D()): Pitch_(Pitch), Position_(Position), Size_(Size) {}
    Tile& operator=(const Tile& Obj) {
      if (this == &Obj)
        return *this;
      Pitch_ = Obj.Pitch_;
      Position_ = Obj.Position_;
      Size_ = Obj.Size_;
      return *this;
    }
    inline Pitches& Pitch() {
      return Pitch_;
    }
    inline Vector2D& Position() {
      return Position_;
    }
    inline Vector2D& Size() {
      return Size_;
    }
};

#endif
