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
 * Creer un fichier pour la résolution avec un solver (format lp)
 * @param cheminFichier chemin du fichier pour la sauvegarde
 */
void GrapheBicolore::fichierGLPK(std::string cheminFichier){
    std::ofstream flux(cheminFichier.c_str());
    if (flux) {
    	std::vector<std::string> greenEdgesVars;
    	std::vector<std::string> vertexVars;
    	std::vector<std::string> vertexCompVars;

        flux << "Minimize" <<std::endl;
        flux << "\ts: ";
        for(int i=0; i<this->aretesVertes().size(); i++){
        	std::string var = "y_" + std::to_string(this->aretesVertes()[i].x) + "_" + std::to_string(this->aretesVertes()[i].y);
        	greenEdgesVars.push_back(var);
        	if(i>0){
        		flux << " + ";
        	}
        	flux << var;
        }
        flux << std::endl << std::endl << "Subject To" << std::endl;
        int constraint_count = 0;
        for(int i=0; i<n; i++){
        	std::string var_x_i = "x_" + std::to_string(i);
        	vertexVars.push_back(var_x_i);
    		constraint_count++;
    		flux << "\tc" << constraint_count << ": " << var_x_i << " >= 1" << std::endl;
    		constraint_count++;
    		flux << "\tc" << constraint_count << ": " << var_x_i << " <= " << n << std::endl;
        	for(int j=i+1; j<n; j++){
        		std::string var_x_j = "x_" + std::to_string(j);
        		std::string var_z_i_j = "z_"+ std::to_string(i) + "_" + std::to_string(j);
        		vertexCompVars.push_back(var_z_i_j);
        		constraint_count++;
        		flux << "\tc" << constraint_count << ": " << var_x_i << " - " << var_x_j <<  " + " << n << var_z_i_j << " >= 1" << std::endl;
        		constraint_count++;
        		flux << "\tc" << constraint_count << ": " << var_x_j << " - " << var_x_i <<  " - " << n <<  var_z_i_j << " >= " << 1-n << std::endl;
        	}
        }

        std::vector<int> sommets_noirs = this->getSommetsNoirsParAdjacence();
        for(int i=0; i<sommets_noirs.size()-1; i++){
        	if(sommets_noirs[i] < sommets_noirs[i+1]){
        		std::string var_z = "z_"+ std::to_string(sommets_noirs[i]) + "_" + std::to_string(sommets_noirs[i+1]);
        		constraint_count++;
        		flux << "\tc" << constraint_count << ": " << var_z << " = 1" << std::endl;
        	}
        	else{
        		std::string var_z = "z_"+ std::to_string(sommets_noirs[i+1]) + "_" + std::to_string(sommets_noirs[i]);
        		constraint_count++;
        		flux << "\tc" << constraint_count << ": " << var_z << " = 0" << std::endl;
        	}
        }

        for(int i=0; i<this->aretesVertes().size(); i++){
        	std::string var_y_i_j = "y_" + std::to_string(this->aretesVertes()[i].x) + "_" + std::to_string(this->aretesVertes()[i].y);
        	std::string var_x_i = "x_" + std::to_string(this->aretesVertes()[i].x);
        	std::string var_x_j = "x_" + std::to_string(this->aretesVertes()[i].y);
    		constraint_count++;
    		flux << "\tc" << constraint_count << ": " << var_x_i << " - " << var_x_j << " - " << n << var_y_i_j << " <= 1" << std::endl;
    		constraint_count++;
    		flux << "\tc" << constraint_count << ": " << var_x_j << " - " << var_x_i << " - " << n << var_y_i_j << " <= 1" << std::endl;
        }
        flux << std::endl << "Binary"<<std::endl;
        for(int i=0; i<vertexCompVars.size(); i++){
			flux << "\t" << vertexCompVars[i] << std::endl;
		}
        for(int i=0; i<greenEdgesVars.size(); i++){
			flux << "\t" << greenEdgesVars[i] << std::endl;
		}
        flux << std::endl << "End" << std::endl;

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

/**
 * @return Vrai si le graphe est linéarisable
 */
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
 * @return degré vert actif du sommet
 */
int GrapheBicolore::degreVert(int s) {
    int degre = 0;
    for (auto voisin: voisinsVerts.at(s)) {
        degre += voisin.active;
    }
    return degre;
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

/**
 * @return vrai si le graphe contient un mauvais cycle, faux sinon
 */
bool GrapheBicolore::mauvaisCycle() {
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
    std::vector<GrapheBicolore::Arete> listeInitiale = this->aretesVertesActives();
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
 * @return ensemble des arêtes vertes actives du graphe
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
 * @return ensemble des arêtes vertes non-actives du graphe
 */
std::vector<GrapheBicolore::Arete> GrapheBicolore::aretesVertesDesactives() {
    std::vector<GrapheBicolore::Arete> aretes;
    for (Arete &arete: aretesVertes()) {
        if (!testerAreteVerteActive(arete.x, arete.y)) {
            aretes.push_back(arete);
        }
    }
    return aretes;
}

/**
 * Désactive toutes les arêtes du graphe
 */
void GrapheBicolore::desactiverToutesAretesVertes() {
    for (Arete &arete: aretesVertes()) {
        desactiverAreteVerte(arete.x, arete.y);
    }
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
 * Active toutes les arêtes vertes compatibles avec une énumération donnée
 * @param enume une énumération des sommets
 */
void GrapheBicolore::activerDepuisEnumeration(std::vector<int> enume){
	this->desactiverToutesAretesVertes();
	for(int i=1; i<enume.size(); i++){
		this->activerAreteVerte(enume[i-1],enume[i]);
	}
}

/**
 * Algorithme de tri sur une liste de sommets.
 * @param sommets liste à trier
 */
void GrapheBicolore::triFusionSommets(std::vector<int> &sommets){
	if(sommets.size() > 1){
			std::vector<int> part1;
			std::vector<int> part2;
			for(int i=0; i<sommets.size()/2; i++){
				part1.push_back(sommets[i]);
			}
			for(int i=sommets.size()/2; i<sommets.size(); i++){
				part2.push_back(sommets[i]);
			}
			triFusionSommets(part1);
			triFusionSommets(part2);

			int i1=0;
			int i2=0;
			while(i1<part1.size() && i2<part2.size()){
				if( part1[i1] < part2[i2] ){
					sommets[i1+i2] = part1[i1];
					i1++;
				}
				else{
					sommets[i1+i2] = part2[i2];
					i2++;
				}
			}
			while(i1<part1.size()){
				sommets[i1+i2] = part1[i1];
				i1++;
			}
			while(i2<part2.size()){
				sommets[i1+i2] = part2[i2];
				i2++;
			}
		}
}

/**
 * Vérifie si un sommet appartient à une liste triée au préalable
 * @param s le sommet
 * @param sommets la liste
 * @return vrai si s appartient à sommets, faux sinon
 */
bool GrapheBicolore::appartientSommetListeTriee(int s, std::vector<int> sommets){
	int deb=0;
	int fin=sommets.size();
	while(deb<fin){
		int milieu = (deb+fin)/2;
		if(sommets[milieu] == s){
			return true;
		}
		else if(sommets[milieu] < s){
			deb = milieu+1;
		}
		else{
			fin = milieu;
		}
	}
	return false;
}

/**
 * Algorithme de tri sur une liste d'arêtes. Celles-ci sont triées par rapport au nombre d'arêtes voisines
 * @param aretes liste à trier
 */
void GrapheBicolore::triFusionAretesParDegre(std::vector<Arete> &aretes){
	if(aretes.size() > 1){
		std::vector<Arete> part1;
		std::vector<Arete> part2;
		for(int i=0; i<aretes.size()/2; i++){
			part1.push_back(aretes[i]);
		}
		for(int i=aretes.size()/2; i<aretes.size(); i++){
			part2.push_back(aretes[i]);
		}
		triFusionAretesParDegre(part1);
		triFusionAretesParDegre(part2);

		int i1=0;
		int i2=0;
		while(i1<part1.size() && i2<part2.size()){
			if( (degreVert(part1[i1].x) + degreVert(part1[i1].y)) < (degreVert(part2[i2].x) + degreVert(part2[i2].y)) ){
				aretes[i1+i2] = part1[i1];
				i1++;
			}
			else{
				aretes[i1+i2] = part2[i2];
				i2++;
			}
		}
		while(i1<part1.size()){
			aretes[i1+i2] = part1[i1];
			i1++;
		}
		while(i2<part2.size()){
			aretes[i1+i2] = part2[i2];
			i2++;
		}
	}
}

/**
 * Vérifie si une arête donnée appartient à un cycle de taille >2
 * @param e l'arête dont il faut vérifier l'appartenance à un cycle
 * @return vrai si e appartient à un cycle, faux sinon
 */
bool GrapheBicolore::appartientCycle(Arete e){
	std::vector<bool> dejaVu;
	for(int i=0; i<n; i++){
		dejaVu.push_back(false);
	}
	int u = e.x;
	int v = e.y;
	dejaVu[u]=true;
	std::queue<int> parcours;
	std::vector<int> voisins = voisinsVertsActifs(u);
	for(int i=0; i<voisins.size(); i++){
		int voisin = voisins[i];
		if(!dejaVu[voisin] && !(voisin==v)){
			dejaVu[voisin] = true;
			parcours.push(voisin);
		}
	}
	voisins = voisinsNoirsActifs(u);
	for(int i=0; i<voisins.size(); i++){
		int voisin = voisins[i];
		if(!dejaVu[voisin] && !(voisin==v)){
			dejaVu[voisin] = true;
			parcours.push(voisin);
		}
	}
	while(!dejaVu[v] && !parcours.empty()){
		int s = parcours.front();
		parcours.pop();
		voisins = voisinsVertsActifs(s);
		for(int i=0; i<voisins.size(); i++){
			int voisin = voisins[i];
			if(!dejaVu[voisin]){
				dejaVu[voisin] = true;
				parcours.push(voisin);
			}
		}
		voisins = voisinsNoirsActifs(s);
		for(int i=0; i<voisins.size(); i++){
			int voisin = voisins[i];
			if(!dejaVu[voisin]){
				dejaVu[voisin] = true;
				parcours.push(voisin);
			}
		}
	}
	return dejaVu[v];
}

/**
 * Construit le sous-graphe induit par un ensemble de sommet.
 * @param sommets l'ensemble de sommets
 * @return le sous-graphe induit
 */
GrapheBicolore* GrapheBicolore::sousGraphe(std::vector<int> sommets){
	this->triFusionSommets(sommets);
	GrapheBicolore* res = new GrapheBicolore(n);
	std::vector<Arete> aretes = this->aretesVertesActives();
	for(int i=0; i<aretes.size(); i++){
		Arete e = aretes[i];
		if(this->appartientSommetListeTriee(e.x,sommets) && this->appartientSommetListeTriee(e.y,sommets)){
			res->ajouterAreteVerte(e.x,e.y);
		}
	}
	std::vector<int> aretes_noires = getSommetsNoirsParAdjacence();
	for(int i=0; i+1<aretes_noires.size(); i++){
		int x = aretes_noires[i];
		int y = aretes_noires[i+1];
		if(this->appartientSommetListeTriee(x,sommets) && this->appartientSommetListeTriee(y,sommets)){
			res->ajouterAreteNoire(x,y);
		}
	}
	return res;
}

/**
 * Renvoie une liste de sous graphes correspondant aux différentes composantes connexes du graphe
 * @return la liste en question
 */
std::vector<GrapheBicolore*> GrapheBicolore::composantesConnexes(){
	std::vector<int> comp;
	for(int i=0; i<n; i++){
		comp.push_back(i);
	}
	//Tous les sommets noirs sont dans la meme composante connexe
	for(int i=1; i<getSommetsNoirsParAdjacence().size(); i++){
		comp[getSommetsNoirsParAdjacence()[i]] = getSommetsNoirsParAdjacence()[0];
	}
	for(int i=0; i<aretesVertesActives().size(); i++){
		int x = aretesVertesActives()[i].x;
		int y = aretesVertesActives()[i].y;
		int memo=comp[y];
		for(int i=0; i<n; i++){
			if(comp[i]==memo){
				comp[i]=comp[x];
			}
		}
	}
	std::vector<GrapheBicolore*> res;
	for(int i=0; i<n; i++){
		std::vector<int> sommets_comp;
		for(int j=0; j<n; j++){
			if(comp[j]==i){
				sommets_comp.push_back(j);
			}
		}
		if(!sommets_comp.empty()){
			GrapheBicolore* g = this->sousGraphe(sommets_comp);
			if(g->degreVertMax()>0 || g->degreNoirMax()>0){
				res.push_back(this->sousGraphe(sommets_comp));
			}
		}
	}
	return res;

}

/**
 * Renvoie une liste de sous graphes telle que, pour le problème Min-Cassure,
 *  l'union de solutions optimales pour chacun de ces graphes, soit une solution optimale pour le graphe
 *  Il faut s'assurer que toutes les arêtes vertes soient bien activées avant l'appel à cette méthode,
 *  et que le graphe est bien connexe. Si il n'est pas connexe, il faut appliquer cette méthode sur chacun des graphes
 *  de la méthode composantesConnexes()
 * @return le sous-graphe induit
 */
std::vector<GrapheBicolore*> GrapheBicolore::divisionGraphe(){
    std::vector<GrapheBicolore*> res;
    std::vector<int> sommets_n = getSommetsNoirsParAdjacence();
    std::vector<Arete> aretes_v = this->aretesVertesActives();

    std::vector<Arete> aretes;

    for(int i=0; i+1<sommets_n.size(); i++){
    	aretes.push_back({sommets_n[i], sommets_n[i+1]});
    }

    for(int i=0; i<aretes_v.size(); i++){
    	aretes.push_back(aretes_v[i]);
    }
    for(int i=0; i<aretes.size(); i++){
    	Arete e = aretes[i];
    	if( (degreVert(e.x)<3) && (degreVert(e.y)<3) && !appartientCycle(e)){
    		//INITIALISATION
    		int u = e.x;
    		int v = e.y;
    		std::vector<bool> dejaVu;
    		std::queue<int> parcours;
    		std::vector<int> accessiblesDepuisU;
    		std::vector<int> accessiblesDepuisV;
    		std::vector<int> voisins;

    		//PARCOURS DEPUIS U
    		for(int i=0; i<n; i++){
    			dejaVu.push_back(false);
    		}
    		parcours.push(u);
    		dejaVu[v]=true;
    		dejaVu[u]=true;
    		while(!parcours.empty()){
    			int s = parcours.front();
	    		accessiblesDepuisU.push_back(s);
    			parcours.pop();

    			voisins = voisinsVertsActifs(s);
    			for(int i=0; i<voisins.size(); i++){
    				int voisin = voisins[i];
    				if(!dejaVu[voisin]){
    					dejaVu[voisin] = true;
    					parcours.push(voisin);
    				}
    			}
    			voisins = voisinsNoirsActifs(s);
    			for(int i=0; i<voisins.size(); i++){
    				int voisin = voisins[i];
    				if(!dejaVu[voisin]){
    					dejaVu[voisin] = true;
    					parcours.push(voisin);
    				}
    			}
    		}
    		//PARCOURS DEPUIS V
    		for(int i=0; i<n; i++){
				dejaVu.push_back(false);
			}
			parcours.push(v);
			dejaVu[u]=true;
    		dejaVu[v]=true;
			while(!parcours.empty()){
				int s = parcours.front();
				accessiblesDepuisV.push_back(s);
				parcours.pop();
				voisins = voisinsVertsActifs(s);
				for(int i=0; i<voisins.size(); i++){
					int voisin = voisins[i];
					if(!dejaVu[voisin]){
						dejaVu[voisin] = true;
						parcours.push(voisin);
					}
				}
				voisins = voisinsNoirsActifs(s);
				for(int i=0; i<voisins.size(); i++){
					int voisin = voisins[i];
					if(!dejaVu[voisin]){
						dejaVu[voisin] = true;
						parcours.push(voisin);
					}
				}
			}

			//Creation des deux sous-graphes et appels récursifs.
			GrapheBicolore* g1 = this->sousGraphe(accessiblesDepuisU);
			GrapheBicolore* g2 = this->sousGraphe(accessiblesDepuisV);
			res = g1->divisionGraphe();
			std::vector<GrapheBicolore*> rec_g2 = g2->divisionGraphe();
			res.insert(res.end(), rec_g2.begin(), rec_g2.end());
			return res;
    	}
    }
    res.push_back(this);
    return res;
}

/**
 * Vérifie si un sommet est saturé par un couplage
 * @param s le sommet en question
 * @param couplage le couplage en question
 * @return vrai si s est saturé par couplage, faux sinon
 */
bool GrapheBicolore::estSature(int s, std::vector<Arete> couplage){
	for(int i=0; i<couplage.size(); i++){
		if(couplage[i].x==s || couplage[i].y==s){
			return true;
		}
	}
	return false;
}

/**
 * Calcule un couplage maximum sur les arêtes vertes
 * @return le couplage maximum
 */
std::vector<GrapheBicolore::Arete> GrapheBicolore::couplageMaximumAretesVertesActives(){
	std::vector<Arete> res;
	// ON COMMENCE PAR CALCULER UN COUPLAGE MAXIMAL
	std::vector<Arete> aretes_v = this->aretesVertesActives();
	for(int i=0; i<aretes_v.size(); i++){
		int x=aretes_v[i].x;
		int y=aretes_v[i].y;
		if(!this->estSature(x,res) && !this->estSature(y,res)){
			res.push_back(aretes_v[i]);
		}
	}

	//Pour l'instant on se contente d'améliorer la solution en 2/3 approx, il faudra améliorer en utilisant l'algo d'edmonds
	bool trouve=true;
	while(trouve){
		trouve = false;
		//on cherche un chemin augmentant de taille 3 de la forme xuvy
		std::vector<Arete> res_copie = res;
		for(int i=0; i<res_copie.size(); i++){
			int u=res_copie[i].x;
			int v=res_copie[i].y;
			int x=-1;
			int y=-1;
			for(int j=0; j<voisinsVertsActifs(u).size(); j++){
				int voisin = voisinsVertsActifs(u)[j];
				if(!this->estSature(voisin,res)){
					x = voisin;
				}
			}
			for(int j=0; j<voisinsVertsActifs(v).size(); j++){
				int voisin = voisinsVertsActifs(v)[j];
				if(!this->estSature(voisin,res)){
					y = voisin;
				}
			}
			if(x>=0 && y>=0){
				trouve=true;
				res.erase(res.begin()+i);
				res.push_back({u,x});
				res.push_back({v,y});
			}
		}
	}

	return res;
}

/**
 * Désactive les arêtes incohérentes, c'est-à-dire les arêtes vertes qui relient deux sommets noirs non-adjacents.
 */
void GrapheBicolore::desactiverAretesIncoherentes(){
	for(int i=0; i<this->aretesVertes().size(); i++){
		int x=aretesVertes()[i].x;
		int y=aretesVertes()[i].y;
		if(this->estSommetNoir(x) && this->estSommetNoir(y) && !this->testerAreteNoireActive(x,y)){
			this->desactiverAreteVerte(x,y);
		}
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
 * @param simplifier vrai pour réduire le nombre d'arêtes vertes considérées
 * @param debug vrai pour obtenir un affichage des étapes
 * @return le nombre d'arête désactivées pour obtenir un graphe linéarisable
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

    // On désactive les arêtes
    for (int i = 0; i < aretes.size(); ++i) {
        desactiverAreteVerte(aretes[i].x, aretes[i].y);
    }

    // On ajoute l'arête si le graphe reste linéarisable
    int nombreAretesRetirees = 0;
    for (int numArete = 0; numArete < aretes.size(); ++numArete) {
        activerAreteVerte(aretes[numArete].x, aretes[numArete].y);
        if (!estLinearisable()) {
            desactiverAreteVerte(aretes[numArete].x, aretes[numArete].y);
            nombreAretesRetirees++;
            if (debug) {
                std::cout << "L'arête numéro " << numArete << " de " << aretes[numArete].x
                          << " à " << aretes[numArete].y << " est retirée" << std::endl;
            }
        }
    }
    if (estLinearisable()) {
        return nombreAretesRetirees;
    }
    // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
    throw std::runtime_error("aucun résultat, sans arêtes vertes le graphe n'est pas linéaire");
}

/**
 * Algorithme glouton qui prend les arêtes par ordre croissant de leur nombre d'arêtes voisines
 * @param debug vrai pour obtenir un affichage des étapes
 * @return le nombre d'arête désactivées pour obtenir un graphe linéarisable
 * @remark modifie l'êtat des arêtes du graphe durant son execution pour obtenir un graphe linéarisable,
 *         on peut alors afficher le graphe pour obtenir le résultat
 */
int GrapheBicolore::algorithmeGloutonDegreCroissant(bool debug){
    // Initialisation de la liste des arêtes
    activerToutesAretesVertes(); // Réactivation avant algorithme
    std::vector<Arete> aretes = aretesVertes();
	this->triFusionAretesParDegre(aretes);

	if(debug){
		std::cout << "Voici la liste des arêtes utilisée : " << std::endl;
		for(int i=0; i<aretes.size(); i++){
			std::cout << aretes[i].x << " " << aretes[i].y << std::endl;
		}
	}
    // On désactive les arêtes
    for (int i = 1; i < aretes.size(); ++i) {
        desactiverAreteVerte(aretes[i].x, aretes[i].y);
    }

    // On ajoute l'arête si le graphe reste linéarisable
    int nombreAretesRetirees = 0;
    for (int numArete = 0; numArete < aretes.size(); ++numArete) {
        activerAreteVerte(aretes[numArete].x, aretes[numArete].y);
        if (!estLinearisable()) {
            desactiverAreteVerte(aretes[numArete].x, aretes[numArete].y);
            nombreAretesRetirees++;
            if (debug) {
                std::cout << "L'arête numéro " << numArete << " de " << aretes[numArete].x
                          << " à " << aretes[numArete].y << " est retirée" << std::endl;
            }
        }
    }
    if (estLinearisable()) {
        return nombreAretesRetirees;
    }
    // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
    throw std::runtime_error("aucun résultat, sans arêtes vertes le graphe n'est pas linéaire");
}




/**
 * Heuristique utilisant le dénombrement des chemins verts et des mauvais cycles
 * @param simplifier vrai pour réduire le nombre d'arêtes vertes considérées
 * @param tailleChemin taille maximum des chemins à dénombrer
 * @param debug vrai pour obtenir un affichage des étapes
 * @return le nombre d'arête désactivées pour obtenir un graphe linéarisable
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

    // On désactive les arêtes
    for (int i = 1; i < aretes.size(); ++i) {
        desactiverAreteVerte(aretes[i].x, aretes[i].y);
    }

    // On active les arêtes par ordre de poids
    int nombreAretesRetirees = 0;
    for (int numArete = 0; numArete < aretes.size(); ++numArete) {
        int imin = -1;
        double poidsMin = -1;
        for (int j = 0; j < aretes.size(); ++j) {
            if (poidsMin == -1 || (poids[j] > 0 && poids[j] < poidsMin)) {
                imin = j;
                poidsMin = poids[j];
            }
        }
        activerAreteVerte(aretes[imin].x, aretes[imin].y);
        // Si le graphe n'est plus linéarisable, on retire l'arête
        if (!estLinearisable()) {
            desactiverAreteVerte(aretes[imin].x, aretes[imin].y);
            nombreAretesRetirees++;
            if (debug) {
                std::cout << "L'arête numéro " << imin << " de " << aretes[imin].x << " à "
                          << aretes[imin].y << " est retirée" << std::endl;
            }
        }
        poids[imin] = -1;
    }
    if (estLinearisable()) {
        return nombreAretesRetirees;
    }
    // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
    throw std::runtime_error("aucun résultat, sans arêtes vertes le graphe n'est pas linéaire");
}

/**
 * Algorithme exact qui teste toutes les combinaisons d'activation et de désactivation d'arêtes vertes
 * @param simplifier vrai pour réduire le nombre d'arêtes vertes considérées, la solution reste optimale
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
    throw std::runtime_error("aucun résultat, sans arête vertes le graphe n'est pas linéaire");
}

/**
 * Utilisation de la méthode visionGraphe() afin d'optimiser le temps de calcul de l'algorithme exact.
 * @param simplifier vrai pour réduire le nombre d'arêtes vertes considérées, la solution reste optimale
 * @param debug vrai pour obtenir un affichage des étapes
 * @return le nombre d'arête désactivées pour obtenir un graphe linéarisable
 * @remark modifie l'êtat des arêtes du graphe durant son execution pour obtenir un graphe linéarisable,
 *         on peut alors afficher le graphe pour obtenir le résultat
 */
int GrapheBicolore::algorithmeExactAvecDecoupage(bool simplifier, bool debug){
	activerToutesAretesVertes(); // Réactivation avant algorithme
	this->desactiverAretesIncoherentes();
	std::vector<GrapheBicolore*> decoupage;
	std::vector<GrapheBicolore*> comp_connexes = this->composantesConnexes();
	for(int i=0; i<comp_connexes.size(); i++){
		std::vector<GrapheBicolore*> decoupage_inter = comp_connexes[i]->divisionGraphe();
		decoupage.insert(decoupage.end(), decoupage_inter.begin(), decoupage_inter.end());
	}
	std::vector<Arete> aretes_a_suppr;
	int res=this->aretesVertesDesactives().size();
	for(int i=0; i<decoupage.size(); i++){
		int nb_aretes = decoupage[i]->algorithmeExact(simplifier, debug);
		res+= nb_aretes;
		aretes_a_suppr = decoupage[i]->aretesVertesDesactives();
		for(int j=0; j<aretes_a_suppr.size(); j++){
			this->desactiverAreteVerte(aretes_a_suppr[j].x, aretes_a_suppr[j].y);
		}
	}
	if (estLinearisable()) {
		return res;
	}
	 // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
	throw std::runtime_error("erreur détectée dans l'algo exact avec decoupage : le graphe n'est pas linéarisable");
}

/**
 * Algorithme glouton pour trouver une solution au problème Min-Cassure, avec garantie de 2-approximation
 * @return le nombre d'arête désactivées pour obtenir un graphe linéarisable
 */
int GrapheBicolore::algorithme2approx(bool debug){
	this->desactiverAretesIncoherentes();
	std::vector<Arete> coup_max = this->couplageMaximumAretesVertesActives();

    // On désactive les arêtes
	std::vector<Arete> aretes = this->aretesVertes();
    for (int i = 0; i < aretes.size(); ++i) {
        desactiverAreteVerte(aretes[i].x, aretes[i].y);
    }

    //On réactive celles du couplage
    for(int i=0; i<coup_max.size(); i++){
    	activerAreteVerte(coup_max[i].x, coup_max[i].y);
	}

    // Puis on rend la solution minimal pour l'inclusion
    int nombreAretesRetirees = 0;
    for (int numArete = 0; numArete < aretes.size(); ++numArete) {
        activerAreteVerte(aretes[numArete].x, aretes[numArete].y);
        if (!estLinearisable()) {
            desactiverAreteVerte(aretes[numArete].x, aretes[numArete].y);
            nombreAretesRetirees++;
            if (debug) {
                std::cout << "L'arête numéro " << numArete << " de " << aretes[numArete].x
                          << " à " << aretes[numArete].y << " est retirée" << std::endl;
            }
        }
    }
    if (estLinearisable()) {
        return nombreAretesRetirees;
    }
    // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
    throw std::runtime_error("aucun résultat, sans arêtes vertes le graphe n'est pas linéaire");
}

/**
 * Algorithme probabiliste pour trouver une solution au problème Min-Cassure
 * @param debug vrai pour obtenir un affichage des étapes
 * @return le nombre d'arête désactivés pour obtenir un graphe linéarisable
 */
int GrapheBicolore::algorithmeProbabiliste(bool debug, bool start, bool complete){
	srand (time(NULL));
	int res;
	std::vector<int> enumeration, best_enumeration;
	std::vector<Arete> aretes = this->aretesVertes();
	if(start){
		res = this->algorithmeGlouton(false, false);
		enumeration = this->enumeration();
	}
	else{
		std::vector<int> sommets_noirs = this->getSommetsNoirsParAdjacence();
		for(int i=0; i<sommets_noirs.size(); i++){
			enumeration.push_back(sommets_noirs[i]);
		}
		std::vector<int> enumeration_bis;
		for(int i=0; i<n; i++){
			if(!this->estSommetNoir(i)){
				enumeration_bis.push_back(i);
			}
		}
		for(int i=enumeration_bis.size(); i>0; i--){
			int r = rand() % i;
			int pos;
			if(enumeration.size()>0){
				pos = rand() % enumeration.size();
			}
			else{
				pos = 0;
			}
			enumeration.insert(enumeration.begin()+pos,enumeration_bis[r]);
			enumeration_bis.erase(enumeration_bis.begin()+r);
		}
	}
	this->activerDepuisEnumeration(enumeration);
	res = this->aretesVertesDesactives().size();
	best_enumeration = enumeration;
	if(debug){
		std::cout<< "On part de l'énumération : ";
		for (int e : enumeration) {
			std::cout << e << " ";
		}
		std::cout << std::endl;
	}
	int N=n*n;
	if(complete){
		N=this->aretesVertes().size();
	}
	for (int i=0; i<N && this->aretesVertesDesactives().size()>0; i++){
		int r = rand() % (this->aretesVertesDesactives().size());
		Arete arete_a_ajouter = this->aretesVertesDesactives()[r];
		if(this->estSommetNoir(arete_a_ajouter.x) && this->estSommetNoir(arete_a_ajouter.y)){
			if(this->testerAreteNoireActive(arete_a_ajouter.x,arete_a_ajouter.y)){
				int i_x = 0;
				while(enumeration[i_x] != arete_a_ajouter.x){
					i_x++;
				}
				int i_y = 0;
				while(enumeration[i_y] != arete_a_ajouter.y){
						i_y++;
				}
				int u = std::min(i_x,i_y);
				int v = std::max(i_x,i_y);
				r = rand()%2;
				if(r){
					enumeration.insert(enumeration.begin()+v, enumeration[u]);
					enumeration.erase(enumeration.begin()+u);
				}
				else{
					enumeration.insert(enumeration.begin()+u+1, enumeration[v]);
					enumeration.erase(enumeration.begin()+v+1);
				}
			}
		}
		else if(this->estSommetNoir(arete_a_ajouter.x) || this->estSommetNoir(arete_a_ajouter.y)){
			int sommet_n, sommet_v;
			if(this->estSommetNoir(arete_a_ajouter.x)){
				sommet_n = arete_a_ajouter.x;
				sommet_v = arete_a_ajouter.y;
			}
			else{
				sommet_v = arete_a_ajouter.x;
				sommet_n = arete_a_ajouter.y;
			}

			int v=0;
			while(enumeration[v] != sommet_v){
				v++;
			}
			enumeration.erase(enumeration.begin()+v);
			int u=0;
			while(enumeration[u] != sommet_n){
				u++;
			}
			r = rand()%2;
			if(r){
				enumeration.insert(enumeration.begin()+u, sommet_v);
			}
			else{
				enumeration.insert(enumeration.begin()+u+1, sommet_v);
			}
		}
		else{
			int sommet_n, sommet_v;
			r = rand()%2;
			if(r){
				sommet_n = arete_a_ajouter.x;
				sommet_v = arete_a_ajouter.y;
			}
			else{
				sommet_v = arete_a_ajouter.x;
				sommet_n = arete_a_ajouter.y;
			}

			int v=0;
			while(enumeration[v] != sommet_v){
				v++;
			}
			enumeration.erase(enumeration.begin()+v);
			int u=0;
			while(enumeration[u] != sommet_n){
				u++;
			}
			r = rand()%2;
			if(r){
				enumeration.insert(enumeration.begin()+u, sommet_v);
			}
			else{
				enumeration.insert(enumeration.begin()+u+1, sommet_v);
			}
		}
		this->activerDepuisEnumeration(enumeration);
		if(complete){
			aretes = this->aretesVertesDesactives();
			for (int numArete = 0; numArete < aretes.size(); ++numArete) {
				activerAreteVerte(aretes[numArete].x, aretes[numArete].y);
				if (!estLinearisable()) {
					desactiverAreteVerte(aretes[numArete].x, aretes[numArete].y);
				}
			}
		}
		enumeration = this->enumeration();
		int nb_cassure = this->aretesVertesDesactives().size();
		if(nb_cassure<res){
			res = nb_cassure;
			best_enumeration = enumeration;
			if(debug){
				std::cout<< "Enumération actuelle : ";
				for (int e : enumeration) {
					std::cout << e << " ";
				}
				std::cout << std::endl;
				std::cout << "La nouvelle énumération est la meilleure rencontrée." << std::endl;
			}
		}
	}
	this->activerDepuisEnumeration(best_enumeration);
	if (estLinearisable()) {
		return res;
	}
	std::cerr << "erreur avec l'énumération : ";
	for (int e : best_enumeration) {
		std::cerr << e << " ";
	}
	std::cerr << std::endl;
	 // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
	throw std::runtime_error("erreur détectée, le graphe n'est pas linéarisable");
}


/**
 * Utilisation de la méthode visionGraphe() afin d'optimiser la qualité de la solution de l'algorithme probabiliste.
 * @param simplifier vrai pour réduire le nombre d'arêtes vertes considérées, la solution reste optimale
 * @param debug vrai pour obtenir un affichage des étapes
 * @return le nombre d'arête désactivées pour obtenir un graphe linéarisable
 * @remark modifie l'êtat des arêtes du graphe durant son execution pour obtenir un graphe linéarisable,
 *         on peut alors afficher le graphe pour obtenir le résultat
 */
int GrapheBicolore::algorithmeProbabilisteAvecDecoupage(bool debug, bool start, bool complete){
	activerToutesAretesVertes(); // Réactivation avant algorithme
	this->desactiverAretesIncoherentes();
	std::vector<GrapheBicolore*> decoupage;
	std::vector<GrapheBicolore*> comp_connexes = this->composantesConnexes();
	for(int i=0; i<comp_connexes.size(); i++){
		std::vector<GrapheBicolore*> decoupage_inter = comp_connexes[i]->divisionGraphe();
		decoupage.insert(decoupage.end(), decoupage_inter.begin(), decoupage_inter.end());
	}
	std::vector<Arete> aretes_a_suppr;
	int res=this->aretesVertesDesactives().size();
	for(int i=0; i<decoupage.size(); i++){
		int nb_aretes = decoupage[i]->algorithmeProbabiliste(debug,start, complete);
		res+= nb_aretes;
		aretes_a_suppr = decoupage[i]->aretesVertesDesactives();
		for(int j=0; j<aretes_a_suppr.size(); j++){
			this->desactiverAreteVerte(aretes_a_suppr[j].x, aretes_a_suppr[j].y);
		}
	}
	if (estLinearisable()) {
		return res;
	}
	 // Ne pas obtenir de solution n'est pas un résultat possible, on lève une erreur d'execution
	throw std::runtime_error("erreur détectée dans l'algo probabiliste avec decoupage : le graphe n'est pas linéarisable");
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
    for (int i = 0; i < listeEnum.size(); ++i) {
		dejaVu[listeEnum[i]] = true;
	}
    while(listeEnum.size()<n){
    	int s = 0;
    	while(dejaVu[s] || voisinsVertsActifs(s).size()>1){
    		s++;
    	}
		listeEnum.push_back(s);
		dejaVu[s]=true;
    	if(voisinsVertsActifs(s).size()>0){
			int precedent = s;
			int courant = voisinsVertsActifs(s)[0];
    		while (courant != -1){
    			listeEnum.push_back(courant);
    			dejaVu[courant]=true;
    			int suivant = prochainSommetVert(precedent, courant);
				precedent = courant;
				courant = suivant;
    		}
    	}
    }

    return listeEnum;
}

void GrapheBicolore::test(){
	std::vector<GrapheBicolore*> tab_g = this->divisionGraphe();
	std::cerr << "methode terminée" << std::endl;
	for(int i=0; i<tab_g.size(); i++){
		std::string name = "graphe" + std::to_string(i) + ".txt";
		tab_g[i]->fichierSage(name);
	}
}

