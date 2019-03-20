#include <stdio.h>
#include <stdlib.h>
#include "agence.h"

enum ville
{
    Toulouse=31
    //on peut en rajouter d'autres
};

typedef struct ville
{
    int code_ville;

    char* nom_ville;

    Agence liste_agences;

    Ville next_ville;
    
}*Ville;


