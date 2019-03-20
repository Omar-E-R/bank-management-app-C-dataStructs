#include <stdio.h>
#include <stdlib.h>
#include "client.h"

typedef struct login
{
    int login_id;
    int password;

    // union reset_login{

    //     int off;
        



    // };

}*Login;

typedef struct login_admin
{
    int code_agence;
    int login_id;
    int password;

}*Login_Admin;


Client client_login();

Admin admin_login();


