
# Algorithmique pour le consensus d’ordres
Code source de nos Travaux d'Étude et de Recherche. Plusieurs méthodes exactes ou approchées pour obtenir un consensus d'ordre. C'est-à-dire, transformer une relation binaire quelconque en une relation d'ordre partiel avec une perte d'information minimisée. Ces algorithmes servent le contexte bio-informatique de la cartographie génétique.

## Organisation du dossier

## `/app`
Contient le fichier `main.cpp`. Il s'agit d'exemples d'utilisation du programme. 
> **Note :** Il peut-être suprimé ou modifié si le programme est utilisé dans un autre cadre.

## `/doc`
Documentation à partir du code source et des commentaires du programme qui doit être généré avec Doxygen : http://www.doxygen.nl/
> **Note :** Contient aussi le fichier de configuration `Doxyfile` indiquant la configuration à suivre à Doxygen.

## `/data`
Différentes données utilisées pour les exemples d'utilisation.

## `/header`
Fichiers d’en-tête du programmes. Définitions de l'ensemble des classes, variables, fonctions et méthodes.

## `/src`
Fichiers sources du programmes. L'ensemble des méthodes et fonctions y sont déclaré. Pour facilité la maintenance, la documentation se situe directement au-dessus des déclarations.

## `/tools`
Plusieurs outils externes pour faciliter l'utilisation du programme. Notamment la visualisation des graphes.

### `graph_sage`
Outil de visualisation des graphes bicolores avec Sagemath.

### `convert_string2int`
Outil Python pour convertir des fichiers décrivant des relations binaires entre gênes utilisant des noms en des relations binaires utilisant des entiers.
