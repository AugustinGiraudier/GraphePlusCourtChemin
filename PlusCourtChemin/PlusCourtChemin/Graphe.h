#ifndef GRAPHE_H
#define GRAPHE_H

#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <list>

#include "Vertex.h"
#include "Edge.h"

#define PI         3.14159265358979323846
#define DOUBLE_MAX 1.7976931348623158e+308

class Graphe
{
private:

	bool oriente;
	int nbSommets;
	int nbArcs;
	int nbValeursParArc;
	int nbValeurParSommet;

	std::vector<Vertex> listeSommets;
	std::vector<std::list<Edge>> listeAdjacense;

	std::unique_ptr<int> degres; // tableau des degrés

	std::string getSplited(std::string& str, const char separator, int index);
	Edge* getEdgeBetween(unsigned int v1, unsigned int v2);
	double computeHeuristique(Vertex& v1, Vertex& v2);

	void computeNCities(int iVille, int nbCityPerThread, const std::vector<int>& villesSelect, double& minAverage, int& index);
	std::vector<int> getCitiesWithMorePeopleThan(unsigned int nbMinHab, std::string& filename);

public:

	Graphe(std::string filePath);
	std::vector<Vertex>* getVertices() { return &this->listeSommets; }
	void Afficher(int nbMax = INT16_MAX);

	// Dikstra
	double Dikstra(unsigned int from, unsigned int to);
	double Dikstra2(unsigned int from, unsigned int to);
	std::vector<double> DikstraAll(unsigned int from);
	double DikstraHeap(unsigned int v1, unsigned int v2);

	// A*
	double AStar(unsigned int from, unsigned int to);

	// Question VRP 1
	Vertex* VRP1(unsigned int nbMinHab, std::string strCsvFileName);

	// Question VRP 2
	std::vector<int> VRP2(unsigned int nbMinHab, std::string strCsvFileName);

};

#endif