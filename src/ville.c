#include "ville.h"

#define ENCRYPTION_KEY 324695741

struct ville
{
    int code_ville;

    Nom_Ville nom;

    Agence liste_agences;

    Ville next_ville;
};

/*


permet de decrypter les donnees des agences



*/