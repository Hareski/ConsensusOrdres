# Conversion en relations binaires d'entiers

Outil Python pour convertir des fichiers décrivant des relations binaires entre gênes utilisant des noms en des relations binaires utilisant des entiers.

## Utilisation
Le programme attend deux paramètres : 

* Le chemin vers un fichier des relations binaires noires
* Le chemin vers un fichier des relations binaires vertes

`python convert_string2int fichier_relations_noires fichier_relations_vertes`

## Format en entrée
Les relations sont entre des noms de gènes

```
KB668223 KB668300
KB668223 KB668929 
KB668234 KB668662 
KB668234 KB668742 
...
```

## Format en sortie
Les relations sont entre des entiers

```
1 2
2 3 
4 2 
3 1 
...
```
