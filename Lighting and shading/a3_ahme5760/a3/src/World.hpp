#ifndef CWORLD_HPP
#define CWORLD_HPP

#include "Cube.hpp"
#include "Light.hpp"
#include "Planet.hpp"
#include <iostream>
#include <list>
#include <vector>


class World {
public:
	Shape* list[4];
	std::list<Shape*> ObjectList;
	std::vector<Shape*>* ObjectVector;

	World();
	~World();
	void draw_world(); // draw all objects in the world
	void reset();
};

#endif
