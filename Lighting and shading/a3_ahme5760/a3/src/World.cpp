/**
 * File: World.hpp
 */

#include "World.hpp"


extern bool isSolar;

World::World() {
	list[0] = new Cube();
	list[1] = new Planet(); //sun
	list[2] = new Planet();//earth
	list[3] = new Planet();//moon

    // object list
	ObjectList.push_back(list[0]);
	ObjectList.push_back(list[1]);
	ObjectList.push_back(list[2]);
	ObjectList.push_back(list[3]);

	// object vector
	ObjectVector = new std::vector<Shape*>(4);
	std::vector<Shape*>& ObjectVectorRef = *ObjectVector;
	ObjectVectorRef[0] = list[0];
	ObjectVectorRef[1] = list[1];
	ObjectVectorRef[2] = list[2];
	ObjectVectorRef[3] = list[3];

}

World::~World(){
    delete list[0];
    delete list[1];
    delete list[2];
    delete list[3];
}

void World::draw_world() {
	if (!isSolar) {
		list[0]->draw();
	}
	else {
		list[1]->set_colour(1,0,0);
		list[1]->draw();
		list[2]->set_colour(0,0,1);
		list[2]->draw();
		list[3]->set_colour(0,1,0);
		list[3]->draw();
	}

}

void World::reset(){
    list[0]->reset();
    list[1]->reset();
    list[2]->reset();
    list[3]->reset();
}


