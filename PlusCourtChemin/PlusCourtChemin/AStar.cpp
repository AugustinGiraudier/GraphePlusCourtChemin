#include "Graphe.h"


double Graphe::AStar(unsigned int v1, unsigned int v2) {

	// calcul des heuristiques :
	std::vector<double> heuristique = std::vector<double>(this->listeSommets.size());

	for (int iSommet = 0; iSommet < this->listeSommets.size(); iSommet++)
		heuristique[iSommet] = this->computeHeuristique(this->listeSommets[iSommet], this->listeSommets[v2]);

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
				double val = lambda[vertID] + heuristique[vertID];
				if (val <= minVal) {
					minVal = val;
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

double Graphe::computeHeuristique(Vertex& v1, Vertex& v2) {

	double delta = v2.values[0] - v1.values[0];
	double radlat1 = v1.values[1] * PI / 180;
	double radlat2 = v2.values[1] * PI / 180;
	double sinus = sin(radlat1) * sin(radlat2);
	double cosinus = cos(radlat1) * cos(radlat2) * cos(delta * PI / 180);
	double Sab = acos(sinus + cosinus);
	return (Sab * 6371);

}