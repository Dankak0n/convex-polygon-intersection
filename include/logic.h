#ifndef TRIANGLES_SRC_LOGIC_H
#define TRIANGLES_SRC_LOGIC_H

#include <vector>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "point.h"
#include "polygon.h"
#include "triangle.h"

enum status_t {NOTHING, CREATING, GRABBING, CATCHING};
enum mouse_status_t {RELEASED, PRESSED};

class Logic {
private:
    std::vector<geometry::ConvexPolygon> polygons {};
    geometry::ConvexPolygon intersection_polygon {};
    geometry::ConvexPolygon being_drawn_polygon {};
    geometry::ConvexPolygon catch_polygon {};
    geometry::Point first_drawn_point {};
    status_t status = NOTHING;
    mouse_status_t mouse_status = RELEASED;
    bool is_debug = false;
    int32_t grabbed_polygon_id {-1};
    int32_t catch_vertex_id {-1};
public:
    Logic(bool _is_debug = false) : is_debug(_is_debug) {}
    Logic() = default;
    ~Logic() = default;

    void update_status();
    void debug();
    void justDraw(sf::RenderWindow& window);
    void addingTriangle();
    void grabbingTriangle();
    void renew();
    void start();
};

#endif //TRIANGLES_SRC_LOGIC_H