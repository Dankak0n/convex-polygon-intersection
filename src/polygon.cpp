#include <numeric>
#include <algorithm>
#include <vector>
#include <initializer_list>
#include <utility>
#include <iterator>

#include "polygon.h"
#include "point.h"
#include "helpers.h"
#include "triangle.h"

namespace geometry {

using Vector2 = Point;

ConvexPolygon::ConvexPolygon()
{}

ConvexPolygon::ConvexPolygon(uint32_t _other) :
points_list(_other) {}

ConvexPolygon::ConvexPolygon(std::vector<geometry::Point> _points_list) {
    setAllPoints(_points_list);
}

ConvexPolygon::ConvexPolygon(std::initializer_list<geometry::Point> _i_list) :
ConvexPolygon( std::vector<geometry::Point>(_i_list.begin(), _i_list.end()) ) {}

ConvexPolygon::ConvexPolygon(const ConvexPolygon& _other) {
    points_list = _other.getAllPoints();
}

ConvexPolygon& ConvexPolygon::operator= (const ConvexPolygon& _other) {
    points_list = _other.getAllPoints();
    return *this;
}
geometry::Point ConvexPolygon::operator[](uint32_t _pos) const {
    return points_list[_pos];
}
bool ConvexPolygon::operator==(const ConvexPolygon& _other) {
    if (_other.size() != size()) {
        return false;
    }
    for (size_t i = 0; i < size(); i++) {
        if (!(getPointAt(i) == _other.getPointAt(i))) {
            return false;
        }
    }
    return true;
}

size_t ConvexPolygon::size() const {
    return points_list.size();
}
std::vector<geometry::Point> ConvexPolygon::getAllPoints() const {
    return points_list;
}
geometry::Point ConvexPolygon::getPointAt(uint32_t _pos) const {
    return (*this)[_pos];
}
ConvexPolygon ConvexPolygon::intersectWithTriangle(const geometry::Triangle& other) const {
    return getIntersection(*this, other);
}
bool ConvexPolygon::isInside(const geometry::Point& _point) const {
    if (size() == 0) {
        return false;
    }
    if (size() == 1) {
        return points_list[0] == _point;
    }
    if (size() == 2) {
        real_t min_x = std::min(points_list[0].getX(), points_list[1].getX());
        real_t max_x = std::max(points_list[0].getX(), points_list[1].getX());
        if (helpers::_lessreal_t(_point.getX(), min_x) ||
            helpers::_lessreal_t(max_x, _point.getX())) {
                return false;
            }
        real_t min_y = std::min(points_list[0].getY(), points_list[1].getY());
        real_t max_y = std::max(points_list[0].getY(), points_list[1].getY());
        if (helpers::_lessreal_t(_point.getY(), min_y) ||
            helpers::_lessreal_t(max_y, _point.getY())) {
                return false;
            }
    }
    real_t _area = Triangle(_point, points_list[0], points_list.back()).area();
    for (size_t pos = 1; pos < size(); pos++) {
        _area += Triangle(_point, points_list[pos - 1], points_list[pos]).area();
    }
    return helpers::_equalreal_t(_area, area());
}
real_t ConvexPolygon::area() const {
    if (size() < 3) {
        return 0.0;
    }
    real_t ans = 0.0;
    for (int i = 2; i < size(); i++) {
        ans += Triangle(points_list[0], points_list[i - 1], points_list[i]).area();
    }
    return ans;
}

//Future:
//https://neerc.ifmo.ru/wiki/index.php?title=Динамическая_выпуклая_оболочка_(достаточно_log%5E2_на_добавление/удаление)
void ConvexPolygon::setAllPoints(std::vector<geometry::Point> _new_points_list) {
    std::sort(_new_points_list.begin(), _new_points_list.end());
    auto new_end = std::unique(_new_points_list.begin(), _new_points_list.end());
    _new_points_list.resize( std::distance(_new_points_list.begin(), new_end) );

    if (_new_points_list.size() <= 2ULL) {
        points_list = _new_points_list;
        return;
    }
    Point envelope_left_bound = _new_points_list.front();
    Point envelope_right_bound = _new_points_list.back();

    std::vector<geometry::Point> up_polyline_envelope(1, envelope_left_bound);
    std::vector<geometry::Point> down_polyline_envelope(1, envelope_left_bound);

    for (size_t i = 1; i < _new_points_list.size(); i++) {
        if (i == _new_points_list.size() - 1 ||
            helpers::isClockWise(envelope_left_bound, _new_points_list[i], envelope_right_bound)) {
            while (up_polyline_envelope.size() >= 2 && !helpers::isClockWise(up_polyline_envelope[up_polyline_envelope.size() - 2],
                                                                    up_polyline_envelope.back(),
                                                                    _new_points_list[i])) {
                up_polyline_envelope.pop_back();
            }
            up_polyline_envelope.push_back(_new_points_list[i]);
        }
        if (i == _new_points_list.size() - 1 ||
            helpers::isAntiClockWise(envelope_left_bound, _new_points_list[i], envelope_right_bound)) {
            while (down_polyline_envelope.size() >= 2 && !helpers::isAntiClockWise(down_polyline_envelope[down_polyline_envelope.size() - 2],
                                                                          down_polyline_envelope.back(),
                                                                          _new_points_list[i])) {
                down_polyline_envelope.pop_back();
            }
            down_polyline_envelope.push_back(_new_points_list[i]);
        }
    }
    _new_points_list = up_polyline_envelope;
	for (size_t i = down_polyline_envelope.size() - 2; i > 0; i--) {
		_new_points_list.push_back(down_polyline_envelope[i]);
    }
    points_list = _new_points_list;
}

void ConvexPolygon::setPointAt(size_t id, const geometry::Point& point) {
    points_list[id] = point;
}

void ConvexPolygon::addPoint(const geometry::Point& _point) {
    points_list.push_back(_point);
    setAllPoints(points_list);
}

struct InfoIntersection {
    bool status;
    geometry::Point point;
};

InfoIntersection getIntersection(Point s1A, Point s1B, Point s2A, Point s2B) {
    real_t Line1A, Line1B, Line1C;
    real_t Line2A, Line2B, Line2C;

    Line1A = s1B.getY() - s1A.getY();
    Line1B = s1A.getX() - s1B.getX();
    Line1C = -Line1A * s1A.getX() - Line1B * s1A.getY();

    Line2A = s2B.getY() - s2A.getY();
    Line2B = s2A.getX() - s2B.getX();
    Line2C = -Line2A * s2A.getX() - Line2B * s2A.getY();

    if (helpers::_equalreal_t(Line2A * Line1B, Line1A * Line2B)) {
        return {false, Point()};
    }
    real_t point_y = (Line1A * Line2C - Line2A * Line1C) /
                     (Line2A * Line1B - Line1A * Line2B);
    real_t point_x = (!helpers::_equalreal_t(Line1A, 0.0L) ?
                     (-Line1B * point_y - Line1C) / Line1A :
                     (-Line2B * point_y - Line2C) / Line2A);
    Point intersection_point = Point{point_x, point_y};
    if (ConvexPolygon{s1A, s1B}.isInside(intersection_point)) {
        return {true, intersection_point};
    } else {
        return {false, Point()};
    }
}

ConvexPolygon getIntersection (const ConvexPolygon& polygon1,
                               const ConvexPolygon& polygon2) {
    std::vector<geometry::Point> all_point_list = polygon1.getAllPoints();
    for (size_t pos = 0; pos < polygon2.size(); pos++) {
        all_point_list.push_back(polygon2[pos]);
    }
    for (size_t pos_polygon2 = 0; pos_polygon2 < polygon2.size(); pos_polygon2++) {
        Point segment1_a = polygon2[pos_polygon2];
        Point segment1_b = polygon2[ (pos_polygon2 + 1) % polygon2.size() ];
        for (size_t pos_polygon1 = 0; pos_polygon1 < polygon1.size(); pos_polygon1++) {
            Point segment2_a = polygon1[pos_polygon1];
            Point segment2_b = polygon1[ (pos_polygon1 + 1) % polygon1.size() ];
            InfoIntersection info_intersection = getIntersection(segment1_a, segment1_b, segment2_a, segment2_b);
            if (info_intersection.status == true) {
                all_point_list.push_back(info_intersection.point);
            }
        }
    }
    std::vector<geometry::Point> new_point_list;
    for (const geometry::Point& point : all_point_list) {
        if (polygon1.isInside(point) && polygon2.isInside(point)) {
            new_point_list.push_back(point);
        }
    }
    return ConvexPolygon(new_point_list);
}

ConvexPolygon getIntersection (const geometry::Triangle& triangle,
                               const ConvexPolygon& polygon) {
    return getIntersection(polygon, static_cast<ConvexPolygon>(triangle));
}

ConvexPolygon getIntersection (const ConvexPolygon& polygon,
                               const geometry::Triangle& triangle) {
    return getIntersection(polygon, triangle);
}
                               
}