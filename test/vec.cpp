#include <iostream>
#include <quadTree.h>

int main(int argv, char** args) {
    Quad Q = Quad(Point(0, 0), Point(8, 8), 4);
    Node *a = new Node(1, 2, Terrain::LOCATION);
    Node *b = new Node(5, 3, Terrain::LOCATION);
    Node *c = new Node(2, 7, Terrain::LOCATION);
    Node *d = new Node(6, 6, Terrain::LOCATION);
    Node *e = new Node(2, 3, Terrain::LOCATION);
    Node *f = new Node(1, 3, Terrain::LOCATION);
    Node *g = new Node(3, 3, Terrain::LOCATION);
    Node *h = new Node(2, 1, Terrain::LOCATION);
    Node *i = new Node(3, 2, Terrain::LOCATION);
    Q.insert(a);
    Q.insert(b);
    Q.insert(c);
    Q.insert(d);
    Q.insert(e);
    Q.insert(f);
    Q.insert(g);
    Q.insert(h);
    Q.insert(i);
    Q.display();
    return 0;
}