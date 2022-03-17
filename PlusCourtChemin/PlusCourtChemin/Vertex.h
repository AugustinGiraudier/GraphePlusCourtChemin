#ifndef VERTEX_H
#define VERTEX_H

#include <string>

class Vertex {

private:
	std::string nom;

public:
	Vertex(std::string nom);
	std::string toString();
};
#endif