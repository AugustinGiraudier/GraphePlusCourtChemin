#include "Graphe.h"

Graphe::Graphe(std::string filePath) {

	std::ifstream is(filePath);

	if (!is.is_open()) {
		std::cerr << "impossible d'ouvrir le fichier...";
		exit(1);
	}

	std::string line;

	getline(is, line); // nom du graphe

	getline(is, line); // oriented
	if (getSplited(line, ' ', 1).find("true") != std::string::npos) // oriented
		this->oriente = true;
	else
		this->oriente = false;

	getline(is, line); // title param

	getline(is, line); // params
	this->nbSommets = stoi(getSplited(line, ' ', 0));
	this->nbValeurParSommet = stoi(getSplited(line, ' ', 1)); // valeurs par sommet;
	this->nbArcs = stoi(getSplited(line, ' ', 2));
	this->nbValeursParArc = stoi(getSplited(line, ' ', 3));

	// initialisation des arrayList :
	this->listeSommets = std::vector<Vertex>();
	listeAdjacense.reserve(this->nbSommets);
	this->listeAdjacense = std::vector<std::list<Edge>>();
	listeAdjacense.reserve(this->nbSommets);
	for (int i = 0; i < this->nbSommets; i++)
		this->listeAdjacense.emplace_back(std::list<Edge>());
	this->degres = std::unique_ptr<int>(new int[nbSommets] {0});

	getline(is, line); // Titre Vertices

	// pour chaque vertex :
	while (getline(is, line)) {
		if (getSplited(line, ' ', 0).find("Edges") != std::string::npos)
			break;

		std::vector<double> val = std::vector<double>(this->nbValeurParSommet);
		for (int i = 0; i < this->nbValeurParSommet; i++) {
			val[i] = std::stod(getSplited(line, ' ', 2+i));
		}

		this->listeSommets.emplace_back(Vertex(getSplited(line, ' ', 1), val));
	}

	// pour chaque edge :
	while (getline(is, line)) {

		std::vector<double> val = std::vector<double>();
		for (int i = 0; i < this->nbValeursParArc; i++)
			val.push_back(std::stod((getSplited(line, ' ', 2 + i))));

		this->listeAdjacense[std::stoi(getSplited(line, ' ', 0))].push_back(Edge(std::stoi(getSplited(line, ' ', 1)), val));
		this->degres.get()[std::stoi(getSplited(line, ' ', 0))]++;
		this->degres.get()[std::stoi(getSplited(line, ' ', 1))]++;
		if (!this->oriente) {
			this->degres.get()[std::stoi(getSplited(line, ' ', 0))]++;
			this->degres.get()[std::stoi(getSplited(line, ' ', 1))]++;
			this->listeAdjacense[std::stoi(getSplited(line, ' ', 1))].push_back(Edge(std::stoi(getSplited(line, ' ', 0)), val));
		}
	}
}

std::vector<int> Graphe::getCitiesWithMorePeopleThan(unsigned int nbMinHab, std::string& filename)
{
	std::ifstream is(filename);

	if (!is.is_open()) {
		std::cerr << "impossible d'ouvrir le fichier...";
		exit(1);
	}

	const char SEPARATOR = ';';
	std::string line;

	getline(is, line); // nom des colonnes

	std::vector<int> villesSelect{};

	// selection des villes de + de n habitants
	while (getline(is, line)) {
		std::string ville = getSplited(line, SEPARATOR, 0);
		unsigned int nbHabitants = stoi(getSplited(line, SEPARATOR, 2));
		if (nbHabitants >= nbMinHab)
			for (int i_vert = 0; i_vert < this->listeSommets.size();i_vert++)
				if (this->listeSommets[i_vert].nom == ville) {
					villesSelect.push_back(i_vert);
					break;
				}
	}
	return villesSelect;
}

std::string Graphe::getSplited(std::string& str, const char separator, int index) {
	int nbSepa = 0;
	int id = 0;
	while (nbSepa < index && id < str.size()) {
		if (str[id] == separator)
			nbSepa++;
		id++;
	}
	std::string out = "";
	for (char* pchar = &(str[id]); *pchar != separator && pchar != &(str[str.size()]); pchar++) {
		out += *pchar;
	}
	return out;
}

Edge* Graphe::getEdgeBetween(unsigned int v1, unsigned int v2)
{
	// verifier qu'il y a bien une arrete vers v2
	for (Edge& e : this->listeAdjacense[v1]) {
		if (e.sommetTerminal == v2)
			return &e;
	}
	// sinon on retourne null
	return nullptr;
}

int Graphe::getIndexWithName(std::string strCityName)
{
	for (int iVille = 0; iVille < listeSommets.size(); iVille++)
		if (listeSommets[iVille].nom == strCityName)
			return iVille;
	return -1;
}

void Graphe::Afficher(int nbMax)
{
	std::cout << "Affichage Graphe : " << std::endl;
	std::cout << "Oriente : " << (this->oriente ? "oui" : "non") << std::endl;
	std::cout << "Nb Sommets : " << this->nbSommets << std::endl;
	std::cout << "Nb Arcs : " << this->nbArcs << std::endl;
	std::cout << "Nb Valeur par Arc : " << this->nbValeursParArc << std::endl;
	std::cout << "\nSommets :" << std::endl;
	for (int iSommet = 0; iSommet < std::min(this->nbSommets, nbMax); iSommet++) {
		std::cout << this->listeSommets[iSommet].toString() << " (deg:" + std::to_string(this->degres.get()[iSommet]) + ")";
		if (this->nbValeurParSommet > 0) {
			std::cout << " (val:";
			for (int i = 0; i < this->nbValeurParSommet; i++) {
				std::cout << " " << this->listeSommets[iSommet].values[i];
			}
			std::cout << ")";
		}
		std::cout << std::endl;
	}
	std::cout << "\nArretes :" << std::endl;
	for (int i = 0; i < std::min(this->nbSommets, nbMax); i++) {
		std::string str = "";
		str += std::to_string(i) + " : ";
		for (Edge e : this->listeAdjacense[i])
			str += e.toString() + " ";
		std::cout << str << std::endl;
	}
}