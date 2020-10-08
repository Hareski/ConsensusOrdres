#ifndef GRAPHEBICOLORE_H
#define GRAPHEBICOLORE_H

#include <vector>

class GrapheBicolore {
private:
    ///////////////////////////////////////////////////////////////////////////////////////////
    // Les variables
    int n; // Nombre de sommets
    struct Voisin {
        int sommet; // L'identifiant du sommet
        bool active; // Indique si le sommet est actif ou non
    };
    std::vector<std::vector<Voisin>> voisinsNoirs;
    std::vector<std::vector<Voisin>> voisinsVerts;
    std::vector<int> sommetsNoirs; // doit être trié
    /** Structure pour une arête {x,y} */
    struct Arete {
        int x, y;
    };
    std::vector<Arete> listeAretesVertes; // peuvent être active ou non dans le graphe courant

    ///////////////////////////////////////////////////////////////////////////////////////////
    // Les méthodes privées

    std::vector<Arete> aretesVertes();

    std::vector<GrapheBicolore::Arete> selectionAretesVertes();

    std::vector<Arete> aretesVertesActives();

    std::vector<Arete> aretesVertesDesactives();

    std::vector<int> voisinsVertsActifs(int s);

    std::vector<int> voisinsNoirsActifs(int s);

    bool ajouterAreteVerte(int sommet1, int sommet2);

    bool ajouterAreteNoire(int sommet1, int sommet2);

    bool testerAreteVerteActive(int a, int b);

    bool testerAreteNoireActive(int a, int b);

    std::vector<int> getSommetsNoirs();

    std::vector<int> getSommetsNoirsParAdjacence();

    int prochainSommetNoir(int precedent, int actuel);

    int prochainSommetVert(int precedent, int actuel);

    bool estSommetNoir(int s);

    bool ajouterSommetNoir(int s);

    int degreVert(int s);

    int degreNoirMax();

    int degreVertMax();

    bool cycleVert();

    bool mauvaisCycle();

    int degreNoir(int s);

    void activerAreteVerte(int sommet1, int sommet2);

    void desactiverAreteVerte(int sommet1, int sommet2);

    std::vector<std::vector<double>> matriceAdjacenceActifsVerte(bool equivalentesNoires);

    std::vector<std::vector<double>> denombrerCheminsVerts(int tailleMax, bool tolererEquivalentesNoires);

    double denombrerMauvaisCycles(int tailleMax, std::vector<int> sommetsNoirs);

    void desactiverToutesAretesVertes();

    void activerToutesAretesVertes();

    void activerDepuisEnumeration(std::vector<int> enume);

    void triFusionSommets(std::vector<int> &sommets);

    bool appartientSommetListeTriee(int s, std::vector<int> sommets);

    void triFusionAretesParDegre(std::vector<Arete> &aretes);

    bool appartientCycle(Arete e);

    GrapheBicolore* sousGraphe(std::vector<int> sommets);

    std::vector<GrapheBicolore*> composantesConnexes();

    std::vector<GrapheBicolore*> divisionGraphe();

    bool estSature(int s, std::vector<Arete> couplage);

    std::vector<Arete> couplageMaximumAretesVertesActives();

    void desactiverAretesIncoherentes();

public:
    ///////////////////////////////////////////////////////////////////////////////////////////
    // Les méthodes publiques

    GrapheBicolore(int n);

    GrapheBicolore(std::string cheminFichier);

    GrapheBicolore(int n, std::string cheminFichierNoires, std::string cheminFichierVertes);

    void fichierSage(std::string cheminFichier);

    void fichierTikz(std::string cheminFichier);

    void fichierGLPK(std::string cheminFichier);

    bool estLinearisable();

    int algorithmeGlouton(bool simplifier, bool debug);

    int algorithmeGloutonDegreCroissant(bool debug);

    int heuristiqueDenombrement(bool simplifier, int tailleChemin, bool debug);

    int algorithmeExact(bool simplifier, bool debug);

    int algorithmeExactAvecDecoupage(bool simplifier, bool debug);

    int algorithme2approx(bool debug);

    int algorithmeProbabiliste(bool debug, bool start, bool complete);

    int algorithmeProbabilisteAvecDecoupage(bool debug, bool start, bool complete);

    bool testSymetrie();

    std::vector<int> enumeration();

    void test();
};

#endif
