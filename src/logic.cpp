#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <algorithm>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include "logic.h"
#include "point.h"
#include "polygon.h"
#include "triangle.h"

namespace constants {

int32_t POINT_OFFSET = 6;
int32_t POINT_RADIUS = 5;
sf::Color MAIN_COLOR = sf::Color{ 0, 130, 130 };
sf::Color MAIN_COLOR_TRANSP = sf::Color{ 0, 130, 130, 180 };
sf::Color INTERSECT_COLOR = sf::Color{ 130, 0, 0, 180 };
sf::Color BEING_DRAWN_COLOR = sf::Color{ 130, 130, 0, 180 };
uint32_t WINDOW_H = 600;
uint32_t WINDOW_W = 1000;
uint32_t CELL_SIZE = 60;
uint32_t COLLAPSE_DIST = 20;

}

void Logic::update_status() {
    if (being_drawn_polygon.size() > 0) {
        status = CREATING;
        return;
    }
    if (grabbed_polygon_id != -1) {
        status = GRABBING;
        return;
    }
    if (catch_vertex_id != -1) {
        status = CATCHING;
        return;
    }
    status = NOTHING;
}

bool isIntegerCoords(sf::Vector2i vector) {
    uint32_t dx = vector.x % constants::CELL_SIZE;
    uint32_t dy = vector.y % constants::CELL_SIZE;
    return vector.x >= 0 && (dx < constants::COLLAPSE_DIST || dx > constants::CELL_SIZE - constants::COLLAPSE_DIST) &&
           vector.y >= 0 && (dy < constants::COLLAPSE_DIST || dy > constants::CELL_SIZE - constants::COLLAPSE_DIST);
}

void drawGrid(sf::RenderWindow& window) {
    for (uint32_t i = 0; i <= constants::WINDOW_W; i += constants::CELL_SIZE) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(i, 0),sf::Color::Black),
            sf::Vertex(sf::Vector2f(i, constants::WINDOW_H), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }
    for (uint32_t i = 0; i <= constants::WINDOW_H; i += constants::CELL_SIZE) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, i), sf::Color::Black),
            sf::Vertex(sf::Vector2f(constants::WINDOW_W, i), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }
}

void drawPoint(sf::RenderWindow& window, geometry::Point point, sf::Color color = constants::MAIN_COLOR) {
    sf::CircleShape point_to_draw;
    point_to_draw.setPosition(point.getX() - constants::POINT_OFFSET,
                              point.getY() - constants::POINT_OFFSET);
    point_to_draw.setFillColor(color);
    point_to_draw.setOutlineColor(sf::Color::Black);
    point_to_draw.setOutlineThickness(1);
    point_to_draw.setRadius(constants::POINT_RADIUS);
    window.draw(point_to_draw);
}

void drawPolygon(sf::RenderWindow& window,
                 const geometry::ConvexPolygon& polygon,
                 const sf::Color& fill_color) {
    if (polygon.size() == 2) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(polygon[0].getX(), polygon[0].getY()),sf::Color::Black),
            sf::Vertex(sf::Vector2f(polygon[1].getX(), polygon[1].getY()),sf::Color::Black),
        };
        window.draw(line, 2, sf::Lines);
    } else {
        sf::ConvexShape polygon_to_draw;
        polygon_to_draw.setFillColor(fill_color);
        polygon_to_draw.setPointCount(polygon.size());
        polygon_to_draw.setOutlineColor(sf::Color::Black);
        polygon_to_draw.setOutlineThickness(3);
        for (int i = 0; i < polygon.size(); i++) {
            polygon_to_draw.setPoint(i, sf::Vector2f(polygon.getPointAt(i).getX(),
                                                    polygon.getPointAt(i).getY()));
        }
        window.draw(polygon_to_draw);
    }
    for (int i = 0; i < polygon.size(); i++) {
        drawPoint(window, polygon.getPointAt(i));
    }
}

void drawArea(sf::RenderWindow& window, long double area) {
    area /= std::pow(constants::CELL_SIZE, 2);
    geometry::ConvexPolygon polygon {
        geometry::Point{constants::WINDOW_W - 230.F, constants::WINDOW_H - 90.F},
        geometry::Point{constants::WINDOW_W * 1.F - 30, constants::WINDOW_H - 90.F},
        geometry::Point{constants::WINDOW_W - 230.F, constants::WINDOW_H * 1.F - 30},
        geometry::Point{constants::WINDOW_W * 1.F - 30, constants::WINDOW_H * 1.F - 30},
    };
    drawPolygon(window, polygon, sf::Color::White);

    sf::Text area_text;
    sf::Font font;
    font.loadFromFile("../src/fonts/times.ttf");
    area_text.setString(std::string(std::to_string(area)));
    area_text.setFont(font);
    area_text.setPosition(constants::WINDOW_W - 190, constants::WINDOW_H - 80);
    area_text.setFillColor(sf::Color::Black);
    window.draw(area_text);
}

void Logic::justDraw(sf::RenderWindow& window) {
    drawGrid(window);
    for (geometry::ConvexPolygon polygon : polygons) {
        drawPolygon(window, polygon, constants::MAIN_COLOR_TRANSP);
    }
    drawPolygon(window, intersection_polygon, constants::INTERSECT_COLOR);
    if (being_drawn_polygon.size() > 0) {
        geometry::ConvexPolygon being_drawn_polygon_copy = being_drawn_polygon;
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        geometry::Point mouse_point = geometry::Point{1.F * mouse_pos.x, 1.F * mouse_pos.y};
        being_drawn_polygon_copy.addPoint(mouse_point);
        drawPolygon(window, being_drawn_polygon_copy, constants::BEING_DRAWN_COLOR);
        drawPoint(window, first_drawn_point, sf::Color::Red);
    }
    geometry::ConvexPolygon intersection_polygon_absolute = intersection_polygon;
    if (being_drawn_polygon.size() > 0) {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        geometry::Point mouse_point = geometry::Point{1.F * mouse_pos.x, 1.F * mouse_pos.y};
        geometry::ConvexPolygon p = being_drawn_polygon;
        p.addPoint(mouse_point);
        if (polygons.size() > 0) {
            intersection_polygon_absolute = geometry::getIntersection(intersection_polygon_absolute, p);
        } else {
            intersection_polygon_absolute = p;
        }
    }
    drawArea(window, intersection_polygon_absolute.area());
}
void Logic::renew() {
    if (polygons.size() == 0) {
        return;
    }
    if (polygons.size() == 1) {
        intersection_polygon = polygons[0];
        return;
    }
    geometry::ConvexPolygon new_intersection_polygon = polygons[0];
    for (size_t pos = 1; pos < polygons.size(); pos++) {
        new_intersection_polygon = geometry::getIntersection(new_intersection_polygon, polygons[pos]);
    }
    intersection_polygon = new_intersection_polygon;
}
void Logic::debug() {
    geometry::ConvexPolygon p = intersection_polygon;
    std::cout << "[]: ";
    for (int i = 0; i < p.size(); i++) {
        std::cout << p.getPointAt(i).getX() << ' ' << p.getPointAt(i).getY() << std::endl;
    }
    std::cout << std::endl;
}

void normPoint(geometry::Point& point) {
    int32_t x = std::round(point.getX());
    int32_t y = std::round(point.getY());
    if (isIntegerCoords( sf::Vector2i(x, y) )) {
        uint32_t new_x = x / constants::CELL_SIZE * constants::CELL_SIZE;
        uint32_t new_y = y / constants::CELL_SIZE * constants::CELL_SIZE;
        if (x % constants::CELL_SIZE > (constants::CELL_SIZE - x % constants::CELL_SIZE) % constants::CELL_SIZE) {
            new_x += constants::CELL_SIZE;
        }
        if (y % constants::CELL_SIZE > (constants::CELL_SIZE - y % constants::CELL_SIZE) % constants::CELL_SIZE) {
            new_y += constants::CELL_SIZE;
        }

        point = geometry::Point{ new_x * 1.F, new_y * 1.F};
    }
}

void Logic::start() {
    // geometry::ConvexPolygon p1
    // { geometry::Point{243, 453}, geometry::Point{627, 83}, geometry::Point{762, 433}, geometry::Point{900, 200} };
    // geometry::ConvexPolygon p2
    // { geometry::Point{193, 159}, geometry::Point{550, 521}, geometry::Point{718, 143} };
    // geometry::ConvexPolygon p3
    // { geometry::Point{202, 317}, geometry::Point{555, 277}, geometry::Point{437, 71} };
    // geometry::ConvexPolygon p4
    // {  geometry::Point{20, 20}, geometry::Point{30, 30}, geometry::Point{40, 40} };
    // polygons.push_back(p1);
    // polygons.push_back(p2);
    // polygons.push_back(p3);
    renew();
    sf::RenderWindow window(sf::VideoMode(constants::WINDOW_W, constants::WINDOW_H), "Triangles");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                update_status();
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                geometry::Point mouse_point = geometry::Point{1.F * mouse_pos.x, 1.F * mouse_pos.y};
                normPoint(mouse_point);

                if (is_debug) {
                    std::cout << mouse_pos.x << ' ' << mouse_pos.y << std::endl;
                    std::cout << "[ " << isIntegerCoords(mouse_pos) << " ]" << std::endl;

                    std::cout.precision(10);
                    for (int i = 0; i < intersection_polygon.size(); i++) {
                        std::cout << std::fixed << 
                        intersection_polygon.getPointAt(i).getX() << ' ' << 
                        intersection_polygon.getPointAt(i).getY() << std::endl; 
                    }
                    // std::cout << polygons[4].size() << std::endl;
                    std::cout << std::fixed << intersection_polygon.area() << std::endl;
                }

                if (status == CREATING) {
                    if (first_drawn_point.getDistance(mouse_point) < constants::COLLAPSE_DIST) {
                        polygons.push_back(being_drawn_polygon);
                        being_drawn_polygon = geometry::ConvexPolygon {};
                        first_drawn_point = geometry::Point {};
                    } else {
                        geometry::ConvexPolygon being_drawn_polygon_copy = being_drawn_polygon;
                        being_drawn_polygon_copy.addPoint(mouse_point);
                        bool in_first_drawn_point = false;
                        for (size_t i = 0; i < being_drawn_polygon_copy.size(); i++) {
                            if (being_drawn_polygon_copy.getPointAt(i) == first_drawn_point) {
                                in_first_drawn_point = true;
                                break;
                            }
                        }
                        if (in_first_drawn_point || being_drawn_polygon.size() < 3) {
                            being_drawn_polygon.addPoint(mouse_point);   
                        }
                    }
                    continue;
                }
                if (status == NOTHING) {
                    first_drawn_point = mouse_point;
                    being_drawn_polygon = geometry::ConvexPolygon{mouse_point};
                    continue;
                    if (event.type == sf::Event::MouseButtonReleased) {
                        first_drawn_point = mouse_point;
                        being_drawn_polygon = geometry::ConvexPolygon{mouse_point};
                    } else {
                        first_drawn_point = mouse_point;
                    }
                    continue;
                    for (size_t i = 0; i < polygons.size(); i++) {
                        for (size_t j = 0; j < polygons[i].size(); j++) {
                            if (polygons[i].getPointAt(j).getDistance(mouse_point) < constants::COLLAPSE_DIST) {
                                catch_polygon = polygons[i];
                                catch_vertex_id = j;
                                polygons.erase(polygons.begin() + i);
                                break;
                            }
                        }
                        if (catch_vertex_id != -1) {
                            break;
                        }
                    }
                    if (catch_vertex_id != -1) {
                        continue;
                    }
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        for (size_t i = 0; i < polygons.size(); i++) {
                            if (polygons[i].isInside(mouse_point)) {
                                grabbed_polygon_id = i;
                                break;
                            }
                        }
                        if (grabbed_polygon_id != -1) {
                            continue;
                        }
                    }
                }
            }
            justDraw(window);
        }

        window.clear(sf::Color::White);
        renew();
        justDraw(window);
        window.display();
        sf::sleep(sf::milliseconds(1));
    }
    return;
}
