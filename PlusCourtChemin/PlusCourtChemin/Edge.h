#ifndef EDGE_H
#define EDGE_H

#include<string>
#include<vector>

class Edge
{
public:
	int sommetTerminal;
	std::vector<double> valeurs;

	Edge(int sommetTerm, std::vector<double> valeurs);

	std::string toString();
};


#endif