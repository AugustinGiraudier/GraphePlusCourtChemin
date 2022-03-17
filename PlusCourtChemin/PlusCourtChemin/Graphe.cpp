#include "Graphe.h"

#include <algorithm>

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
		val.reserve(this->nbValeursParArc);
		for (int i = 0; i < this->nbValeursParArc; i++)
			val.emplace_back(std::stod((getSplited(line, ' ', 2 + i))));

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

std::vector<double> Graphe::DikstraAll(unsigned int v1)
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
			lambda[vertID] = DBL_MAX;
	}

	while (!Z.empty()) {
	
		// récupération du minimum dans lambda avec vert dans Z :
		int x = 0;
		{
			int idInZ = -1;
			int xInZ = 0;
			double minVal = DBL_MAX;
			for (unsigned int vertID : Z) {
				idInZ++;
				if (lambda[vertID] <= minVal) {
					minVal = lambda[vertID];
					x = vertID;
					xInZ = idInZ;
				}
			}
			Z.erase(std::next(Z.begin(), xInZ)); // suppression de x dans Z
		}

		// pour tous les successeurs de x :
		for (Edge& e : this->listeAdjacense[x]) {
			int iId = e.sommetTerminal;

			// s'il appartient à Z :
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
			lambda[vertID] = DBL_MAX;
	}

	while (!Z.empty()) {

		// récupération du minimum dans lambda avec vert dans Z :
		int x = 0;
		{
			int idInZ = -1;
			int xInZ = 0;
			double minVal = DBL_MAX;
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

			// s'il appartient à Z :
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
