#include "Graphe.h"

std::vector<int> Graphe::VRP2(unsigned int nbMinHab, std::string strCsvFileName)
{
	// récupération des villes concernées :
	std::list<int> ListvillesSelect;

	{
		std::vector<int> villesSelect = getCitiesWithMorePeopleThan(nbMinHab, strCsvFileName);
		std::cout << "[i] nombre de villes trouvees : " << villesSelect.size() << std::endl;
		ListvillesSelect = std::list<int>(villesSelect.begin(), villesSelect.end());
	}

	std::vector<int> chemin{};
	chemin.reserve(ListvillesSelect.size());

	chemin.emplace_back(ListvillesSelect.front());
	ListvillesSelect.remove(ListvillesSelect.front());

	while (!ListvillesSelect.empty())
	{
		double min = DOUBLE_MAX;
		int index = -1;

		for (int val : ListvillesSelect) {
			double res = DikstraHeap(chemin[chemin.size() - 1], val);
			if (res < min) {
				min = res;
				index = val;
			}
		}

		chemin.emplace_back(index);
		ListvillesSelect.remove(index);
	}

	std::cout << "[i] distance totale de la boucle : " << GetCircuitLength(std::list<int>(chemin.begin(), chemin.end())) << " km" << std::endl;

	return chemin;
}

std::vector<int> Graphe::VRP2v2(unsigned int nbMinHab, std::string strCsvFileName)
{
	// récupération des villes concernées :
	std::vector<int> villesSelect = getCitiesWithMorePeopleThan(nbMinHab, strCsvFileName);
	std::cout << "[i] nombre de villes trouvees : " << villesSelect.size() << std::endl;

	std::list<int> chemin;
	chemin.push_back(villesSelect[0]);

	for (int iVille = 1; iVille < villesSelect.size(); iVille++) {
		double cheminLen = GetCircuitLength(chemin);
		std::list<int>::iterator BestSpace = chemin.begin();
		double length = DOUBLE_MAX;
		std::list<int>::iterator it = chemin.begin();
		std::list<int>::iterator it2 = chemin.begin();
		std::advance(it2, 1);
		while (it != chemin.end()) {

			int villeA = *it;
			int villeB = (it2 == chemin.end()) ? 0 : *(it2);

			double a = DikstraHeap(villeA, villesSelect[iVille]);
			double b = DikstraHeap(villesSelect[iVille], villeB);
			double c = DikstraHeap(villeA, villeB);
			double circuitlen = cheminLen + a + b - c;

			std::advance(it, 1);
			std::advance(it2, 1);

			if (circuitlen < length) {
				length = circuitlen;
				BestSpace = it;
			}
		}
		chemin.insert(BestSpace, villesSelect[iVille]);
	}

	std::cout << "[i] distance totale de la boucle : " << GetCircuitLength(chemin) << " km" << std::endl;

	return std::vector<int>(chemin.begin(), chemin.end());
}

double Graphe::GetCircuitLength(const std::list<int>& circuit)
{
	double res = 0;

	auto it = circuit.begin();
	auto it2 = circuit.begin();
	std::advance(it2, 1);
	while (it2 != circuit.end()) {
		int iVille = *it;
		int iVille2 = *it2;
		res += DikstraHeap(iVille, iVille2);
		std::advance(it, 1);
		std::advance(it2, 1);
	}

	res += DikstraHeap(circuit.back(), circuit.front());

	return res;
}

std::string Graphe::getMyMappsScript(const std::vector<int>& circuit)
{
	std::string out = "";
	out += "WKT,nom\n";
	for (int ires = 0; ires < circuit.size();ires++) {
		int iville = circuit[ires];
		int iville2 = circuit[((ires + 1) == circuit.size() ? 0 : ires + 1)];
		out += "\"POINT(" + std::to_string(listeSommets[iville].values[0]) + ' ' + std::to_string(listeSommets[iville].values[1]) + ")\"," + listeSommets[iville].nom + ",\n";
		out += "\"LINESTRING(" + std::to_string(listeSommets[iville].values[0]) + ' ' + std::to_string(listeSommets[iville].values[1]) + ','
			+ std::to_string(listeSommets[iville2].values[0]) + ' ' + std::to_string(listeSommets[iville2].values[1]) + ")\",line" + std::to_string(iville) + std::to_string(iville2) + ",\n";
	}
	return out;
}