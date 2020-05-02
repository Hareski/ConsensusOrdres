#include <iostream>
#include <thread>
#include <fstream>
#include <sys/stat.h>
#include "../header/GrapheBicolore.h"


/**
 * Affiche le temps écoulé depuis start
 * @param start Valeur à initialisé avant la tâche dont le temps est calculé
 */
int affichageTemps(std::chrono::time_point<std::chrono::high_resolution_clock> start) {
    auto stop = std::chrono::high_resolution_clock::now();
    auto diff = stop - start;
    std::cout << "Programme terminé en "
              << std::chrono::duration<double, std::milli>(diff).count()
              << " ms" << std::endl;
}

/**
 * Linéarisation puis énumération d'un graphe dans l'ordre avec l'algorithme glouton,
 * avec l'heuristique déjà existante puis finalement avec un algorithme exact (avec simplification
 * et sans pour chaque algorithme)
 * @param fichierAretesNoires nombre de sommets
 * @param fichierAretesNoires fichier de description des arêtes noires
 * @param fichierAretesVertes fichier de description des arêtes vertes
 */
void testLinearisationEnumeration(int n,
                                  const std::string &fichierAretesNoires,
                                  const std::string &fichierAretesVertes,
                                  const std::string &dossierSauvegarde) {

    if (mkdir(dossierSauvegarde.c_str(), 0777) == -1) {
        std::cout << "Erreur de création de " << dossierSauvegarde << std::endl;
        return;
    }
    std::cout << dossierSauvegarde << " créé." << std::endl;

    // Création du graphe bicolore et sauvegarde du fichier sage
    auto *g = new GrapheBicolore(n, fichierAretesNoires, fichierAretesVertes);
    std::cout << "Le graphe est initialisé avec " << n << " sommets." << std::endl;
    g->fichierSage(dossierSauvegarde + "grapheOrigine.sage.descriptor");
    std::cout << dossierSauvegarde << "grapheOrigine.sage.descriptor créé." << std::endl;

    // Initialisation de l'horloge
    auto start = std::chrono::high_resolution_clock::now();

    // Algorithme Glouton avec simplification
    int gloutonSimple = g->algorithmeGlouton(true, false);
    std::cout << std::endl << "L'algorithme glouton simplifié calcul une solution de " << gloutonSimple << std::endl;
    affichageTemps(start);
    g->fichierSage(dossierSauvegarde + "gloutonSimple.sage.descriptor");
    std::cout << dossierSauvegarde << "gloutonSimple.sage.descriptor créé." << std::endl;

    // Algorithme Glouton sans simplification
    start = std::chrono::high_resolution_clock::now();
    int glouton = g->algorithmeGlouton(false, false);
    std::cout << std::endl << "L'algorithme glouton calcul une solution de " << glouton << std::endl;
    affichageTemps(start);
    g->fichierSage(dossierSauvegarde + "glouton.sage.descriptor");
    std::cout << dossierSauvegarde << "glouton.sage.descriptor créé." << std::endl;

    // Heuristique avec simplification et limité à une taille de 10
    start = std::chrono::high_resolution_clock::now();
    int heuristiqueSimple = g->heuristiqueDenombrement(true, 10, false);
    std::cout << std::endl << "L'heuristique simplifié calcul une solution de " << heuristiqueSimple << std::endl;
    affichageTemps(start);
    g->fichierSage(dossierSauvegarde + "heuristiqueSimple.sage.descriptor");
    std::cout << dossierSauvegarde << "heuristiqueSimple.sage.descriptor créé." << std::endl;

    // Heuristique sans simplification et limité à une taille de 10
    start = std::chrono::high_resolution_clock::now();
    int heuristique = g->heuristiqueDenombrement(false, 10, false);
    std::cout << std::endl << "L'heuristique calcul une solution de " << heuristique << std::endl;
    affichageTemps(start);
    g->fichierSage(dossierSauvegarde + "heuristique.sage.descriptor");
    std::cout << dossierSauvegarde << "heuristique.sage.descriptor créé." << std::endl;

    // Algorithme exact simplifié
    start = std::chrono::high_resolution_clock::now();
    int exactSimple = g->algorithmeExact(true, false);
    std::cout << std::endl << "La valeur optimal est " << exactSimple << std::endl;
    affichageTemps(start);
    g->fichierSage(dossierSauvegarde + "exactSimple.sage.descriptor");
    std::cout << dossierSauvegarde << "exactSimple.sage.descriptor créé." << std::endl;

    // Algorithme exact
    start = std::chrono::high_resolution_clock::now();
    int exact = g->algorithmeExact(false, false);
    std::cout << std::endl << "La valeur optimal est " << exact << std::endl;
    affichageTemps(start);
    g->fichierSage(dossierSauvegarde + "exact.sage.descriptor");
    std::cout << dossierSauvegarde << "exact.sage.descriptor créé." << std::endl;

    // On affiche l'énumération trouvée par le dernier algorithme exact
    const std::vector<int> &enumeration = g->enumeration();
    std::cout << std::endl << "L'énumération trouvé est :" << std::endl;
    for (int e : enumeration) {
        std::cout << e << " ";
    }
    std::cout << std::endl;
}


int main() {

    // Variables
    std::string fichierAretesNoires;
    std::string fichierAretesVertes;
    int n;

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Test 17aretes_simple2 ==========" << std::endl;
    n = 18; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/17aretes_simple2/simple2_aretesNoires";
    fichierAretesVertes = "../data/aretes/17aretes_simple2/simple2_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./test17aretes_simple2/");

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Test 22aretes_g9 ==========" << std::endl;
    n = 22; // Nombre d'arêtes dans le graphe noté de 1 à n
    fichierAretesNoires = "../data/aretes/22aretes_g9/g9_aretesNoires";
    fichierAretesVertes = "../data/aretes/22aretes_g9/g9_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./test22aretes_g9/");

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Test 462aretes_moustique1 ==========" << std::endl;
    n = 462; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/462aretes_moustique1/moustique1_aretesNoires";
    fichierAretesVertes = "../data/aretes/462aretes_moustique1/moustique1_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./test462aretes_moustique1/");

    return 0;
}