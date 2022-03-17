
#include"Vertex.h"

Vertex::Vertex(std::string nom)
	:nom(nom)
	{}

std::string Vertex::toString() {
	return "Vertex : " + this->nom;
}