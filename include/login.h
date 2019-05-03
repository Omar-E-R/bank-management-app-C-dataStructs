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

#endif
