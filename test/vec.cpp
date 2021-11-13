#include <iostream>
#include "point.h"
#include "zorder.h"

int main() {

    Z order;
    std::vector<Point> zSpace = order.getSpace();
    // for(Point& p : zSpace){
    //     std::cout << p << std::endl;
    // }

    std::vector<Z::queryZCode> h = order.getRanges(0, Point(5, 5), Point(0, 0), 7);

    std::vector<Point> highlight = order.inRange(Point(5, 5), Point(0, 0));
    for(Point& p : highlight){
        std::cout << p << std::endl;
    }

    highlight = order.inRange(Point(0, 0), Point(5, 5));
    for(Point& p : highlight){
        std::cout << p << std::endl;
    }


    return 0;
}