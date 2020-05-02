#ifndef TABLEAUBINAIRE_H
#define TABLEAUBINAIRE_H

class TableauBinaire {
private:
    bool value;
    unsigned taille;
    unsigned cardinalite;
    TableauBinaire *next;
    TableauBinaire *valeurCourante;

public:
    virtual ~TableauBinaire();

    TableauBinaire(unsigned taille, unsigned cardinalite);

    bool valeur(unsigned index);

    bool genererProchain();

    void afficher(std::ostream &flux) const;

    bool valeurSuivante();
};

std::ostream &operator<<(std::ostream &flux, const TableauBinaire &T);

#endif
