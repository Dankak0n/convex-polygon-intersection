#ifndef TRIANGLES_SRC_POINT_H
#define TRIANGLES_SRC_POINT_H

#include <initializer_list>
#include <utility>

#include "helpers.h"

namespace geometry {

class Point {
private:
    real_t x {};
    real_t y {};
public:
    explicit Point();
    explicit Point(real_t _x, real_t _y);
    explicit Point(std::initializer_list<real_t> _i_list);
    Point(const Point& _other);
    ~Point() = default;

    Point& operator= (const Point& _other);
    bool   operator==(const Point& _other) const;
    bool   operator< (const Point& _other) const;
    operator std::pair<real_t, real_t>() const;
    
    real_t getX() const;
    real_t getY() const;

    void setX(real_t _x);
    void setY(real_t _y);
    void setCoords(real_t _x, real_t _y);

    real_t getDistance(const Point& _other) const;
    real_t getNorm() const;
    real_t getCrossProduct(const Point& _other) const;
    Point        getVector(const Point& _other) const;
};

using Vector2 = Point;

}

namespace helpers {
inline bool isClockWise(geometry::Point a, geometry::Point b, geometry::Point c) {
    return helpers::_lessreal_t(a.getX() * (b.getY() - c.getY()) +
                                b.getX() * (c.getY() - a.getY()) +
                                c.getX() * (a.getY() - b.getY()),
                                0);
}
inline bool isAntiClockWise(geometry::Point a, geometry::Point b, geometry::Point c) {
    return helpers::_greaterreal_t(a.getX() * (b.getY() - c.getY()) +
                                   b.getX() * (c.getY() - a.getY()) +
                                   c.getX() * (a.getY() - b.getY()),
                                   0);
}
void swap(geometry::Point& a, geometry::Point& b);
}

#endif //TRIANGLES_SRC_POINT_H