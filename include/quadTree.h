#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <iostream>
#include <string>
#include <list>

#include "point.h"
#include "map.h"

enum class Terrain;

class Node : public Point
{
    public:
        Terrain type;
        Node();
        Node(int, int, Terrain);
        ~Node();
};

class Quad
{
    private:
        Point topLeft;
        Point botRight;
        Quad* northEast;
        Quad* northWest;
        Quad* southEast;
        Quad* southWest;
        std::list<Node*> nodeList;
        int nodeCount;
        const int threshold;
    public:
        Quad();
        Quad(Point, Point, int);
        ~Quad();

        void insert(Node*);
        bool inQuad(Node*);
        void split();
        bool isEmpty();
        std::list<Node*> inRange(Point, Point, Terrain);
        bool intersects(Point, Point);

        void display();   //--> TO BE DEBUGGED
};

#endif // __QUADTREE_H__