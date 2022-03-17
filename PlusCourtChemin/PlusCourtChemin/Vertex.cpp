
#include"Vertex.h"

Vertex::Vertex(std::string nom, std::vector<double> values)
	: nom(nom)
	, values(values)
{}

std::string Vertex::toString() {
	return "Vertex : " + this->nom;
}