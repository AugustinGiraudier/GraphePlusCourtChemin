#include "Graphe.h"

double Graphe::Dijkstra(unsigned int v1, unsigned int v2)
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

std::vector<double> Graphe::DijkstraAll(unsigned int v1)
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

double Graphe::DijkstraHeap(unsigned int v1, unsigned int v2)
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