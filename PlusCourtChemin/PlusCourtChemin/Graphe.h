#ifndef GRAPHE_H
#define GRAPHE_H

/*
 Fichier d'en tête de la classe graphe.

 \author Augustin Giraudier & Justin Colin
*/

#include <exception>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <list>
#include <thread>
#include <queue>

#include "Vertex.h"
#include "Edge.h"

#define PI         3.14159265358979323846	// valeur de Pi
#define DOUBLE_MAX 1.7976931348623158e+308	// valeur max d'une variable double

/*
 Classe de gestion d'un graphe permettant sa lecture depuis un fichier,
 l'execution de plusieurs algorithmes de plus court chemin ainsi que l'export csv pour MyMaps.
*/
class Graphe
{
private:

	// ----- DONNEES MEMBRES ----- //

	bool oriente;			// explicite si legraphe est orienté ou non
	int nbSommets;			// nombre de sommets du graphe
	int nbArcs;				// nombre d'arc du graphe
	int nbValeursParArc;	// nombre de valeurs portées par un arc (poids...)
	int nbValeurParSommet;	// nombre de valeurs portées par un sommet

	std::vector<Vertex> listeSommets;				// liste des vertices
	std::vector<std::list<Edge>> listeAdjacense;	// liste d'adjacense des arretes

	std::unique_ptr<int> degres;					// tableau des degrés par sommet

	// ----- METHODES PRIVEES ----- //

	/*
		Retourne la sous chaine à l'index donné en splitant par le caractère donné

		@param str : chaine d'entrée
		@param separator : caractère de séparation
		@param index : index de la chaine splitée à retourner

		@return la sous chaine à l'index donné
	*/
	std::string getSplited(std::string& str, const char separator, int index);

	/*
		Retourne l'arrete entre les deux sommets donnés

		@param v1 : index du sommet 1 dans la liste de sommet
		@param v2 : index du sommet 2 dans la liste de sommet

		@return l'arrete joignant les deux sommets ou nullptr si innéxistante
	*/
	Edge* getEdgeBetween(unsigned int v1, unsigned int v2);

	/*
		Retourne la distance réelle à vol d'oiseau entre les deux villes

		@param v1 : vertex de la ville 1
		@param v2 : vertex de la ville 2
	
		@return la distance vol d'oiseau entre les deux villes
	*/
	double computeHeuristique(Vertex& v1, Vertex& v2);

	/*
		Calcule la moyenne des distances entre la ville pointée par iVille et les villes selectionnées 
		et remplace l'index par celui de la ville si la moyenne est la plus petite trouvée
		Le calcul est réalisé sur n villes (idéal pour multithreader)

		@param iVille : premier index de ville
		@param nbCityPerThread : nombre de ville à traiter
		@param villesSelect : villes selectionnéespour les distances
		@param minAverage : plus basse moyenne enregistrée
		@param index : index de la plus basse moyenne enregistrée
	*/
	void VRP1computeNCities(int iVille, int nbCityPerThread, const std::vector<int>& villesSelect, double& min, int& index);

	/*
		Retourne les villes ayant plus d'habitants que le nombre indiqué

		@param nbMinHab : nombre d'habitant à partir duquel la ville est retournée
		@param filename : nom du fichier csv ayant les infos de population

		@return la liste des villes concernées
	*/
	std::vector<int> getCitiesWithMorePeopleThan(unsigned int nbMinHab, std::string& filename);

	/*
		retourne la distance représentant le parcours du circuit avec retour à la ville de départ

		@param circuit : liste des villes à parcourir dans la boucle

		@return la distance parcourue pour toute la boucle
	*/
	double GetCircuitLength(const std::list<int>& circuit);

public:

	// ----- METHODES PUBLIQUES ----- //

	int getNbSommet() { return listeSommets.size(); }
	int getNbArc() { return this->nbArcs; }
	/*
		Crée le graphe à l'aide du fichier de configuration du graphe

		@param filePath : chemin d'accès au fichier tograph
	*/
	Graphe(std::string filePath);

	/*
		O(1)
		Retourne le nom de la ville pointée par un index

		@param index : index de la ville à cibler

		@return le nom de la ville
	*/
	std::string getCityNameWithIndex(unsigned int index) { return listeSommets[index].nom; }

	/*
		O(n)
		retourne l'index de la ville avec son nom 
	*/
	int getIndexWithName(std::string strCityName);

	/*
		Affiche le graphe dans la console

		@param nbMax : nombre maximum de lignes à afficherpar catégorie
	*/
	void Afficher(int nbMax = INT16_MAX);

	/*
		retourne une chaine de caractère contenant le script CSV d'import MyMaps
		afin de visualiser le circuit

		@param circuit : la liste des villes que la boucle contient

		@return l'export CSV du circuit
	*/
	std::string getMyMappsScript(const std::vector<int>& circuit);

	// ----- Dijkstra

	/*
		O(n²)
		Algo dijkstra classique

		@param from : index de la ville de départ
		@param to : index de la ville d'arrivée

		@return la distance minimale entre les deux villes
	*/
	double Dijkstra(unsigned int from, unsigned int to);

	/*
		O(n²)
		Algo Dijkstra vers toutes les villes du graphe

		@param from : index de la ville de départ

		@return la distance minimale entre la ville de départ et chaque ville du graphe
	*/
	std::vector<double> DijkstraAll(unsigned int from);
	/*
		O(m+n*log(n))
		Algo dijkstra avec une priority queue

		@param v1 : index de la ville de départ
		@param v2 : index de la ville d'arrivée

		@return la distance minimale entre les deux villes
	*/
	double DijkstraHeap(unsigned int v1, unsigned int v2);

	// ----- A*

	/*
		O(n²)
		Algo A* basé sur dijkstra2 avec une heuristique en fonction des coordonnées (lat, long)

		@param from : index de la ville de départ
		@param to : index de la ville d'arrivée

		@return la distance minimale entre les deux villes
	*/
	double AStar(unsigned int from, unsigned int to);

	// ----- Question VRP 1

	/*
		O(n^3 * n2) avec n2 le nombre de villes selectionnées
		Algo brute force pour répondre à la question du VRP1
		résultat optimal mais relative lenteur

		@param nbMinHab : nombre d'habitant que les villes selectionnées doivent surpasser
		@param strCsvFileName : nom du fichier CSV à utiliser pour les infos de population

		@return le sommet solution (minimisant les distances à toutes les villes sélectionnées)
	*/
	Vertex* VRP1(unsigned int nbMinHab, std::string strCsvFileName);

	/*
		O(n² * n2) avec n2 le nombre de villes selectionnées
		Algo optimisé pour répondre à la question du VRP1
		résultat quasi optimal et rapide

		@param nbMinHab : nombre d'habitant que les villes selectionnées doivent surpasser
		@param strCsvFileName : nom du fichier CSV à utiliser pour les infos de population

		@return le sommet solution (minimisant les distances à toutes les villes sélectionnées)
	*/
	Vertex* VRP1v2(unsigned int nbMinHab, std::string strCsvFileName);

	// ----- Question VRP 2

	/*
		O(n2² * (m+n*log(n))) avec n2 le nombre de villes selectionnées
		Algo simple de proximité utilisant simplement la ville la plus proche du dernier sommet ajouté

		@param nbMinHab : nombre d'habitant que les villes selectionnées doivent surpasser
		@param strCsvFileName : nom du fichier CSV à utiliser pour les infos de population

		@return un circuit convenable
	*/
	std::vector<int> VRP2(unsigned int nbMinHab, std::string strCsvFileName);

	/*
		O(n2^3 * (m+n*log(n))) avec n2 le nombre de villes selectionnées
		Algo proximité faisant grandir le circuit en ajoutant chaque ville au meilleur endroit dans le circuit

		@param nbMinHab : nombre d'habitant que les villes selectionnées doivent surpasser
		@param strCsvFileName : nom du fichier CSV à utiliser pour les infos de population

		@return un circuit proche de la meilleur solution
	*/
	std::vector<int> VRP2v2(unsigned int nbMinHab, std::string strCsvFileName);
};

#endif