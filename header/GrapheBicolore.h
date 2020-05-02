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

    void activerToutesAretesVertes();

public:
    ///////////////////////////////////////////////////////////////////////////////////////////
    // Les méthodes publiques

    GrapheBicolore(int n);

    GrapheBicolore(std::string cheminFichier);

    GrapheBicolore(int n, std::string cheminFichierNoires, std::string cheminFichierVertes);

    void fichierSage(std::string cheminFichier);

    void fichierTikz(std::string cheminFichier);

    bool estLinearisable();

    int algorithmeGlouton(bool simplifier, bool debug);

    int heuristiqueDenombrement(bool simplifier, int tailleChemin, bool debug);

    int algorithmeExact(bool simplifier, bool debug);

    bool testSymetrie();

    std::vector<int> enumeration();
};

#endif
