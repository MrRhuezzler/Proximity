#include <quadTree.h>
#include <iostream>

Node::Node() : Point()
{
    type = Terrain::LOCATION;       // By default the terrain of a Node is set to LOCATION
}

Node::Node(int x, int y, Terrain t = Terrain::LOCATION) : Point(x, y)
{
    type = t;
}

Node::~Node()
{}

// Utililty function to check if the node is in the specified rectangle
bool contains(Node* n, Point l, Point r)        
{
    return (n->x >= l.x && n->x <= r.x && n->y >= l.y && n->y <= r.y);
}

Quad::Quad():threshold(4), nodeCount(0)
{
    topLeft = Point(0, 0);
    botRight = Point(8, 8);

    northEast = NULL;
    northWest = NULL;
    southEast = NULL;
    southWest = NULL;
}

Quad::Quad(Point tL, Point bR, int thr):threshold(thr), nodeCount(0)
{
    topLeft = tL;
    botRight = bR;

    northEast = NULL;
    northWest = NULL;
    southEast = NULL;
    southWest = NULL;
}

bool Quad::inQuad(Node* n)
{
    if(n->x > botRight.x || n->y > botRight.y || n->x < topLeft.x || n->y < topLeft.y)
    {
        return false;
    }
    return true;
}

bool Quad::isEmpty()
{
    if(nodeList.empty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Quad::split()
{
    //std::cout<<"\nSPLITTING...\n\n";
    northWest = new Quad(topLeft, Point((topLeft.x + botRight.x) / 2, (topLeft.y + botRight.y) / 2), threshold);
    northEast = new Quad(Point((topLeft.x + botRight.x) / 2, topLeft.y), Point(botRight.x, (topLeft.y + botRight.y) / 2), threshold);
    southWest = new Quad(Point(topLeft.x, (topLeft.y + botRight.y) / 2), Point((topLeft.x + botRight.x) / 2, botRight.y), threshold);
    southEast = new Quad(Point((topLeft.x + botRight.x) / 2, (topLeft.y + botRight.y) / 2), botRight, threshold);
}

void Quad::insert(Node* n)
{
    if(!inQuad(n))                  // If the node is not within the boundary of the quad, return
    {
        return;
    }

    if((topLeft.x + botRight.x) / 2 >= n->x)                // WEST
    {
        if((topLeft.y + botRight.y) / 2 >= n->y)            // NORTH
        {           
            if(nodeCount + 1 > threshold)                   // Checking if inserting the node will exceed the threshold
            {
                if(northWest == NULL)                       // If it has not already been split, we split the quad
                {
                    split();
                }
                northWest->insert(n);                       // Recursively insert the node into the northwest quad
            }
            else                                            // If threshold is not exceeded, simply insert the node
            {
                nodeList.push_back(n);                     
                nodeCount++;
            }
        }
        else                                                // SOUTH
        {
            if(nodeCount + 1 > threshold)
            {
                if(northWest == NULL)
                {
                    split();
                }
                southWest->insert(n);
            }
            else
            {
                nodeList.push_back(n);
                nodeCount++;
            }
        }
    }
    else                                                    // EAST
    {
        if((topLeft.y + botRight.y) / 2 >= n->y)            // NORTH
        {
            if(nodeCount + 1 > threshold)
            {
                if(northWest == NULL)
                {
                    split();
                }
                northEast->insert(n);
            }
            else
            {
                nodeList.push_back(n);
                nodeCount++;
            }
        }
        else                                                // SOUTH
        {
            if(nodeCount + 1 > threshold)
            {
                if(northWest == NULL)
                {
                    split();
                }
                southEast->insert(n);
            }
            else
            {
                nodeList.push_back(n);
                nodeCount++;
            }
        }
    }
}

std::list<Node*> Quad::inRange(Point lT, Point rB, Terrain target)
{
    std::list<Node*> output;        // output is the list of nodes of type 'target' inside the specified query region

    if(!intersects(lT, rB))         // If the query region is outside the quad boundary, simply return
    {
        return output;
    }

    for(auto i = nodeList.begin(); i != nodeList.end(); ++i)        // Traversing all the nodes in the quad(Maximum of 'threshold' nodes)
    {
        if(contains(*i, lT, rB) && target == (*i)->type)            // Checking if the node is inside the query region and if it has the desired type
        {
            output.push_back(*i);                                   // Add the node to the output list
        }
    }

    if(northWest == NULL)       // Checking if the quad has been split
    {
        return output;          // If it has not been split, there is nothing more to check
    }
    else
    {
        // To the output list we recursively add the nodes that are inside the query range from the inner quads
        output.splice(output.end(), northWest->inRange(lT, rB, target));        
        output.splice(output.end(), northEast->inRange(lT, rB, target));
        output.splice(output.end(), southWest->inRange(lT, rB, target));
        output.splice(output.end(), southEast->inRange(lT, rB, target));

        return output;
    }
}

bool Quad::intersects(Point u, Point v)
{
    if(topLeft.x >= v.x || u.x >= botRight.x)
    {
        return false;
    }
    else if(botRight.y <= u.y || v.y <= topLeft.y)
    {
        return false;
    }
    else
    {
        return true;
    }
}


void Quad::display()
{
    for(auto i = nodeList.begin(); i != nodeList.end(); ++i)
    {
        std::cout<<(*i)->type<<"\n";
    }

    if(northWest != NULL)
    {
        std::cout<<"\nInner:\n";

        if(!northWest->isEmpty())           
        {
            std::cout<<"NorthWest:\n";
            northWest->display();
            std::cout<<"\n";
        }
        if(!northEast->isEmpty())
        {
            std::cout<<"NorthEast:\n";
            northEast->display();
            std::cout<<"\n";
        }
        if(!southWest->isEmpty())
        {
            std::cout<<"SouthWest:\n";
            southWest->display();
            std::cout<<"\n";
        }
        if(!southEast->isEmpty())
        {
            std::cout<<"SouthEast:\n";
            southEast->display();
            std::cout<<"\n";
        }
    }
    
    return;
}


Quad::~Quad()
{
    while(!nodeList.empty())
    {
        delete nodeList.front();
        nodeList.pop_front();
    }

    // Recursively deletes all the inner quads
    if(northEast) delete northEast;
    if(northWest) delete northWest;
    if(southEast) delete southEast;
    if(southWest) delete southWest;
}