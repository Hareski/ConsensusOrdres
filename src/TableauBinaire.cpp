#include <stdexcept>
#include <iostream>
#include "../header/TableauBinaire.h"

/**
 * Constructeur de la structure de tableau binaire
 * @param taille taille du tableau binaire
 * @param cardinalite nombre d'entrées "vrai" dans le tableau
 */
TableauBinaire::TableauBinaire(unsigned taille, unsigned cardinalite) : taille(taille), cardinalite(cardinalite) {
    value = cardinalite >= taille; // Forcé d'avoir 1 si pas assez de place.
    valeurCourante = nullptr;
    if (taille > 1) {
        next = new TableauBinaire(taille - 1, cardinalite);
    } else {
        next = nullptr;
    }
}

/**
 * Détruit la queue du tableau
 */
TableauBinaire::~TableauBinaire() {
    delete next;
};

/**
 * @param index index dans le tableau
 * @remark La méthode valeurSuivante() pour itérer l'ensemble des valeurs permet
 * d'obtenir une meilleure complexité
 * @return la valeur à l'index indiqué
 */
bool TableauBinaire::valeur(unsigned index) {
    if (index == 0) {
        return value;
    } else if (next != nullptr) {
        return next->valeur(index - 1);
    } else {
        throw std::runtime_error("index supérieur à la taille");
    }
}

/**
 * Simule un itérateur qui permet d'obtenir successivement l'ensemble des valeurs du tableau
 * et boucle une fois la dernière valeur obtenue
 * @return la prochaine valeur à itérer dans le tableau
 */
bool TableauBinaire::valeurSuivante() {
    if (valeurCourante == nullptr || valeurCourante->next == nullptr) {
        valeurCourante = this;
    } else {
        valeurCourante = valeurCourante->next;
    }
    return valeurCourante->value;
}

/**
 * Génère le prochain tableau binaire s'il existe
 * @return vrai s'il existe un tableau non-généré, faux sinon
 */
bool TableauBinaire::genererProchain() {
    valeurCourante = nullptr;
    if (next != nullptr) {
        if (next->genererProchain()) {
            return true; // Les suivants ont modifié une valeur
        } else {
            if (!value && cardinalite > 0) { // La valeur est fausse
                delete next;
                next = new TableauBinaire(taille - 1, cardinalite - 1);
                return value = true;
            } else {
                return false; // Plus de generation possible
            }
        }
    }
    return false;
}

/**
 * Affiche l'ensemble du tableau dans le flux
 * @param flux
 */
void TableauBinaire::afficher(std::ostream &flux) const {
    if (value) flux << "1";
    else flux << "0";
    if (next) {
        next->afficher(flux);
    } else {
        flux << ".";
    }
}

/**
 * Affiche l'ensemble du tableau dans le flux
 * @param flux
 * @param T le tableau
 * @return retourne le flux passé en paramètre
 */
std::ostream &operator<<(std::ostream &flux, const TableauBinaire &T) {
    T.afficher(flux);
    return flux;
}
