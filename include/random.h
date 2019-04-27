#ifndef RANDOM_H

#define RANDOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>

/* For uuid_generate() and uuid_unparse() */
#include <uuid/uuid.h>

#define lowercaseuuid true
#define BANK_NUMBER "40053"

extern int global_account_number;


/* Uncomment to always generate capital UUIDs. */
//#define capitaluuid true


int uuid_gen(char* uuid);
char* login_id_generator(int *code);
char *login_id_decoder(char *login_id);
char *agency_id_generator();
char* iban_gen(const char *indicatif_agence, const char *code);

#endif