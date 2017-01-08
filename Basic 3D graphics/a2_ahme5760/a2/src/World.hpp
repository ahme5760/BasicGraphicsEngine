#ifndef CWORLD_HPP
#define CWORLD_HPP

#include "Cube.hpp"
#include "pyramid.hpp"
#include "House.hpp"
#include <iostream>
#include <list>
#include <vector>


class World {
public:
	Shape* list[3];
	std::list<Shape*> ObjectList;
	std::vector<Shape*>* ObjectVector;

	World();
	~World();
	void draw_world(); // draw all objects in the world
};

#endif
