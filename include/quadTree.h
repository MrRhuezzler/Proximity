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
        Point topLeft;                  // Top-Left coordinate of the quad
        Point botRight;                 // Right-Bottom coordinate of the quad

        Quad* northEast;                // Top-Right child quad
        Quad* northWest;                // Top-Left child quad
        Quad* southEast;                // Bottom-Right child quad
        Quad* southWest;                // Bottom-Left child quad

        std::list<Node*> nodeList;      // List of nodes inside the quad
        int nodeCount;                  // Count of the nodes inside the quad
        const int threshold;            // threshold indicates the maximum number of nodes inside a quad

    public:
        Quad();                         
        Quad(Point, Point, int);        
        ~Quad();                       

        void insert(Node*);             // Function to insert a node in the quad
        bool inQuad(Node*);             // Function to check if the quad contains a node
        void split();                   // Function to split the quad into four children quads
        bool isEmpty();                 // Function to check if the quad has any nodes in it
        std::list<Node*> inRange(Point, Point, Terrain);    // Function to find the nodes in given range
        bool intersects(Point, Point);  // Checks if the given rectangle intersects with the quad

        void display();                 // Displays info on the quad tree rooted at the quad
};

#endif