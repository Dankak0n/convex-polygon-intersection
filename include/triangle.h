#ifndef TRIANGLES_SRC_TRIANGLE_H
#define TRIANGLES_SRC_TRIANGLE_H

#include <array>

#include "point.h"
#include "helpers.h"
#include "polygon.h"

namespace geometry {

class ConvexPolygon;

class Triangle {
    std::array<geometry::Point, 3> points_clockwise;
public:
    explicit Triangle() = default;
    explicit Triangle(const geometry::Point& _a,
                      const geometry::Point& _b,
                      const geometry::Point& _c);
    explicit Triangle(const std::array<geometry::Point, 3>& _points);

    operator geometry::ConvexPolygon() const;
    
    virtual ~Triangle() = default;
    
    real_t area() const;
};

}

#endif //TRIANGLES_SRC_TRIANGLE_H