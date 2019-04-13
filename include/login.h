#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <crypt.h>
#include "ville.h"
#define LOGIN_ID_SIZE 10
#define LOGIN_KEY_SIZE 10
#define CODE_AGENCE_SIZE 37

typedef struct login_size
{
	size_t id, key;

} login_size_t ;

typedef struct login
{
    const char* login_id;
    const char* key;//car le login va etre decrypte une premiere fois donc on utilise jamais le vrai mots de pass

} *Login;

typedef struct login_admin
{
    const char* code_agence;
    const char* login_id;
    const char* pin;

} *Login_Admin;

Login init_login_arg(login_size_t alloc_size, char* id, char* key);

int client_login();//permet l'acces aux compte et a tous les droits du client registre et authentifiee

int admin_login();//permet l'acces a tous les comptes et dossier de l'agence dont le admin y apprtient

