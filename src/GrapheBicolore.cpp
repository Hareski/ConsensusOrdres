#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <string>
#include <TableauBinaire.h>
#include <cycleCount.h>
#include "../header/GrapheBicolore.h"

/**
 * Constructeur d'un graphe sans arêtes
 * @param n nombre de sommet dans le graphe
 */
GrapheBicolore::GrapheBicolore(int n) {
    this->n = n;

    for (int i = 0; i < n; ++i) {
        std::vector<Voisin> line;
        voisinsNoirs.push_back(line);
        voisinsVerts.push_back(line);
    }
}


/**
 * Construit le graphe à partir d'un fichier de description
 * @param cheminFichier chemin du fichier
 */
GrapheBicolore::GrapheBicolore(std::string cheminFichier) {
    std::ifstream flux(cheminFichier.c_str());
    if (flux) {
        std::string ligne;
        getline(flux, ligne);
        this->n = std::stoi(ligne);
        for (int i = 0; i < n; ++i) {
            std::vector<Voisin> line;
            voisinsNoirs.push_back(line);
            voisinsVerts.push_back(line);
        }

        while (getline(flux, ligne)) {
            int sommet = std::stoi(ligne);
            getline(flux, ligne);
            int i = 0;
            int j = 0;
            while (i < ligne.size()) {
                j = 0;
                while (ligne.at(i + j) != ':') {
                    j++;
                }
                int voisin = std::stoi(ligne.substr(i, j));
                j++;
                if (ligne.at(i + j) == 'v') {
                    this->ajouterAreteVerte(sommet, voisin);
                } else {
                    this->ajouterAreteNoire(sommet, voisin);
                }
                i += (j + 2);
            }
        }
        flux.close();
    } else {
        throw std::invalid_argument("impossible d'ouvrir le fichier");
    }
}

/**
 * Construit le graphe à partir de deux fichier de description des arêtes
 * @param n nombre de sommets
 * @param cheminFichierNoires chemin vers le fichier de description des arêtes noires
 * @param cheminFichierVertes chemin vers le fichier de description des arêtes vertes
 * @remark les fichiers ont une arêtes par ligne, avec l'identifiant du premier sommet, un espace
 *         puis l'identifiant du second sommet
 */
GrapheBicolore::GrapheBicolore(int n, std::string cheminFichierNoires, std::string cheminFichierVertes) {
    this->n = n;
    for (int i = 0; i < n; ++i) {
        std::vector<Voisin> line;
        voisinsNoirs.push_back(line);
        voisinsVerts.push_back(line);
    }
    std::ifstream fluxNoires(cheminFichierNoires.c_str());
    if (fluxNoires) {
        std::string ligne;
        while (getline(fluxNoires, ligne)) {
            int i = 0;
            while (ligne.at(i) != ' ') { i++; }
            int sommet = std::stoi(ligne.substr(0, i));
            int voisin = std::stoi(ligne.substr(i + 1, ligne.size()));
            ajouterAreteNoire(sommet, voisin);
        }
        fluxNoires.close();
    } else {
        throw std::invalid_argument("impossible d'ouvrir le fichier " + cheminFichierNoires);
    }
    std::ifstream fluxVertes(cheminFichierVertes.c_str());
    if (fluxVertes) {
        std::string ligne;
        while (getline(fluxVertes, ligne)) {
            int i = 0;
            while (ligne.at(i) != ' ') { i++; }
            int sommet = std::stoi(ligne.substr(0, i));
            int voisin = std::stoi(ligne.substr(i + 1, ligne.size()));
            ajouterAreteVerte(sommet, voisin);
        }
        fluxVertes.close();
    } else {
        throw std::invalid_argument("impossible d'ouvrir le fichier " + cheminFichierVertes);
    }
}

/**
 * @param s un sommet
 * @return retourne les voisins verts actifs du sommet s
 */
std::vector<int> GrapheBicolore::voisinsVertsActifs(int s) {
    std::vector<int> voisinsActifs;
    for (auto voisin: voisinsVerts.at(s)) {
        if (voisin.active) {
            voisinsActifs.push_back(voisin.sommet);
        }
    }
    return voisinsActifs;
}

/**
 * @param s un sommet
 * @return retourne les voisins noirs actifs du sommet s
 */
std::vector<int> GrapheBicolore::voisinsNoirsActifs(int s) {
    std::vector<int> voisinsActifs;
    for (auto voisin: voisinsNoirs.at(s)) {
        if (voisin.active) {
            voisinsActifs.push_back(voisin.sommet);
        }
    }
    return voisinsActifs;
}

/**
 * Ajoute une arête verte active au graphe
 * @param sommet1 un sommet de l'arête
 * @param sommet2 l'autre sommet de l'arête
 * @return retourne faux si l'arête existe dans le graphe (active ou non), vrai sinon
 */
bool GrapheBicolore::ajouterAreteVerte(int sommet1, int sommet2) {
    for (auto voisin: voisinsVerts.at(sommet1)) {
        if (voisin.sommet == sommet2) { return false; }
    }
    listeAretesVertes.push_back({sommet1, sommet2});
    voisinsVerts.at(sommet1).push_back({sommet2, true});
    voisinsVerts.at(sommet2).push_back({sommet1, true});
    return true;
}

/**
 * Active une arête verte du graphe
 * @param sommet1 un sommet de l'arête
 * @param sommet2 l'autre sommet de l'arête
 */
void GrapheBicolore::activerAreteVerte(int sommet1, int sommet2) {
    for (auto &voisin: voisinsVerts.at(sommet1)) {
        if (voisin.sommet == sommet2) { voisin.active = true; }
    }
    for (auto &voisin: voisinsVerts.at(sommet2)) {
        if (voisin.sommet == sommet1) { voisin.active = true; }
    }
}

/**
 * Désactive une arête verte du graphe
 * @param sommet1 un sommet de l'arête
 * @param sommet2 l'autre sommet de l'arête
 */
void GrapheBicolore::desactiverAreteVerte(int sommet1, int sommet2) {
    for (auto &voisin: voisinsVerts.at(sommet1)) {
        if (voisin.sommet == sommet2) { voisin.active = false; }
    }
    for (auto &voisin: voisinsVerts.at(sommet2)) {
        if (voisin.sommet == sommet1) { voisin.active = false; }
    }
}

/**
 * Ajoute une arête noire au graphe
 * @param sommet1 un sommet de l'arête
 * @param sommet2 l'autre sommet de l'arête
 * @return retourne faux si l'arête est déjà activé dans le graphe, vrai sinon
 */
bool GrapheBicolore::ajouterAreteNoire(int sommet1, int sommet2) {
    if (testerAreteNoireActive(sommet1, sommet2)) {
        return false;
    }
    if ((degreNoir(sommet1) + degreNoir(sommet2) == 1) || degreNoirMax() == 0) {
        voisinsNoirs.at(sommet1).push_back({sommet2, true});
        voisinsNoirs.at(sommet2).push_back({sommet1, true});
        ajouterSommetNoir(sommet1);
        ajouterSommetNoir(sommet2);
        return true;
    }
    return false;
}

/**
 * Creer un fichier pour la visualisation du graphe avec Sage
 * @param cheminFichier chemin du fichier pour la sauvegarde
 */
void GrapheBicolore::fichierSage(std::string cheminFichier) {
    std::ofstream flux(cheminFichier.c_str());
    if (flux) {
        for (int i = 0; i < n; ++i) {
            flux << i << std::endl; // Nom du sommet
            for (auto j: voisinsNoirsActifs(i)) {
                flux << j << ":n;"; // Arête noire
            }
            for (auto j: voisinsVertsActifs(i)) {
                flux << j << ":v;"; // Arête verte
            }
            if (i < n - 1) {
                flux << std::endl;
            }
        }
    } else {
        throw std::invalid_argument("impossible d'écrire le fichier");
    }
}

/**
 * Creer un fichier pour la visualisation du graphe avec Tikz en simple cercle
 * @param cheminFichier chemin du fichier pour la sauvegarde
 */
void GrapheBicolore::fichierTikz(std::string cheminFichier) {
    std::ofstream flux(cheminFichier.c_str());
    if (flux) {
        flux << "\\documentclass[tikz]{standalone}" << std::endl;
        flux << "\\begin{document}" << std::endl;
        flux << "   \\begin{tikzpicture}[thick,scale=.9, every node/.style={transform shape}]" << std::endl;
        flux << "       \\tikzset{greenvertex/.style = {shape=circle, minimum size=2.3em, fill = green}}"
             << std::endl;
        flux << "       \\tikzset{blackvertex/.style = {shape=circle,draw, minimum size=2.3em}}" << std::endl;
        flux << "" << std::endl;
        for (int k = 0; k < n; ++k) {
            if (voisinsVertsActifs(k).empty()) {
                flux << "       \\node[blackvertex] (" << k << ") at  (360/" << n << " *" << k << ":" << n / 5
                     << "cm) {$" << k
                     << "$};"
                     << std::endl;
            } else if (voisinsNoirsActifs(k).empty()) {
                flux << "       \\node[greenvertex] (" << k << ") at  (360/" << n << " *" << k << ":" << n / 5
                     << "cm) {$" << k
                     << "$};"
                     << std::endl;
            } else {
                flux << "       \\node[blackvertex, greenvertex] (" << k << ") at  (360/" << n << " *" << k
                     << ":" << n / 5 << "cm) {$" << k
                     << "$};"
                     << std::endl;
            }
        }
        for (int i = 0; i < n; ++i) {
            for (auto j: voisinsNoirsActifs(i)) {
                flux << "       \\draw (" << i << ") -- (" << j << ");" << std::endl;
            }
            for (auto j: voisinsVertsActifs(i)) {
                flux << "       \\draw[green] (" << i << ") -- (" << j << ");" << std::endl;
            }
        }
        flux << "   \\end{tikzpicture}" << std::endl;
        flux << "\\end{document}" << std::endl;
        flux << std::endl;
    } else {
        throw std::invalid_argument("impossible d'ouvrir le fichier.");
    }
}

/**
 * @param a un premier sommet
 * @param b un autre sommet
 * @return Vrai s'il existe une arête verte entre a et b
 */
bool GrapheBicolore::testerAreteVerteActive(int a, int b) {
    for (auto voisin: voisinsVerts.at(a)) {
        if (voisin.active && voisin.sommet == b) { return true; }
    }
    return false;
}

/**
 * @param a un premier sommet
 * @param b un autre sommet
 * @return Vrai s'il existe une arête noire entre a et b
 */
bool GrapheBicolore::testerAreteNoireActive(int a, int b) {
    for (auto voisin: voisinsNoirs.at(a)) {
        if (voisin.active && voisin.sommet == b) { return true; }
    }
    return false;
}

bool GrapheBicolore::estLinearisable() {
    // Si le degré vert maximum est supérieur à 2, le graphe n'est pas linéaire
    if (degreVertMax() > 2) { return false; }
    // S'il contient des cycles verts, le graphe n'est pas linéaire
    if (cycleVert()) { return false; }
    // S'il contient des mauvais cycles, le graphe n'est pas linéaire
    if (mauvaisCycle()) { return false; }
    // Si aucune des conditions précédentes n'est vrai, alors il est linéaire
    return true;
}

/**
 * @param s un sommet
 * @return degré noir du sommet
 */
int GrapheBicolore::degreNoir(int s) {
    return voisinsNoirs.at(s).size();
}

/**
 * @param s un sommet
 * @return degré vert du sommet
 */
int GrapheBicolore::degreVert(int s) {
    return voisinsVertsActifs(s).size();
}

/**
 * @return le degré noir maximum du graphe
 */
int GrapheBicolore::degreNoirMax() {
    int res = 0;
    for (int i = 0; i < n; ++i) {
        res = std::max(degreNoir(i), res);
    }
    return res;
}

/**
 * @return le degré vert maximum du graphe
 */
int GrapheBicolore::degreVertMax() {
    int res = 0;
    for (int i = 0; i < n; ++i) {
        res = std::max(degreVert(i), res);
    }
    return res;
}

/**
 * @return vrai si le graphe contient un cycle vert, faux sinon
 */
bool GrapheBicolore::cycleVert() {
    std::queue<int> file;
    std::vector<int> niveaux;
    niveaux.reserve(n);
    for (int i = 0; i < n; ++i) {
        niveaux.push_back(-1);
    }

    // Parcours du graphe
    int sommetNul = 0;
    while (sommetNul < niveaux.size()) {
        niveaux.at(sommetNul) = 0;
        file.push(sommetNul);
        while (!file.empty()) {
            int sommet = file.front();
            file.pop();
            int niv = niveaux.at(sommet);
            for (int &voisin : voisinsVertsActifs(sommet)) {
                if (niveaux.at(voisin) == -1) {
                    niveaux.at(voisin) = niv + 1;
                    file.push(voisin);
                } else if (niveaux.at(voisin) >= niv) {
                    return true;
                }
            }
        }
        while (sommetNul < niveaux.size() && niveaux.at(sommetNul) != -1) {
            sommetNul++;
        }
    }
    return false;
}

/**
 * @return liste des sommets noirs
 */
std::vector<int> GrapheBicolore::getSommetsNoirs() {
    return sommetsNoirs;
}

/**
 * Rend le prochain sommet noir sous l'hypothèse que la composante
 * connexe contenant le sommet actuel est une chaine.
 * @param precedent le sommet déjà exploré
 * @param actuel le sommet en cours d'exploration
 * @return le prochain sommet à explorer
 */
int GrapheBicolore::prochainSommetNoir(int precedent, int actuel) {
    if (voisinsNoirsActifs(actuel).at(0) == precedent) {
        if (voisinsNoirs.at(actuel).size() < 2) {
            return -1;
        } else {
            return voisinsNoirsActifs(actuel).at(1);
        }
    } else {
        return voisinsNoirsActifs(actuel).at(0);
    }
}

/**
 * Rend le prochain sommet vert sous l'hypothèse que le graphe vert est une chaine.
 * @param precedent le sommet déjà exploré
 * @param actuel le sommet en cours d'exploration
 * @return le prochain sommet à explorer ou -1 s'il s'agit d'une extrémité
 */
int GrapheBicolore::prochainSommetVert(int precedent, int actuel) {
    if (voisinsVertsActifs(actuel).at(0) == precedent) {
        if (voisinsVertsActifs(actuel).size() < 2) {
            return -1;
        } else {
            return voisinsVertsActifs(actuel).at(1);
        }
    } else {
        return voisinsVertsActifs(actuel).at(0);
    }
}

/**
 * @return liste des sommets noirs dans l'ordre de la chaine
 * @invariant il faut s'assurer que les sommets noirs forment une chaine avant d'executer cette méthode
 */
std::vector<int> GrapheBicolore::getSommetsNoirsParAdjacence() {
    int i = 0;
    std::vector<int> res;
    if (sommetsNoirs.empty()) {
        return res;
    }
    while (degreNoir(sommetsNoirs.at(i)) > 1) { // TODO : segfault possible
        i++;
    }
    int s1 = sommetsNoirs.at(i);
    int s2 = voisinsNoirsActifs(s1).at(0);
    res.push_back(s1);
    while (s2 != -1) {
        int temp = s2;
        s2 = prochainSommetNoir(s1, s2);
        s1 = temp;
        res.push_back(s1);
    }
    return res;
}

// TODO: a supprimer
bool GrapheBicolore::testSymetrie() {
    for (int i = 0; i < n; ++i) {
        for (auto v1 : voisinsVerts.at(i)) {
            bool good = false;
            for (auto v2: voisinsVerts.at(v1.sommet)) {
                if (v2.sommet == i && v1.active == v2.active) {
                    good = true;
                }
            }
            if (!voisinsVerts.at(v1.sommet).empty() && !good) {
                throw std::runtime_error("symétrie non-respecté");
            }
        }
    }
}

/**
 * @return vrai si le graphe contient un mauvais cycle, faux sinon
 */
bool GrapheBicolore::mauvaisCycle() {
    testSymetrie();
    std::vector<int> P = getSommetsNoirsParAdjacence();
    std::vector<bool> dejaVu;
    dejaVu.reserve(n);
    for (int i = 0; i < n; i++) {
        dejaVu.push_back(false);
    }

    for (int &s_noir : P) {
        for (int &voisin_vert : voisinsVertsActifs(s_noir)) {
            if (!dejaVu.at(voisin_vert)) {
                int s_vert_1 = s_noir;
                int s_vert_2 = voisin_vert;
                while (s_vert_2 != -1 && !estSommetNoir(s_vert_2)) {
                    dejaVu.at(s_vert_2) = true;
                    int temp = s_vert_2;
                    s_vert_2 = prochainSommetVert(s_vert_1, s_vert_2);
                    s_vert_1 = temp;
                }
                if (s_vert_2 != -1 && !testerAreteNoireActive(s_noir, s_vert_2)) {
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * @param s un sommet
 * @return vrai si le sommet s est noir, faux sinon
 */
bool GrapheBicolore::estSommetNoir(int s) {
    if (sommetsNoirs.empty()) {
        return false;
    }

    // Recherche dichotomique
    int deb = 0;
    int fin = sommetsNoirs.size() - 1;
    while (deb <= fin) {
        int milieu = (deb + fin) / 2;
        if (sommetsNoirs.at(milieu) > s) {
            fin = milieu - 1;
        } else if (sommetsNoirs.at(milieu) < s) {
            deb = milieu + 1;
        } else {
            return true;
        }
    }
    return false;
}

/**
 * Ajoute un sommet à la liste des sommets noirs
 * @param s le sommet à ajouter noir
 * @return faux si le sommet et déjà noir, faux sinon
 */
bool GrapheBicolore::ajouterSommetNoir(int s) {
    // La liste est vide
    if (sommetsNoirs.empty()) {
        sommetsNoirs.push_back(s);
        return true;
    }

    // Sinon recherche dichotomique
    int deb = 0;
    int fin = sommetsNoirs.size() - 1;
    while (deb <= fin) {
        int milieu = (deb + fin) / 2;
        if (sommetsNoirs.at(milieu) > s) {
            fin = milieu - 1;
        } else if (sommetsNoirs.at(milieu) < s) {
            deb = milieu + 1;
        } else {
            return false;
        }
    }
    sommetsNoirs.insert(sommetsNoirs.begin() + deb, s);
    return true;
}

/**
 * @return ensemble des arêtes vertes active ou non du graphe
 */
std::vector<GrapheBicolore::Arete> GrapheBicolore::aretesVertes() {
    return listeAretesVertes;
}

/**
 * Selection d'un sous-ensemble d'aretes vertes telle qu'un des ensembles d'arêtes vertes dont la suppression
 * rend le graphe linéarisable soit inclus dans ce sous-ensemble.
 * @return liste d'arêtes vertes
 * @remark retourne actuellement l'ensemble des arêtes vertes incidentes à un sommet noir ou
 * incidente à un sommet de degré supérieur à 2
 */
std::vector<GrapheBicolore::Arete> GrapheBicolore::selectionAretesVertes() {
    std::vector<GrapheBicolore::Arete> listeInitiale = listeAretesVertes;
    std::vector<GrapheBicolore::Arete> selection;
    for (Arete arete : listeAretesVertes) {
        if (estSommetNoir(arete.x) || estSommetNoir(arete.y)
            || degreVert(arete.x) > 2 || degreVert(arete.y) > 2) {
            selection.push_back(arete);
        }
    }
    return selection;
}

/**
 * @return ensemble des arêtes vertes active du graphe
 */
std::vector<GrapheBicolore::Arete> GrapheBicolore::aretesVertesActives() {
    std::vector<GrapheBicolore::Arete> aretes;
    for (Arete &arete: aretesVertes()) {
        if (testerAreteVerteActive(arete.x, arete.y)) {
            aretes.push_back(arete);
        }
    }
    return aretes;
}

/**
 * Réactive toutes les arêtes du graphe
 */
void GrapheBicolore::activerToutesAretesVertes() {
    for (Arete &arete: aretesVertes()) {
        activerAreteVerte(arete.x, arete.y);
    }
}

/**
 * Génère la matrice d'adjacence des arêtes vertes actives
 * @param tolererEquivalentesNoires faux si l'on ne dois pas considérer les arêtes vertes
 * incidentes à i et j telle que il existe une arête noire incidente aussi à i et j, vrai sinon
 * @return la matrice d'adjacence
 */
std::vector<std::vector<double>> GrapheBicolore::matriceAdjacenceActifsVerte(bool tolererEquivalentesNoires) {
    std::vector<double> ligne(n, 0);
    std::vector<std::vector<double>> matrice(n, ligne);
    for (int a = 0; a < n; ++a) {
        for (int b : voisinsVertsActifs(a)) {
            if (tolererEquivalentesNoires || !testerAreteNoireActive(a, b)) {
                matrice[a][b] = 1;
            }
        }
    }
    return matrice;
}

/**
 * Dénombre les chemins simples verts
 * @param tailleMax taille maximale des chemins à considérer
 * @param tolererEquivalentesNoires faux si l'on ne dois pas considérer les arêtes vertes
 * incidentes à i et j telle que il existe une arête noire incidente aussi à i et j, vrai sinon
 * @return une matrice de tous les chemins verts avec return[i][j] le nombre de chemin de i à j
 */
std::vector<std::vector<double>> GrapheBicolore::denombrerCheminsVerts(int tailleMax, bool tolererEquivalentesNoires) {
    std::vector<std::vector<double>> matriceVerte = matriceAdjacenceActifsVerte(tolererEquivalentesNoires);
    std::vector<std::vector<std::vector<double>>> compteursAvecTaille = pathsCount(matriceVerte, tailleMax, false);
    std::vector<double> ligne(n, 0);
    std::vector<std::vector<double>> compteurs(n, ligne);
    for (int taille = 1; taille < compteursAvecTaille.size(); ++taille) {
        for (int a = 0; a < n; ++a) {
            for (int b = 0; b < n; ++b) {
                compteurs[a][b] += compteursAvecTaille[taille][a][b];
            }
        }
    }
    return compteurs;
}

/**
 * Dénombre les mauvais cycles selon la chaine des sommets noirs
 * @param tailleMax taille maximale des chemins à considérer
 * @param sommetsNoirs chaine des sommets noirs
 * @return nombre de mauvais cycles
 */
double GrapheBicolore::denombrerMauvaisCycles(int tailleMax, std::vector<int> sommetsNoirs) {
    // Construction du tableau des chemins
    std::vector<std::vector<double>> matriceVerte = matriceAdjacenceActifsVerte(false);
    std::vector<std::vector<std::vector<double>>> compteursAvecTaille = pathsCount(matriceVerte, tailleMax, true);

    // Compteur par segments
    double compteur = 0;
    for (int tailleSegment = 2; tailleSegment < sommetsNoirs.size(); ++tailleSegment) {
        for (int s = 0; s < sommetsNoirs.size() - tailleSegment; ++s) {
            for (int tailleChemin = 0; tailleChemin < compteursAvecTaille.size(); ++tailleChemin) {
                compteur += compteursAvecTaille[tailleChemin][sommetsNoirs[s]][sommetsNoirs[s + tailleSegment]];
            }
        }
    }
    return compteur;
}

/**
 * Algorithme glouton naif et déterministe pour la linéarisation
 * @param simplifier vrai pour réduire le nombre d'arêtes vertes considérées, la solution reste optimal
 * @param debug vrai pour obtenir un affichage des étapes
 * @return le nombre d'arête désactivés pour obtenir un graphe linéarisable
 * @remark modifie l'êtat des arêtes du graphe durant son execution pour obtenir un graphe linéarisable,
 *         on peut alors afficher le graphe pour obtenir le résultat
 */
int GrapheBicolore::algorithmeGlouton(bool simplifier, bool debug) {
    // Initialisation de la liste des arêtes
    activerToutesAretesVertes(); // Réactivation avant algorithme
    std::vector<Arete> aretes;
    if (simplifier) {
        aretes = selectionAretesVertes();
    } else {
        aretes = aretesVertes();
    }

    // On retire les arêtes dans l'ordre de la liste
    for (int nombreAretesRetirees = 1; nombreAretesRetirees < aretes.size() + 1; ++nombreAretesRetirees) {
        desactiverAreteVerte(aretes[nombreAretesRetirees - 1].x, aretes[nombreAretesRetirees - 1].y);
        if (debug) {
            std::cout << "L'arête numéro " << nombreAretesRetirees - 1 << " de " << aretes[nombreAretesRetirees - 1].x
                      << " à " << aretes[nombreAretesRetirees - 1].y << " est retirée" << std::endl;
        }
        if (estLinearisable()) {
            return nombreAretesRetirees;
        }
    }
    // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
    throw std::runtime_error("aucun résultat, sans arêtes vertes le graphe n'est pas linéaire");
}

/**
 * Heuristique utilisant le dénombrement des chemins verts et des mauvais cycles
 * @param simplifier vrai pour réduire le nombre d'arêtes vertes considérées
 * @param tailleChemin taille maximum des chemins à dénombrer
 * @param debug vrai pour obtenir un affichage des étapes
 * @return le nombre d'arête désactivés pour obtenir un graphe linéarisable
 * @remark modifie l'êtat des arêtes du graphe durant son execution pour obtenir un graphe linéarisable,
 *         on peut alors afficher le graphe pour obtenir le résultat
 */
int GrapheBicolore::heuristiqueDenombrement(bool simplifier, int tailleChemin, bool debug) {
    // Initialisation de la liste des arêtes et des valeurs initiales
    activerToutesAretesVertes(); // Réactivation avant algorithme
    std::vector<Arete> aretes;
    if (simplifier) {
        aretes = selectionAretesVertes();
    } else {
        aretes = aretesVertes();
    }
    std::vector<int> chaineSommetsNoirs = getSommetsNoirsParAdjacence();
    double compteurMauvaisCycles = denombrerMauvaisCycles(tailleChemin, chaineSommetsNoirs);
    std::vector<std::vector<double>> compteurCheminsVerts = denombrerCheminsVerts(tailleChemin, true);
    double poids[aretes.size()];

    // Calculs des poids
    for (int i = 0; i < aretes.size(); ++i) {
        poids[i] = 0;
        // Degré vert
        int degre = degreVert(aretes[i].x);
        if (degre > 2) { poids[i] = degre - 2; }
        degre = degreVert(aretes[i].y);
        if (degre > 2) { poids[i] += degre - 2; }
        // Cycle vert
        poids[i] += compteurCheminsVerts[aretes[i].x][aretes[i].y];
        // Mauvais cycles
        desactiverAreteVerte(aretes[i].x, aretes[i].y);
        double mauvaisCyclesArete =
                compteurMauvaisCycles - denombrerMauvaisCycles(tailleChemin, chaineSommetsNoirs);
        poids[i] += mauvaisCyclesArete;
        activerAreteVerte(aretes[i].x, aretes[i].y);
        if (debug) {
            std::cout << "L'arête numéro " << i << " de " << aretes[i].x << " à " << aretes[i].y
                      << " a un poids de " << poids[i] << " avec " <<
                      compteurCheminsVerts[aretes[i].x][aretes[i].y] << " cycle(s) vert(s) et "
                      << mauvaisCyclesArete << " mauvais cycle(s)." << std::endl;
        }
    }

    // On retire les arêtes par ordre de poids
    for (int nombreAretesRetirees = 1; nombreAretesRetirees < aretes.size(); ++nombreAretesRetirees) {
        int indiceMaximum = -1;
        double poidsMaximum = -1;
        for (int j = 0; j < aretes.size(); ++j) {
            if (poids[j] > poidsMaximum) {
                indiceMaximum = j;
                poidsMaximum = poids[j];
            }
        }
        desactiverAreteVerte(aretes[indiceMaximum].x, aretes[indiceMaximum].y);
        if (debug) {
            std::cout << "L'arête numéro " << indiceMaximum << " de " << aretes[indiceMaximum].x << " à "
                      << aretes[indiceMaximum].y << " est retirée" << std::endl;
        }
        if (estLinearisable()) {
            return nombreAretesRetirees;
        }
        poids[indiceMaximum] = -1;
    }
    // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
    throw std::runtime_error("aucun résultat, sans arêtes vertes le graphe n'est pas linéaire");
}

/**
 * Algorithme exact qui teste toutes les combinaisons d'activation et de désactivation d'arêtes vertes
 * @param simplifier vrai pour réduire le nombre d'arêtes vertes considérées, la solution reste optimal
 * @param debug vrai pour obtenir un affichage des étapes
 * @return le nombre d'arête désactivés pour obtenir un graphe linéarisable
 * @remark modifie l'êtat des arêtes du graphe durant son execution pour obtenir un graphe linéarisable,
 *         on peut alors afficher le graphe pour obtenir le résultat
 */
int GrapheBicolore::algorithmeExact(bool simplifier, bool debug) {
    // Initialisation de la liste des arêtes
    activerToutesAretesVertes(); // Réactivation avant algorithme
    std::vector<Arete> aretes;
    if (simplifier) {
        aretes = selectionAretesVertes();
    } else {
        aretes = aretesVertes();
    }

    const int nombreAretesVertes = aretes.size();
    for (int nombreAretesRetirees = 0; nombreAretesRetirees <= nombreAretesVertes; ++nombreAretesRetirees) {
        // Initialisation du tableau binaire des arêtes à ajouter (=faux) ou non (=vrai)
        auto *tableauBinaireAretes = new TableauBinaire(nombreAretesVertes, nombreAretesRetirees);

        // Boucle sur la cardinalité en commençant par retiré le moins d'arêtes possible
        do {
            // Génération du vecteur d'arêtes vertes pour l'instance à tester selon tableauBinaireAretes
            for (auto aretesVerte : aretes) {
                bool val = tableauBinaireAretes->valeurSuivante(); // On récupère la prochaine valeur
                if (debug) { std::cout << val; }
                if (!val) { activerAreteVerte(aretesVerte.x, aretesVerte.y); }
                else { desactiverAreteVerte(aretesVerte.x, aretesVerte.y); }
            }
            if (debug) { std::cout << std::endl; }
            // Test de la linéarité de l'instance courante
            if (estLinearisable()) {
                if (debug) {
                    for (auto aretesVerte : aretes) {
                        // Le tableau binaire boucle une fois la dernière valeur obtenue
                        // On aura donc les mêmes valeurs que précédemment
                        bool val = tableauBinaireAretes->valeurSuivante();
                        if (val) {
                            std::cout << "L'arête de " << aretesVerte.x << " à "
                                      << aretesVerte.y << " est retirée dans la solution" << std::endl;
                        }
                    }
                }
                estLinearisable();
                return nombreAretesRetirees;
            }
        } while (tableauBinaireAretes->genererProchain()); // Sinon on cherche le prochain
        // Boucle sur la cardinalité en commençant par retiré le moins d'arêtes possible
    }
    // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
    throw std::runtime_error("aucun résultat, sans arêtes vertes le graphe n'est pas linéaire");
}

/**
 * Donne l'énumération d'un graphe linéarisable
 * @return liste donnant l'énumération
 */
std::vector<int> GrapheBicolore::enumeration() {
    if (!estLinearisable()) {
        throw std::runtime_error("énumération d'un graphe non linéarisé");
    }
    const std::vector<int> &chaineNoir = getSommetsNoirsParAdjacence();

    std::vector<int> listeEnum;
    listeEnum.reserve(n);
    bool dejaVu[n];
    for (int i = 0; i < n; ++i) {
        dejaVu[i] = false;
    }

    for (int sommetNoir : chaineNoir) {
        const std::vector<int> &voisins = voisinsVertsActifs(sommetNoir);
        std::vector<int> chaine[voisins.size()]; // Maximum 2
        bool premiereGauche = true;
        if (!voisins.empty()) { // Le sommet noir à des voisins verts
            // On boucle 1 ou 2 fois selon le nombre de voisins verts
            for (int numeroChaine = 0; numeroChaine < voisins.size(); ++numeroChaine) {
                int courant = voisins[numeroChaine]; // Le voisin est choisi arbitrairement
                int precedent = sommetNoir; // Son premier prédécesseur est le sommet noir
                while (courant != -1 && !dejaVu[courant] && !estSommetNoir(courant)) {
                    chaine[numeroChaine].push_back(courant);
                    dejaVu[courant] = true;
                    int suivant = prochainSommetVert(precedent, courant);
                    precedent = courant;
                    courant = suivant;
                }
                // Si on le tant que s'arrête avec un sommet noir alors la chaine doit être à droite
                if (numeroChaine == 0 && estSommetNoir(courant)) {
                    premiereGauche = false;
                }
            }
            dejaVu[sommetNoir] = true;
            if (premiereGauche) { // La chaine[0] doit être à gauche et retournée
                for (int j = chaine[0].size() - 1; j >= 0; --j) {
                    listeEnum.push_back(chaine[0][j]);
                }
                listeEnum.push_back(sommetNoir);
                if (voisins.size() == 2) {
                    for (int k : chaine[1]) {
                        listeEnum.push_back(k);
                    }
                }
            } else { // La chaine[1] doit être à gauche et retournée (elle peut être vide aussi)
                if (voisins.size() == 2) {
                    for (int j = chaine[1].size() - 1; j >= 0; --j) {
                        listeEnum.push_back(chaine[1][j]);
                    }
                }
                listeEnum.push_back(sommetNoir);
                for (int k : chaine[0]) {
                    listeEnum.push_back(k);
                }
            }
        } else { // Le sommet noir n'a pas de voisin
            listeEnum.push_back(sommetNoir);
        }
    }
    return listeEnum;
}