#ifndef TRIANGLES_SRC_POLYGON_H
#define TRIANGLES_SRC_POLYGON_H

#include <vector>
#include <initializer_list>
#include <utility>

#include "point.h"
#include "helpers.h"

namespace geometry {

class Triangle;

class ConvexPolygon {
private:
    std::vector<geometry::Point> points_list {};
public:
    explicit ConvexPolygon();
    explicit ConvexPolygon(uint32_t _other);
    explicit ConvexPolygon(std::vector<geometry::Point> _points_list);
    explicit ConvexPolygon(std::initializer_list<geometry::Point> _i_list);
    ConvexPolygon(const ConvexPolygon& _other);
    virtual ~ConvexPolygon() = default;

    ConvexPolygon&  operator= (const ConvexPolygon& _other);
    geometry::Point operator[](uint32_t _pos) const;
    bool            operator==(const ConvexPolygon& _other);
    
    size_t size() const;
    std::vector<geometry::Point> getAllPoints() const;
    void setAllPoints(std::vector<geometry::Point> _new_points_list);
    void setPointAt(size_t id, const geometry::Point& point);
    geometry::Point getPointAt(uint32_t _pos) const;
    ConvexPolygon intersectWithTriangle(const geometry::Triangle& other) const;
    bool isInside(const geometry::Point& _point) const;
    virtual real_t area() const;
    void addPoint(const geometry::Point& _point);
};

ConvexPolygon getIntersection (const ConvexPolygon& polygon,
                               const geometry::Triangle& triangle);
ConvexPolygon getIntersection (const geometry::Triangle& triangle,
                               const ConvexPolygon& polygon);
ConvexPolygon getIntersection (const ConvexPolygon& polygon1,
                               const ConvexPolygon& polygon2);
}

#endif //TRIANGLES_SRC_POLYGON_H