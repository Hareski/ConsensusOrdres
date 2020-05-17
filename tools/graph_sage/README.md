# Visualisation des multigraphes noir et verts

Outil de visualisation des graphes bicolores avec Sagemath.

## Prérequis
Il s'agit d'un fichier de configuration de Sagemath. Il vous faut donc le télécharger ainsi que Python sur [sagemath.org](https://www.sagemath.org/) .

Assurez-vous d'avoir un fichier qui suit le format de description. Le programme de *concensus d'ordre de multigraphes noir et verts* fournit une méthode `fichierSage()` permettant de les générer. 

## Utilisation 

Utiliser la commande :
`sage -python visualisation_multigraphe.sage.py fichier_de_relations fichier_de_sortie.png`

On peut modifier la commande en remplaçant `sage` par le chemin vers sage s'il ne fait pas partie de votre `PATH`.

Le fichier `visualisation_multigraphe.sage` vous permet de modifier le programme Sage plus facilement mais n'est pas utile pour l'utilisation.

## Format
Exemple d'un fichier du format attendu :

```
0
8:v;
1
2:n;5:v;
2
1:n;3:n;
3
2:n;4:n;7:v;8:v;
4
3:n;
5
1:v;6:v;
6
5:v;7:v;
7
3:v;6:v;
8
0:v;3:v;
```