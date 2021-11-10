#include <point.h>

int main(){

    Point u(5, 4);
    Point v(6, 8);

    std::cout << u + v << " | " << u - v << std::endl;
    u += v;
    std::cout << u << std::endl;

    return 0;
}