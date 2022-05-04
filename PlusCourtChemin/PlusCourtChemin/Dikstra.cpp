#include "Graphe.h"

std::vector<double> Graphe::DikstraAll(unsigned int v1)
{
	std::vector<std::pair<double, int>> Z;
	std::vector<double> lambda(listeSommets.size(), DOUBLE_MAX);

	Z.push_back(std::make_pair(0.0f, v1));
	lambda[v1] = 0;

	while (!Z.empty())
	{
		int vertID = -1;
		int vertInZ = -1;
		double min = DOUBLE_MAX;

		// trouver le min
		int ZCpt = 0;
		for (auto& pairVec : Z) {
			if (pairVec.first < min) {
				min = pairVec.first;
				vertID = pairVec.second;
				vertInZ = ZCpt;
			}
			ZCpt++;
		}

		Z.erase(std::next(Z.begin(), vertInZ));

		for (Edge& e : this->listeAdjacense[vertID]) {

			int v = e.sommetTerminal;
			double dist = e.valeurs[0];

			if (lambda[v] > lambda[vertID] + dist)
			{
				lambda[v] = lambda[vertID] + dist;
				Z.push_back(std::make_pair(lambda[v], v));
			}
		}
	}

	return lambda;
}

double Graphe::Dikstra(unsigned int v1, unsigned int v2)
{

	// vecteur de retour lambda :
	std::vector<double> lambda = std::vector<double>(this->listeSommets.size());

	std::vector<unsigned int> Z = std::vector<unsigned int>();
	Z.reserve(this->listeSommets.size() - 1);
	for (unsigned int index = 0; index < this->listeSommets.size(); index++)
		if (index != v1)
			Z.emplace_back(index);
	lambda[v1] = 0;

	for (unsigned int vertID : Z) {

		// recherche d'arrete v1 --> v2
		Edge* e = getEdgeBetween(v1, vertID);

		// si l'arrete existe :
		if (e != nullptr)
			lambda[vertID] = e->valeurs[0];
		else
			lambda[vertID] = DOUBLE_MAX;
	}

	while (!Z.empty()) {

		// recuperation du minimum dans lambda avec vert dans Z :
		int x = 0;
		{
			int idInZ = -1;
			int xInZ = 0;
			double minVal = DOUBLE_MAX;
			for (unsigned int vertID : Z) {
				idInZ++;
				if (lambda[vertID] <= minVal) {
					minVal = lambda[vertID];
					x = vertID;
					xInZ = idInZ;
				}
			}
			if (Z[xInZ] == v2)
				return lambda[v2];
			Z.erase(std::next(Z.begin(), xInZ)); // suppression de x dans Z
		}

		// pour tous les successeurs de x :
		for (Edge& e : this->listeAdjacense[x]) {
			int iId = e.sommetTerminal;

			// s'il appartient a Z :
			for (unsigned int vertID : Z) {
				if (vertID == iId) {
					// si lambda de x + l(x,i) < lambda de i :
					if (lambda[x] + e.valeurs[0] < lambda[iId])
						lambda[iId] = lambda[x] + e.valeurs[0];
					break;
				}
			}
		}
	}

	return -1;
}

double Graphe::DikstraHeap(unsigned int v1, unsigned int v2)
{
	std::priority_queue< std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> heap;

	std::vector<double> lambda(listeSommets.size(), DOUBLE_MAX);

	heap.push(std::make_pair(0.0f, v1));
	lambda[v1] = 0;

	while (!heap.empty())
	{
		int vertID = heap.top().second;
		if (vertID == v2)
			return lambda[vertID];

		heap.pop();

		for (Edge& e : this->listeAdjacense[vertID]) {

			int v = e.sommetTerminal;
			double dist = e.valeurs[0];

			if (lambda[v] > lambda[vertID] + dist)
			{
				lambda[v] = lambda[vertID] + dist;
				heap.push(std::make_pair(lambda[v], v));
			}
		}
	}

	return lambda[v2];
}

double Graphe::Dikstra2(unsigned int v1, unsigned int v2)
{
	std::vector<std::pair<double, int>> Z;
	std::vector<double> lambda(listeSommets.size(), DOUBLE_MAX);

	Z.push_back(std::make_pair(0.0f, v1));
	lambda[v1] = 0;

	while (!Z.empty())
	{
		int vertID = -1;
		int vertInZ = -1;
		double min = DOUBLE_MAX;

		// trouver le min
		int ZCpt = 0;
		for (auto& pairVec : Z) {
			if (pairVec.first < min) {
				min = pairVec.first;
				vertID = pairVec.second;
				vertInZ = ZCpt;
			}
			ZCpt++;
		}

		if (vertID == v2)
			return lambda[vertID];

		Z.erase(std::next(Z.begin(), vertInZ));

		for (Edge& e : this->listeAdjacense[vertID]) {

			int v = e.sommetTerminal;
			double dist = e.valeurs[0];

			if (lambda[v] > lambda[vertID] + dist)
			{
				lambda[v] = lambda[vertID] + dist;
				Z.push_back(std::make_pair(lambda[v], v));
			}
		}
	}

	return lambda[v2];
}