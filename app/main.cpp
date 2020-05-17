#include <iostream>
#include <thread>
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

    // Algorithme Glouton
    start = std::chrono::high_resolution_clock::now();
    int glouton = g->algorithmeGlouton(false, true);
    std::cout << std::endl << "L'algorithme glouton calcul une solution de " << glouton << std::endl;
    affichageTemps(start);
    g->fichierSage(dossierSauvegarde + "glouton.sage.descriptor");
    std::cout << dossierSauvegarde << "glouton.sage.descriptor créé." << std::endl;

    // Heuristique limité à une taille de 10
    start = std::chrono::high_resolution_clock::now();
    int heuristique = g->heuristiqueDenombrement(false, 10, false);
    std::cout << std::endl << "L'heuristique limité à 10 calcul une solution de " << heuristique << std::endl;
    affichageTemps(start);
    g->fichierSage(dossierSauvegarde + "heuristiqueLimite10.sage.descriptor");
    std::cout << dossierSauvegarde << "heuristiqueLimite10.sage.descriptor créé." << std::endl;

    // Heuristique
    start = std::chrono::high_resolution_clock::now();
    int heuristiqueNolim = g->heuristiqueDenombrement(false, 50, false);
    std::cout << std::endl << "L'heuristique sans limite calcul une solution de " << heuristiqueNolim << std::endl;
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
    std::cout << "========== Test 1 ==========" << std::endl;
    n = 18; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/1_17sommets/1_aretesNoires";
    fichierAretesVertes = "../data/aretes/1_17sommets/1_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./1_17sommets/");

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Test 2 ==========" << std::endl;
    n = 22; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/2_22sommets/2_aretesNoires";
    fichierAretesVertes = "../data/aretes/2_22sommets/2_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./2_22sommets/");

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Test 3 ==========" << std::endl;
    n = 34; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/3_34sommets/3_aretesNoires";
    fichierAretesVertes = "../data/aretes/3_34sommets/3_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./3_34sommets/");

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Test 4 ==========" << std::endl;
    n = 28; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/4_28sommets/4_aretesNoires";
    fichierAretesVertes = "../data/aretes/4_28sommets/4_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./4_28sommets/");

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Test 5 ==========" << std::endl;
    n = 34; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/5_34sommets/5_aretesNoires";
    fichierAretesVertes = "../data/aretes/5_34sommets/5_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./5_34sommets/");

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Test 6 ==========" << std::endl;
    n = 34; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/6_34sommets/6_aretesNoires";
    fichierAretesVertes = "../data/aretes/6_34sommets/6_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./6_34sommets/");

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Moustique ==========" << std::endl;
    n = 463; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/moustique_462aretes/moustique_aretesNoires";
    fichierAretesVertes = "../data/aretes/moustique_462aretes/moustique_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./moustique_462aretes/");

    ////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "========== Canard ==========" << std::endl;
    n = 1873; // Nombre d'arêtes dans le graphe noté de 0 à n-1
    fichierAretesNoires = "../data/aretes/canard_1873aretes/canard_aretesNoires";
    fichierAretesVertes = "../data/aretes/canard_1873aretes/canard_aretesVertes";
    testLinearisationEnumeration(n, fichierAretesNoires, fichierAretesVertes, "./canard_1873aretes/");
}