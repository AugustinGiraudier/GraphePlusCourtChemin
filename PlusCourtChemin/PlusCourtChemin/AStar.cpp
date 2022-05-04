#include "Graphe.h"
#include <exception>
#include <math.h>

double Graphe::AStar(unsigned int v1, unsigned int v2) {

	// calcul des heuristiques :
	std::vector<double> heuristique = std::vector<double>(this->listeSommets.size(), DOUBLE_MAX-1);

	for (int iSommet = 0; iSommet < this->listeSommets.size(); iSommet++) {
		if (iSommet == v2)
			heuristique[iSommet] = 0;
		else
			heuristique[iSommet] = this->computeHeuristique(this->listeSommets[iSommet], this->listeSommets[v2]);
	}

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
				double val = pairVec.first + heuristique[pairVec.second];
				if (val < min) {
					min = val;
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
}

double Graphe::computeHeuristique(Vertex& v1, Vertex& v2) {

	double delta = v2.values[0] - v1.values[0];
	double radlat1 = v1.values[1] * PI / 180;
	double radlat2 = v2.values[1] * PI / 180;
	double sinus = sin(radlat1) * sin(radlat2);
	double cosinus = cos(radlat1) * cos(radlat2) * cos(delta * PI / 180);
	double Sab = acos(sinus + cosinus);
	return (Sab * 6371);

}