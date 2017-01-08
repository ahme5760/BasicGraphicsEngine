/**
 * File: World.hpp
 */

#include "World.hpp"

World::World() {
	list[0] = new Cube();
	list[1] = new pyramid();
	list[2] = new House();

    // object list
	ObjectList.push_back(list[0]);
	ObjectList.push_back(list[1]);
	ObjectList.push_back(list[2]);

	// object vector
	ObjectVector = new std::vector<Shape*>(3);
	std::vector<Shape*>& ObjectVectorRef = *ObjectVector;
	ObjectVectorRef[0] = list[0];
	ObjectVectorRef[1] = list[1];
	ObjectVectorRef[2] = list[2];

}

World::~World(){
    delete list[0];
    delete list[1];
    delete list[2];
}

void World::draw_world() {
	list[0]->draw();
	list[1]->draw();
	list[2]->draw();

	//
	/*
	for (std::list<Shape*>::const_iterator it = ObjectList.begin(); it!=ObjectList.end(); ++it) {
		(*it)->draw();
	}

	//
	std::vector<Shape*>& ObjectVectorRef = *ObjectVector;
	for (int i = 0; i < 2; i++ ) {
		 ObjectVectorRef[i]->draw();
	}
*/
}

