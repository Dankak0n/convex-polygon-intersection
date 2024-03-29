#include <initializer_list>
#include <utility>
#include <cmath>
#include <limits>

#include "helpers.h"
#include "point.h"

namespace geometry {

Point::Point()
{}

Point::Point(real_t _x, real_t _y) :
x {_x}, y {_y} {}

Point::Point(std::initializer_list<real_t> _i_list) :
x { *_i_list.begin() }, y { *(_i_list.begin() + 1) } {}

Point::Point(const Point& _other) :
x {_other.x}, y {_other.y} {}

Point& Point::operator= (const Point& _other) {
    x = _other.x;
    y = _other.y;
    return *this;
}
bool Point::operator< (const Point& _other) const {
    if (!helpers::_equalreal_t(x, _other.x)) {
        return helpers::_lessreal_t(x, _other.x);
    }
    return helpers::_lessreal_t(y, _other.y);
}
bool Point::operator==(const Point& _other) const {
    return helpers::_equalreal_t(x, _other.x) && helpers::_equalreal_t(y, _other.y);
}
Point::operator std::pair<real_t, real_t>() const {
    return std::pair<real_t, real_t>{x, y};
}

real_t Point::getX() const { return x; }
real_t Point::getY() const { return y; }

void Point::setX(real_t _x) { x = _x; }
void Point::setY(real_t _y) { y = _y; }
void Point::setCoords(real_t _x, real_t _y) {
    x = _x; y = _y;
}
real_t Point::getDistance(const Point& _other) const {
    return std::sqrt( std::pow(x - _other.x, 2) + std::pow(y - _other.y, 2) );
}
real_t Point::getNorm() const {
    return this->getDistance( Point{0, 0} );
}
real_t Point::getCrossProduct(const Point& _other) const {
    return x * _other.y - _other.x * y;
}
Point Point::getVector(const Point& _other) const {
    return Point{_other.x - x, _other.y - y};
}

}

namespace helpers {

void swap(geometry::Point& a, geometry::Point& b) {
    geometry::Point tmp = a;
    a = b;
    b = tmp;
}

}