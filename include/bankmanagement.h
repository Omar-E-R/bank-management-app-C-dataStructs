#ifndef LIBMANAGEMENT_H

#define LIBMANAGEMENT_H


#ifndef BANK_H

#include<stdio.h>
#include<jansson.h>
#include <time.h>
#include <sys/time.h>
#define BANK_NUMBER "40053"
#define CODE_BIC "ELRIFAIO"

#define DOLLAR_TO_EURO 0.891702
#define AGENCY_ID_SIZE 6
#define DATE_SIZE 9
#define DATE_JOIN_SIZE 26 //WWW MMM DD HH:MM:SS YYYY
#define PHONE_SIZE 12
#define LOGIN_KEY_SIZE 110
#define UUID_SIZE 37
#define IBAN_SIZE 38
#define ACCOUNT_NO_SIZE 12

/* checking */

#define BANK_ACCOUNT_STATUS 0X010
#define BANK_ACCOUNT_TYPE 0x011
#define BANK_ACCOUNT_UUID 0x012
#define BANK_ACCOUNT_OPERATIONS_UUID 0x13
#define BANK_ACCOUNT_IBAN 0x014
#define BANK_ACCOUNT_NO 0x015
#define BANK_ACCOUNT_BALANCE 0x016
#define BANK_INDIVIDUAL_STATUS
#define BANK_INDIVIDUAL_UUID 0x017
#define BANK_INDIVIDUAL_SEX 0x018
#define BANK_INDIVIDUAL_LASTNAME 0x019
#define BANK_INDIVIDUAL_FIRSTNAME 0x020
#define BANK_INDIVIDUAL_BIRTHDATE 0x021
#define BANK_INDIVIDUAL_EMAIL 0x022
#define BANK_INDIVIDUAL_ADDRESSE_1 0x023
#define BANK_INDIVIDUAL_CITY 0x024
#define BANK_INDIVIDUAL_ZIPCODE 0x025
#define BANK_INDIVIDUAL_ADDRESSE_2 0x026
#define BANK_INDIVIDUAL_MOBILE 0x027
#define BANK_INDIVIDUAL_HOME 0x028
#define BANK_INDIVIDUAL_ID_CARD 0x029
#define BANK_INDIVIDUAL_JOIN_DATE 0x030
#define BANK_EMPLOYEE_STATUS 0X031
#define BANK_EMPLOYEE_POSITION 0X032
#define BANK_AGENCY_UUID 0X067
#define BANK_AGENCY_ID 0X068
#define BANK_AGENCY_ADDRESS 0X069
#define BANK_AGENCY_CODE 0X070
#define BANK_STATE_UUID 0X077
#define BANK_STATE_ZIPCODE 0X078
#define BANK_STATE_CODE 0X079

typedef enum
{
	BANK_EMPLOYEE_UKNOWN_POSITION,
	BANK_EMPLOYEE_MANAGER,
	BANK_EMPLOYEE_ADVISOR,
	BANK_EMPLOYEE_ASSISTANT

} employee_position;

typedef enum status_type
{
	BANK_OBJECT_EMPTY,
	BANK_OBJECT_INIT,
	BANK_OBJECT_BLOCKED,
	BANK_OBJECT_DELETED,
	BANK_OBJECT_NULL,
	BANK_LOGIN_ENCRYPTED,
	BANK_LOGIN_EMP,
	BANK_OBJECT_CHANGED,
	BANK_OBJECT_CONSTANT

} status_type;

typedef enum
{
	BANK_ACCOUNT_EMPTY = 244,
	BANK_ACCOUNT_LIVRETA = 245,
	BANK_ACCOUNT_LIVRETJEUNE = 246,
	BANK_ACCOUNT_LDD = 247,
	BANK_ACCOUNT_PEL = 248,
	BANK_ACCOUNT_COURANT = 249

} bank_account_type;

typedef enum
{
	BANK_ACCOUNT_NONE,
	BANK_ACCOUNT_SHARED,
	BANK_ACCOUNT_INDIVIDUAL

} account_type;

char *french_states[99];
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

/* returns 0 if bank_account status is size_t flag,
	1 if it is not, and
		-1 if bank_account is NULL */
int bank_account_is_status(account_t *account, status_type flag);
/*returns 0 if bank_account is shared between two,
	and 1 if not, and -1 if bank_account is NULL or EMPTY*/
int bank_account_is_shared(account_t *account);
/* returns 0 if bank_account is
	of type "size_t flag" and 1 if not
	-1 if NULL */
int bank_account_is_type(account_t *account, bank_account_type flag);

int bank_account_set_status(account_t *account, status_type flag);

/* generates a new uuid and add it to the bank_account
	then returns it, to replace uuid that exists use
		bank_account_set_uuid_new */
char *bank_account_set_uuid(account_t *account);
/* generates a new uuid and replaces bank_account's uuid if it has one
	returns 0 for success, and it
	returns 1 for failure i.e bank_account doesnt have a uuid */
int bank_account_set_uuid_new(account_t *account);
/*sets the type of bank_account to
	size_t flag if not already set
	returns 0 success
		returns 1 failure i.e bank_account has type*/
int bank_account_set_type(account_t *account, bank_account_type type);
/* Makes the individual_t
	passed as argument holder of bank_account
		if not already has one
		returns 0 on success
		return 1 on failure
			i.e account has a holder already */
int bank_account_set_holder(account_t *account, individual_t *holder);

/*Creates a new empty individual_t as a holder of bank_account and returns it
	if it fails it returns NULL */
individual_t *bank_account_set_new_holder(account_t *account);
individual_t *bank_account_shared_set_new_holder(account_t *account);

/* Adds a second holder for bank_account
	only if it is a shared account and has one holder already
		returns 0 on success
			1 on failure */
int bank_account_shared_set_holder(account_t *account, individual_t *holder);
/*returns an empty bank account */
account_t *bank_account(bank_account_type type, account_type flag);
int bank_account_set_agency(account_t *account, agency_t *agency);
agency_t *bank_account_get_agency(account_t *account);
individual_t **bank_account_shared_get_holder(account_t *account);
individual_t *bank_account_get_holder(account_t *account);
char *bank_account_get(account_t *account, size_t flag);
/*bank_account has to be already in an agency
	for it to be able to get an IBAN and Account_no
	this function generates the appropriate IBAN and account_no for
	this bank_account */
int bank_account_set_iban(account_t *account);

int bank_account_money_depot(account_t *account, double money_amount, char currency, int flag);
individual_t *bank_individual(size_t flag);

/*returns the status of an individual
	1 if false
	0 if true
	*/
int bank_individual_is_status(individual_t *individual, size_t status);
int bank_individual_set_status(individual_t *individual, status_type flag);

char *bank_individual_set_uuid(individual_t *individual);
int bank_individual_set_sex(individual_t *individual, char sex);
int bank_individual_set_firstname(individual_t *individual, char *firstname);

int bank_individual_set_lastname(individual_t *individual, char *lastname);

int bank_individual_set_birthdate(individual_t *individual, char *birthdate);

int bank_individual_set_email(individual_t *individual, char *email);

int bank_individual_set_address_no1(individual_t *individual, char *address_no1);

int bank_individual_set_address_no2(individual_t *individual, char *address_no2);

int bank_individual_set_city(individual_t *individual, char *city);

int bank_individual_set_zipcode(individual_t *individual, int zipcode);

int bank_individual_set_mobilephone(individual_t *individual, char *mobile_phone);

int bank_individual_set_home_phone(individual_t *individual, char *home_phone);

int bank_individual_set_idcard(individual_t *individual, char *id_card_no);

int bank_individual_set_joindate(individual_t *individual, char *date);

int bank_individual_has_account(individual_t *individual);

char *bank_individual_set_uuid_new(individual_t *individual, char *uuid);

int bank_individual_set_account(individual_t *individual, account_t *account);

account_t *bank_individual_set_new_empty_account(individual_t *individual);

int bank_individual_set_login(individual_t *individual, login_t *login);

login_t *bank_individual_set_new_login(individual_t *individual, char *pass);

int bank_individual_authenticate_login(login_t *login_private, login_t *login);

int bank_individual_reset_login(individual_t *individual, login_t *login, char *newpass);

char *bank_individual_get(individual_t *individual, size_t flag);

account_t *bank_individual_get_account(individual_t *individual, bank_account_type type);

login_t *bank_individual_get_login(individual_t *individual);

int bank_individual_set_agency(individual_t *individual, agency_t *agency);

int bank_individual_remove_agency(individual_t *individual);

agency_t *bank_individual_get_agency(individual_t *individual);

individual_t *bank_search_individual(agency_t *agency, char *firstname, char *lastname, char *birthdate);

employee_t *bank_employee(employee_position position);

int bank_employee_is_position(employee_t *employee, employee_position position);

int bank_employee_is_status(employee_t *employee, status_type flag);

char *bank_employee_set_uuid(employee_t *employee);

int bank_employee_set_individual(employee_t *employee, individual_t *individual);

int bank_employee_set_uuid_new(employee_t *employee);

int bank_employee_set_login(employee_t *employee, login_t *login);

login_t *bank_employee_set_new_login(employee_t *employee, char *pass);

int bank_employee_reset_login(employee_t *employee, login_t *login, char *newpass);

int bank_employee_authenticate_login(employee_t *employee, login_t *login);

int bank_employee_set_agency(employee_t *employee, agency_t *agency);

int bank_employee_remove_agency(employee_t *employee);

login_t *bank_login();

login_t *bank_login_generate(agency_t *agency);

int bank_login_set_key(login_t *login, char *key);
int bank_login_set_id(login_t *login, char *id);

agency_t *bank_agency();

int bank_agency_is_status(agency_t *agency, status_type flag);

char *bank_agency_set_uuid(agency_t *agency);

int bank_agency_has_id(agency_t *agency);

int bank_agency_set_id(agency_t *agency, char *id);

int bank_agency_set_address(agency_t *agency, char *address);

int bank_agency_set_code(agency_t *agency, int agencycode);

char *bank_agency_get(agency_t *agency, size_t flag);

individual_t *bank_agency_get_individuals(agency_t *agency);

account_t *bank_agency_get_accounts(agency_t *agency);

employee_t *bank_agency_get_employees(agency_t *agency);

state_t *bank_agency_get_state(agency_t *agency);

int bank_agency_account_add(account_t *account);

int bank_agency_individual_add(individual_t *individual);

int bank_agency_employee_add(employee_t *employee);

state_t *bank_state();

int bank_state_set_status_empty(state_t *state);
int bank_state_set_status_init(state_t *state);

char *bank_state_set_uuid(state_t *state);

int bank_state_set_zipcode(state_t *state, int zipcode);

int bank_state_set_statecode(state_t *state, int statecode);

char *bank_state_get(state_t *state, size_t flag);

agency_t *bank_state_get_empty_agency(state_t *state);

agency_t *bank_state_get_agencies(state_t *state);
agency_t *bank_state_get_agency_n(state_t *state, int option, char *arg);

int bank_state_add_agency(state_t *state, agency_t *agency);

bank_t *bank();
void bank_set_status(bank_t *bank, status_type flag);
state_t *bank_get_state(bank_t *bank);
void bank_add_state(bank_t *data, state_t *state);
state_t *bank_get_state_n(bank_t *bank, int option, size_t num, char *name);

admin_t *bank_admin();

char *bank_admin_set_uuid(admin_t *admin);

login_t *bank_admin_set_new_login(admin_t *admin, char *pass);
int bank_admin_set_login(admin_t *admin, login_t *login);

int bank_admin_authenticate_login(admin_t *admin, login_t *login);

int bank_admin_set_agency(admin_t *admin, agency_t *agency);

int bank_admin_set_bank(admin_t *admin, bank_t *bank);

int bank_admin_add(admin_t *admin_list, admin_t *admin);
bank_t *bank_admin_get_bank(admin_t *admin);

int bank_login_set_status(login_t *login, status_type flag);

login_t *bank_login_authenticate(login_t *list, login_t *login);
login_t *bank_agency_get_logins(agency_t *agency);
int bank_agency_free_logins(agency_t *agency);

void free_login(login_t *login);

void free_admin(admin_t *admin);

void free_individual(individual_t *individual);

void free_agency(agency_t *agency);

void free_state(state_t *state);

int bank_free_logins(login_t *login);
void bank_agency_changed(agency_t *agency);
void bank_agency_set_status(agency_t *agency, status_type status);
int bank_employee_export_info(employee_t *employee);
void bank_individual_changed(individual_t *individual);
login_t *bank_employee_get_login(employee_t *employee);
int bank_state_is_status(state_t *state, status_type status);
char *bank_login_get_id(login_t *login);
int bank_agency_export_info(agency_t *agency);

int clear();
int modify_individual(individual_t *individual, individual_t *new_ind);
login_t *create_login(agency_t *agency, char *pass);
individual_t *create_individual(agency_t *agency, login_t *login, char sex, char *lastname, char *firstname, char *birthdate, char *email, char *address_no1, char *city, int zipcode, char *address_no2, char *mobile_phone, char *home_phone, char *id_card_no);
individual_t *scan_individual(login_t *login, agency_t *agency);
login_t *bank_login_add(login_t *list, login_t *login);

void bank_print_status(status_type status);

void bank_print_changes(status_type changes);

void bank_print_individual(individual_t *individual);

void bank_print_account(account_t *account);

void bank_print_agency_info(agency_t *agency);

void bank_print_state_info(state_t *state);

void bank_print_employee(employee_t *employee);



employee_t *bank_employee_get_next(employee_t *employee);

employee_t *bank_individual_get_employee(individual_t *individual);

state_t *bank_get_next_state(state_t *state, int flag);
agency_t *bank_agency_get_next(agency_t *agency, int flag);

int bank_print_accounts(account_t *account, size_t flag, int count);
int bank_employee_set_postion(employee_t *employee, size_t flag);

account_t *bank_account_get_n(account_t *account, char *iban);


int bank_individual_set_employee(individual_t *individual, employee_t *employee);
int bank_individual_compare(individual_t *ind1, individual_t *ind2);
int bank_agency_remove_individual(agency_t *agency, individual_t *individual);
int bank_export_client_info(individual_t *individual);
int bank_export_bank_account_info(individual_t *individual);
employee_t *bank_agency_get_employee_n(agency_t *agency, char *uuid);
char *bank_login_get_uuid(login_t *login);
int bank_account_changed(account_t *account);
individual_t *scan_modify_individual();
individual_t *bank_employee_get_individual(employee_t *employee);
int bank_money_transfer(account_t *account_sender, char *iban_reciever, double transaction_amount, char currency);
account_t *bank_individual_get_account_n(individual_t *individual, int num);
individual_t *bank_account_get_holder_n(account_t *account, int num);
int bank_export_account_activity(account_t *account, char *month, char *year);
#endif

#ifndef JSONBANK_H
	/* ----------------------------PARSING-------------------*/

	int bank_json_parse_individual(individual_t *individual, int option, size_t flags);
	int bank_json_parse_account(account_t *account, int option, size_t flags);
	int bank_json_parse_agency(agency_t *agency, int function, int option, size_t flags);

	int bank_json_parse_state(state_t *state, int function, int option, size_t flags);

	int bank_json_parse_bank(bank_t *bank, int option, int function);
	login_t *bank_json_parse_login(json_t *login_array);
	login_t *bank_json_parse_admin();

	int bank_json_dump_individual(individual_t *individual, int option, size_t flag);
	int bank_json_dump_account(account_t *account, size_t flag);
	int bank_json_dump_agency(agency_t *agency, int option, size_t flags);
	int bank_json_dump_state(state_t *state, int option, size_t flags);
	int bank_json_dump_bank(bank_t *bank, int option, size_t flags);
	int bank_json_dump_admin(login_t *admin, size_t flag);
	int bank_write_activity(account_t *account, char *activity);

	void *guaranteed_memset(void *v, int c, size_t n);

	void *secure_malloc(size_t size);

	void secure_free(void *ptr);

#ifndef LOGIN_H
/* ----------------------------LOGIN-------------------*/

int validate_login(login_t *private_log, login_t *user_log);
int encrypt_login_pass(login_t *login);


#endif

#endif


#ifndef RANDOM_H
/*-------------------RANDOM------------------------*/
char *login_id_decoder(char *login_id);
char *agency_id_generator();
char *login_id_generator(int *code);
#endif

#endif