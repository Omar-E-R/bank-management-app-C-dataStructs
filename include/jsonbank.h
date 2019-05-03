#ifndef JSONBANK_H
#define JSONBANK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <jansson.h>
#include "bank.h"
#include "bankmanagement.h"
#include "login.h"

/* Has all bank account data */
typedef struct account_t account_t;

/* Has all the client data and bank accounts */
typedef struct individual_t individual_t;

/* Has all the employe data i.e job position and priviliges*/
typedef struct employee_t employee_t;

/* Has all the clients, bank accounts data and the employees as well, only for which it belongs (agency)*/
typedef struct agency_t agency_t;

/* Has all the data of the agencies as well as their clients, bank accounts and employees that belongs to it (state)*/
typedef struct state_t state_t;

/* Has an ID and a password for logins */
typedef struct login_t login_t;

/* Has the login credentials for an IT administrator, who has permission to view all of the bank data */
typedef struct admin_t admin_t;

/* Data type that represents a bank and hold its data */
typedef struct bank_t bank_t;

#define ADMIN_JSON_FILE "admin.json"

#define failhdr fprintf(stderr, "%s:%d: ", __FILE__, __LINE__)

#define fail(msg)                     \
	do                                \
	{                                 \
		failhdr;                      \
		fprintf(stderr, "%s\n", msg); \
	} while (0)

int bank_json_parse_individual(individual_t *individual, int option, size_t flags);
int bank_json_parse_account(account_t *account, int option, size_t flags);
int bank_json_parse_agency(agency_t *agency, int function, int option, size_t flags);

int bank_json_parse_state(state_t *state, int function, int option, size_t flags);

int bank_json_parse_bank(bank_t *bank, int option, int function);
login_t* bank_json_parse_login(json_t* login_array);
login_t* bank_json_parse_admin();


int bank_json_dump_individual(individual_t *individual, int option, size_t flag);
int bank_json_dump_account(account_t *account, size_t flag);
int bank_json_dump_agency(agency_t *agency, int option, size_t flags);
int bank_json_dump_state(state_t *state, int option, size_t flags);
int bank_json_dump_bank(bank_t *bank, int option, size_t flags);
int bank_json_dump_admin(login_t *admin, size_t flag);

int bank_write_activity(account_t *account, char* activity);

void *guaranteed_memset(void *v, int c, size_t n);

void *secure_malloc(size_t size);

void secure_free(void *ptr);

#endif