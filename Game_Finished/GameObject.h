#pragma once

struct GameObject {
    int width, height;
    double x, y;
    GameObject(double x, double y, int width, int height) : x(x), y(y), width(width), height(height) {}
};

struct Ball : GameObject {
    double dx, dy; // direction of movement
    bool outOfBounds;
    Ball(double x, double y, int width, int height, double dx, double dy, bool outOfBounds) : GameObject(x, y, width, height), dx(dx), dy(dy), outOfBounds(outOfBounds) {}
};

struct Platform : GameObject {
    double speed;
    Platform(double x, double y, int width, int height, double speed) : GameObject(x, y, width, height), speed(speed) {}
};

struct Brick : GameObject {
    bool destroyed;
    Brick(double x, double y, int width, int height, bool destroyed) : GameObject(x, y, width, height), destroyed(destroyed) {}
};
