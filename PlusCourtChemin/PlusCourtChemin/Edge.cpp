#include "Edge.h"


Edge::Edge(int sommetTerm, std::vector<double> valeurs)
:sommetTerminal(sommetTerm)
,valeurs(valeurs)
{}

std::string Edge::toString() {

	std::string strValues = "";

	if (this->valeurs.size() > 0) {
		for (double val : valeurs)
			strValues += std::to_string(val) + ",";
		strValues = strValues.substr(0, strValues.length() - 1);
	}

	return std::to_string(this->sommetTerminal) + "(" + strValues + ")";
}