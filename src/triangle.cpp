#include <array>
#include <cmath>
#include <utility>

#include "point.h"
#include "helpers.h"
#include "polygon.h"
#include "triangle.h"

namespace geometry {

using Vector2 = Point;

Triangle::Triangle(const geometry::Point& _a,
                   const geometry::Point& _b,
                   const geometry::Point& _c) {
    points_clockwise = {_a, _b, _c};
    if (!helpers::isClockWise(_a, _b, _c)) {
        helpers::swap(points_clockwise[1], points_clockwise[2]);
    }
}

Triangle::Triangle(const std::array<geometry::Point, 3>& _point_array) :
Triangle(_point_array[0], _point_array[1], _point_array[2]) {}

Triangle::operator geometry::ConvexPolygon() const {
    return geometry::ConvexPolygon {points_clockwise[0], points_clockwise[1], points_clockwise[2]};
}

real_t Triangle::area() const {
    Vector2 side_a = points_clockwise[0].getVector(points_clockwise[1]);
    Vector2 side_b = points_clockwise[0].getVector(points_clockwise[2]);
    real_t cross_product = side_a.getCrossProduct(side_b);
    return std::abs(cross_product) / 2.0;
}

}