#include <stdio.h>
#include <stdlib.h>
#include "ville.h"

typedef struct login
{
    int login_id;
    int key;//car le login va etre decrypte une premiere fois donc on utilise jamais le vrai mots de pass

}*Login;

typedef struct login_admin
{
    int code_agence;
    int login_id;
    int pin;

}*Login_Admin;


int client_login();//permet l'acces aux compte et a tous les droits du client registre et authentifiee

int admin_login();//permet l'acces a tous les comptes et dossier de l'agence dont le admin y apprtient

