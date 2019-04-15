#ifndef LOGIN_H
#define LOGIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <crypt.h>

#define LOGIN_ID_SIZE 10
#define LOGIN_KEY_SIZE 10
#define CODE_AGENCE_SIZE 37

typedef struct login_size
{
	size_t id, key;

} login_size_t ;


typedef struct login *Login;

typedef struct login_admin *Login_Admin;

Login init_login();
Login init_login_arg(login_size_t alloc_size, char* id, char* key);
Login init_login_admin();
int encrypt_login_pass(Login user);
int encrypt_code(const char* hash_code);


#endif