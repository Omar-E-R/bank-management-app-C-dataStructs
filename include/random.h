#ifndef RANDOM_H

#define RANDOM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>


extern int global_account_number;


/* For uuid_generate() and uuid_unparse() */
#include <uuid/uuid.h>



#define BANK_NUMBER "40305"

/* Uncomment to always generate capital UUIDs. */
//#define capitaluuid true

/* Uncomment to always generate lower-case UUIDs. */
//#define lowercaseuuid true

/*
 * Don't uncomment either if you don't care (the case of the letters
 * in the 'unparsed' UUID will depend on your system's locale).
 */

int uuid_gen(const char* uuid);
int* login_id_generator(int *code);
int login_id_decoder(int* login_id);
int indicatif_gen(const char **indicatif_agence, size_t nb_ids); //5chiffres
int iban_gen(const char *iban, const char *indicatif_agence, const char* code_postale);

#endif