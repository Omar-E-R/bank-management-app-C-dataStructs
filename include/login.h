#ifndef LOGIN_H
#define LOGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <crypt.h>
#include <sys/syscall.h>
#include "bank.h"
#include "jsonbank.h"
#include "random.h"

#define LOGIN_ID_SIZE 11

typedef struct login_t login_t;

int validate_login(login_t* private_log, login_t* user_log);
int encrypt_login_pass(login_t* login);

// login_t* init_login();

// login_t* init_login_arg(login_size_t alloc_size, char *id, char *key);

// login_t*_Admin init_login_admin();


// login_t* create_login(char* pass, const char *code_agence, const char *code_departement);

// login_t*_Admin create_admin_login(char *pass, const char *code_agence, const char *code_departement);

// void printout_log_ad(login_t*_Admin login);
// void printout_log(login_t* login);

// int validate_login_admin(login_t*_Admin private, login_t*_Admin user);
#endif
