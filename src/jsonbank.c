#include"jsonbank.h"

#define DATABASE_DIR "./database/"
#define BASIC_DATA "basic_data"
#define ADMIN_DATA "admin_data"
#define JSON_EXT ".json"
#define TMPDIR "./tmp/"
#define FILENAME_MAX_ 360

/* EMPLOYEE
#define MULTI_ACCESS
#define EMPLOYEE_ACCESS
#define LOGIN
#define EMPLOYEE_LOGIN

*/

/*ADMIN
#define MULTI_ACCESS
#define ADMIN_ACCESS
#define EMPLOYEE_ACCESS
*/


/*CLIENT
#define CLIENT_LOGIN
#define LOGIN
*/



#ifdef CLIENT_LOGIN
#define LOGIN
#endif
#ifdef EMPLOYEE_LOGIN
#define LOGIN
#endif



#ifdef ADMIN_ACCESS
#define MULTI_ACCESS
#define EMPLOYEE_ACCESS
#define ADMIN_LOGIN
#endif


#define CLIENTS_LOGIN 0x0122
#define CLIENT_RESET_LOGIN 0x222
#define ACCOUNTS_PRINTING 0X322
#define EMPLOYEES_LOGIN 0x422
#define CLIENTS_DATA 0x522
#define EMPLOYEES_DATA 0x622

void *guaranteed_memset(void *v, int c, size_t n)
{
	volatile char *p = v;
	while (n--)
		*p++ = c;
	return v;
}

void *secure_malloc(size_t size)
{
	/* Store the memory area size in the beginning of the block */
	void *ptr = malloc(size + 8);
	*((size_t *)ptr) = size;
	return ptr + 8;
}

void secure_free(void *ptr)
{
	size_t size;

	ptr -= 8;
	size = *((size_t *)ptr);

	guaranteed_memset(ptr, 0, size + 8);
	free(ptr);
}

static int path_finder(const char *filename)
{

	char cmd[360];

	sprintf(cmd, "mkdir -p %s", filename);

	FILE *fp = popen(cmd, "r");

	if (!fp)
	{
		fail("unable to find the JSON file PATH");
		return EXIT_FAILURE;
	}

	// fscanf(fp, "%216s", path);

	pclose(fp);

	return EXIT_SUCCESS;
}

static int json_dump_data_file(json_t *root, const char* path, const char* file_uuid, size_t flag)
{

	char *filename = calloc((strlen(path) + strlen(file_uuid) + 17 + 1) , sizeof(char));

	strcpy(filename, DATABASE_DIR);

	if(path!=NULL)
	{
		strcat(filename, path);

	}

	path_finder(filename);

	strcat(filename, file_uuid);

	strcat(filename, JSON_EXT);

	int res = json_dump_file(root, filename, flag);

	if (res == -1)
	{
		json_decref(root);

		fail("json_dump_file returned an invalid error");

		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

static int json_dump_data_file_tmp(json_t *root, const char *file_uuid, size_t flag)
{

	char *filename = malloc((strlen(file_uuid) + 6 + 5 + 1) * sizeof(char));

	strcat(filename, TMPDIR);
	strcat(filename, file_uuid);
	strcat(filename, ".json");

	int res = json_dump_file(root, filename, flag);

	if (res == -1)
	{
		json_decref(root);

		fail("json_dump_file returned an invalid error");

		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

static json_t *json_load_data_file(const char *file_uuid, size_t flags)
{
	json_t *json;
	json_error_t error;

	char *path = calloc((strlen(file_uuid) + 17 + 1), sizeof(char));

	strcpy(path, DATABASE_DIR);

	strcat(path, file_uuid);

	strcat(path, JSON_EXT);

	json = json_load_file(path, flags, &error);

	free(path);

	if (!json)
	{
		fail("json_load_file returned an invalid error");
		fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
		return NULL;
	}

	return json;
}



int bank_json_parse_individual(individual_t *individual, int option, size_t flags)
{
	if (individual == NULL || individual->status != BANK_OBJECT_INIT)
	{
		fail("error bank_json_parse_individual(1)");
		return EXIT_FAILURE;
	}

	char *path = calloc(strlen(individual->agency->state->uuid_state) + strlen(individual->agency->uuid_agency) + strlen(individual->uuid) + 15 + 1, sizeof(char));

	strcpy(path, individual->agency->state->uuid_state);

	strcat(path, "/");

	strcat(path, individual->agency->uuid_agency);

	strcat(path, "/individuals/");

	strcat(path, individual->uuid);

	json_error_t error;

	json_t *root = json_load_data_file(path, flags);

	char *uuid, *lastname, *firstname, *birthdate, *email, *address_no1, *address_no2, *city, *mobile_phone, *home_phone, *id_card_no, *joineddate;
	int res, status, zip_code, sex, flag, uuid_size, bd_size, jd_size;

	res=json_unpack_ex(root, &error, JSON_ALLOW_NUL, "{ s:i, s:s%, s:i, s:s, s:s, s:s%, s:s, s:s, s:s, s:i, s:s, s:s, s:s, s:s, s:s%, s:i}", "status", &status, "uuid", &uuid, &uuid_size,"sex", &sex, "lastname", &lastname, "firstname", &firstname, "birthdate", &birthdate, &bd_size, "email", &email, "address_no1", &address_no1, "city", &city, "zipcode", &zip_code, "address_no2", &address_no2, "mobilephone", &mobile_phone, "homephone", &home_phone, "id_card_no", &id_card_no, "joineddate", &joineddate, &jd_size, "flag", &flag);

	if(res==-1 )
	{
		fail("error bank_json_parse_individual(2)");
		fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
		json_decref(root);
		return EXIT_FAILURE;
	}

#ifdef ADMIN_ACCESS

	if (strcmp(individual->uuid, uuid) != 0 || strcmp(individual->firstname, firstname) != 0 || strcmp(individual->lastname, lastname) != 0 || strcmp(individual->birthdate, birthdate) != 0 || uuid_size != UUID_SIZE - 1 || bd_size != DATE_SIZE - 1 || jd_size != DATE_JOIN_SIZE - 1 || (sex != 'M' && sex != 'F'))
	{
		fail("error bank_json_parse_individual(2): ADMIN ACCESS VERIFICATION");
		json_decref(root);
		return EXIT_FAILURE;
	}

#endif

	individual->status = BANK_OBJECT_EMPTY;

#ifdef CLIENT_LOGIN
	bank_individual_set_birthdate(individual, birthdate);
	bank_individual_set_firstname(individual, firstname);
	bank_individual_set_lastname(individual, lastname);
#endif

	bank_individual_set_address_no1(individual, address_no1);

	if(address_no2!=NULL)
	{
		bank_individual_set_address_no2(individual, address_no2);
	}

	bank_individual_set_city(individual, city);

	bank_individual_set_email(individual, email);

	strcpy(individual->mobile_phone, mobile_phone);

	if(home_phone!=NULL)
	{
		strcpy(individual->home_phone, home_phone);

	}
	strcpy(individual->joineddate, joineddate);

	bank_individual_set_idcard(individual, id_card_no);

	individual->zip_code=zip_code;
	individual->sex=sex;
	individual->status=status;
	individual->flag=flag;

	json_t* value, *account_array=json_object_get(root, "accounts");
	size_t index;

	json_array_foreach(account_array, index, value)
	{

		res=json_unpack_ex(value, &error, 0, "{s:s%}", "uuid_account", &uuid , &uuid_size);

		if (res ==-1 || uuid_size!=(UUID_SIZE-1))
		{
			fail("error bank_json_parse_individual(3)");
			fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
			json_decref(root);
			return EXIT_FAILURE;
		}

#ifdef ADMIN_ACCESS

		if(uuid_size != UUID_SIZE -1 )
		{
			fail("error bank_json_parse_individual(3): ADMIN ACCESS VERIFICATION");
			json_decref(root);
			return EXIT_FAILURE;
		}
#endif

		individual->bank_account[index]=bank_account(BANK_ACCOUNT_EMPTY, BANK_OBJECT_NULL);

		strcpy(individual->bank_account[index]->uuid_account, uuid);

		individual->bank_account[index]->status=BANK_OBJECT_INIT;

		individual->bank_account[index]->agency=individual->agency;

		if(option==1)
		{
			bank_json_parse_account(individual->bank_account[index], JSON_ALLOW_NUL);
			bank_agency_account_add(individual->bank_account[index]);
		}
	}

	json_decref(root);
	free(path);
	return EXIT_SUCCESS;
}




int bank_json_parse_account(account_t *account, size_t flags)
{
	if (account == NULL || account->status != BANK_OBJECT_INIT)
	{
		fail("error bank_json_parse_account(1)");
		return EXIT_FAILURE;
	}

	char *path = calloc(strlen(account->agency->state->uuid_state) + strlen(account->agency->uuid_agency) + strlen(account->uuid_account) + 15 + 1, sizeof(char));

	strcpy(path, account->agency->state->uuid_state);

	strcat(path, "/");

	strcat(path, account->agency->uuid_agency);

	strcat(path, "/accounts/");

	strcat(path, account->uuid_account);

	json_error_t error;

	json_t *root = json_load_data_file(path, flags);

	char *uuid_account, *uuid_activity, *account_no, *iban;
	int res, status, account_type, bank_account_type;
	double balance;

	res = json_unpack_ex(root, &error, 0, "{s:i, s:s, s:s, s:s, s:s, s:f, s:i, s:i}", "status", &status, "uuid", &uuid_account, "uuid_activity", &uuid_activity, "iban", &iban, "account_no", &account_no, "balance", &balance, "bank_account_type", &bank_account_type, "account_type", &account_type);


	if (res == -1)
	{
		fail("error bank_json_parse_account(2)");
		fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
		json_decref(root);
		return EXIT_FAILURE;
	}

	if (account->iban[0]=='\0')
	{
		strcpy(account->iban, iban);

		account->status = status;

		account->bank_account_type = bank_account_type;

		account->account_type = account_type;

		account->account_balance = balance;
	}

	strcpy(account->account_no, account_no);

	strcpy(account->uuid_activity, uuid_activity);

	json_decref(root);

	free(path);

	return EXIT_SUCCESS;
}


int bank_json_parse_agency(agency_t *agency, int function, int option, size_t flags)
{
	if (agency == NULL )
	{
		fail("error bank_json_parse_agency(0) NULL");

		return EXIT_FAILURE;
	}

#ifndef ADMIN_ACCESS
	if ( agency->status != BANK_OBJECT_INIT)
	{
		fail("bank_json_parse_agency(0) REPARSING EMPTY AGENCY IGNORED");

		return EXIT_SUCCESS;
	}
#endif

	char *path = calloc(strlen(agency->state->uuid_state) + strlen(agency->uuid_agency) + 3 + 1, sizeof(char));

	strcpy(path, agency->state->uuid_state);

	strcat(path, "/");

	strcat(path, agency->uuid_agency);


	json_t* root=json_load_data_file(path, flags);

	json_error_t error;

	int res, status;
/*ONLY FOR VERIFICATIONS VIA ADMIN PAGE*/
#ifdef ADMIN_ACCESS

	char* uuid_agency, *agency_id, *agency_address, *agency_code;

	res = json_unpack_ex(root, &error, JSON_ALLOW_NUL, "{s:i, s:s, s:s, s:s, s:s }", "status", &status, "uuid", &uuid_agency, "agency_id", &agency_id, "agency_address", &agency_address, "agency_code", &agency_code);

	if (res==-1 || strcmp(agency->uuid_agency, uuid_agency) != 0 || strcmp(agency->agency_code, agency_code) != 0 || strcmp(agency->agency_id, agency_id)!=0)
	{
		fail("error bank_json_parse_agency(1)");
		fprintf(stderr, "agency_t and agency.json does not belong to eachothers\n");
		json_decref(root);
		return EXIT_FAILURE;
	}

	agency->status=status;

	if(agency_address!=NULL)
	{
		if(agency->status==BANK_OBJECT_EMPTY)
		{
			fail("error bank_json_parse_agency CONFLICTING DATA");
			json_decref(root);
			return EXIT_FAILURE;
		}
		json_decref(root);
		return EXIT_SUCCESS;
	}
#endif

	json_t* value, *value_log;

	size_t index=0, index_log_cli=0;

	char* login_id, *login_key, *uuid, *lastname, *firstname, *birthdate;

	individual_t* iter_indiv;

	login_t* iter_login;

	account_t* iter_account;

	employee_t* iter_employee;


	/* AGENCY-> LOGINS*/
	if(function==CLIENTS_LOGIN)
	{

		json_t *login_client_array = json_object_get(root, "login_client");

		agency->logins=bank_json_parse_login(login_client_array);

		return EXIT_SUCCESS;
	}


	/* AGENCY-> ACCOUNTS */
	if(function==CLIENT_RESET_LOGIN || function==ACCOUNTS_PRINTING)
	{
		//accounts
		int bank_account_type;
		char *iban;
		json_t *account_array = json_object_get(root, "accounts");

		json_array_foreach(account_array, index, value)
		{
			res = json_unpack_ex(value, &error, JSON_ALLOW_NUL, "{s:i, s:s, s:i, s:s}", "status", &status, "uuid", &uuid, "bank_account_type", &bank_account_type, "iban", &iban);

			if (res == -1)
			{
				fail("error bank_json_parse_agency(5): ACCOUNTS INTERFACE");
				fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
				json_decref(root);
				return EXIT_FAILURE;
			}

			iter_account = bank_account(bank_account_type, BANK_ACCOUNT_NONE);

			iter_account->status = status;

			strcpy(iter_account->iban, iban);

			strcpy(iter_account->uuid_account, uuid);

			iter_account->agency = agency;

			bank_agency_account_add(iter_account);
		}
		json_decref(root);
		return EXIT_SUCCESS;
	}



#ifdef EMPLOYEE_ACCESS

	if (function==EMPLOYEES_LOGIN)
	{
		json_t *login_employee_array = json_object_get(root, "login_employee");

		agency->logins=bank_json_parse_login(login_employee_array);

		return EXIT_SUCCESS;
	}

	/* AGENCY -> INDIVIDUALS\{employees}-> (LOGIN && ACCOUNTS) */
	if(function==CLIENTS_DATA)
	{

		json_t *login_client_array = json_object_get(root, "login_client");

		json_t *individual_array = json_object_get(root, "individuals");

		json_array_foreach(individual_array, index, value)
		{

			//individual
			res = json_unpack_ex(value, &error, JSON_ALLOW_NUL, "{s:i, s:s, s:s, s:s, s:s}", "status", &status, "uuid", &uuid, "lastname", &lastname, "firstname", &firstname, "birthdate", &birthdate);

			if (res == -1)
			{
				fail("error bank_json_parse_agency(2)");
				fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
				json_decref(root);
				return EXIT_FAILURE;
			}

			iter_indiv = bank_individual(BANK_ACCOUNT_EMPTY);

			iter_indiv->status = BANK_OBJECT_EMPTY;

			bank_individual_set_lastname(iter_indiv, lastname);
			bank_individual_set_firstname(iter_indiv, firstname);
			bank_individual_set_birthdate(iter_indiv, birthdate);

			strcpy(iter_indiv->uuid, uuid);

			if (status != BANK_LOGIN_EMP)
			{
				value_log = json_array_get(login_client_array, index_log_cli);
				index_log_cli++;
			}
			else
			{
				value_log = NULL;
			}

			//login

			if (value_log != NULL)
			{
				int status_;

				res = json_unpack_ex(value_log, &error, JSON_ALLOW_NUL, "{s:i, s:s, s:s, s:s}", "status", &status_, "uuid", &uuid, "login_id", &login_id, "login_key", &login_key);

				if (res == -1)
				{
					fail("error bank_json_parse_agency(3)");
					fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
					json_decref(root);
					return EXIT_FAILURE;
				}

				iter_login = bank_login();

				iter_indiv->status = status_;

				iter_indiv->agency = agency;

				iter_indiv->login = iter_login;
/*-------------------------------------------NEEEEEDS TESTING -------------------------------*/
				if (strcmp(iter_indiv->uuid, uuid) != 0)
				{
					fail("error bank_json_parse_agency(4)");
					fprintf(stderr, "login and individual does not belong to eachothers\n");
					json_decref(root);
					return EXIT_FAILURE;
				}

				iter_login->status = status;

				strcpy(iter_login->login_id, login_id);

				strcpy(iter_login->login_key, login_key);

				bank_agency_individual_add(iter_indiv);

				if (option == 1)
				{
					bank_json_parse_individual(iter_indiv, 1, 0);
				}

			}
		}

	}

#endif

#ifdef MULTI_ACCESS
	/* AGENCY->EMPLOYEE -> INDIVIDUAL-> (LOGIN  && ACCOUNTS )*/
	if(function==EMPLOYEES_DATA)
	{


		json_t *employee_array = json_object_get(root, "employees");

		int position;

		//employee

		json_array_foreach(employee_array, index, value)
		{
			res = json_unpack_ex(value, &error, JSON_ALLOW_NUL, "{s:i, s:i, s:s, s:s, s:s, s:s }", "status", &status, "position", &position, "uuid", &uuid, "lastname", &lastname, "firstname", &firstname, "birthdate", &birthdate);

			if (res == -1)
			{
				fail("error bank_json_parse_agency(6): MANAGERS INTERFACE");
				fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
				json_decref(root);
				return EXIT_FAILURE;
			}

#ifndef EMPLOYEE_LOGIN
			iter_employee = bank_employee(position);
			iter_employee->status = status;
			iter_employee->position = position;

			iter_employee->agency = agency;

			iter_indiv = bank_individual(BANK_ACCOUNT_EMPTY);

			iter_indiv->status = BANK_OBJECT_EMPTY;

			bank_individual_set_lastname(iter_indiv, lastname);

			bank_individual_set_firstname(iter_indiv, firstname);

			bank_individual_set_birthdate(iter_indiv, birthdate);

			strcpy(iter_indiv->uuid, uuid);

			iter_indiv->agency = agency;

			iter_login = agency->logins;

			while (iter_login != NULL)
			{
				if (strcmp(iter_login->uuid, iter_indiv->uuid) == 0)
				{
					iter_indiv->login = iter_login;
					free(iter_login->uuid);
					break;
				}
				iter_login = iter_login->next;
			}

			if (iter_indiv->login == NULL)
			{
				fail("error bank_json_parse_agency(7): MANAGERS INTERFACE");
				json_decref(root);
				return EXIT_FAILURE;
			}
			bank_agency_employee_add(iter_employee);
#else
			if(strcmp(uuid, agency->employees->personal_data->uuid)==0)
			{
				iter_indiv=agency->employees->personal_data;

				iter_employee=agency->employees;

				iter_employee->status = status;
				iter_employee->position = position;

				iter_indiv->status = BANK_OBJECT_EMPTY;
				iter_indiv->agency = agency;

				bank_individual_set_lastname(iter_indiv, lastname);

				bank_individual_set_firstname(iter_indiv, firstname);

				bank_individual_set_birthdate(iter_indiv, birthdate);
#endif
				if (option == 1)
				{
					iter_indiv->status = BANK_OBJECT_INIT;

					bank_json_parse_individual(iter_indiv, 1, 0);
				}
#ifdef EMPLOYEE_LOGIN
			}
#endif
		}

	}
#endif

	json_decref(root);
	free(path);

	return EXIT_SUCCESS;


}

int bank_json_parse_state(state_t *state, int function, int option, size_t flags)
{
	if (state == NULL)
	{
		fail("error bank_json_parse_state(0) NULL");
		return EXIT_FAILURE;
	}
#ifndef ADMIN_ACCESS
	if (state->status!=BANK_OBJECT_INIT)
	{
		fail("bank_json_parse_state(0) REPARSING INGORING AN EMPTY STATE");
		return EXIT_SUCCESS;
	}
#endif

	char *path = calloc(strlen(state->uuid_state) + 1, sizeof(char));

	strcpy(path, state->uuid_state);


	json_t *root = json_load_data_file(path, flags);
	json_error_t error;
	int res, status;

#ifdef ADMIN_ACCESS

	int zip_code;
	char* uuid, *state_code;

	res = json_unpack_ex(root, &error, JSON_ALLOW_NUL, "{s:i, s:s, s:i, s:s}", "status", &status, "uuid", &uuid, "zipcode", &zip_code, "statecode", &state_code);

	if(res==-1)
	{
		fail("error bank_json_parse_state(1)");
		fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
		json_decref(root);
		return EXIT_FAILURE;
	}

	if(status!=state->status || strcmp(state->uuid_state, uuid)!=0 || zip_code!=(state->zip_code) || strcmp(state_code,state->state_code)!=0 )
	{
		fail("error bank_json_parse_state(2): CONFLICTING DATA");
		fprintf(stderr, "\nstatus %d:%d", state->status, status);
		fprintf(stderr, "\nuuid %s:%s", state->uuid_state, uuid);
		fprintf(stderr, "\nzipcode %d:%d",state->zip_code,  zip_code );
		fprintf(stderr, "\nstatecode %s:%s", state->state_code, state_code);
		json_decref(root);
		return EXIT_FAILURE;
	}

#endif

	json_t *agency_array, *value;

	char *uuid_agency, *agency_id, *agency_address, *agency_code;

	size_t index;

	agency_t* iter_agency;

	agency_array=json_object_get(root, "agencies");

	json_array_foreach(agency_array, index, value)
	{

		res = json_unpack_ex(value, &error, JSON_ALLOW_NUL, "{s:i, s:s, s:s, s:s, s:s }", "status", &status, "uuid", &uuid_agency, "agency_id", &agency_id, "agency_address", &agency_address, "agency_code", &agency_code);

		if(res==-1)
		{
			fail("error bank_json_parse_state(1)");
			fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
			json_decref(root);
			return EXIT_FAILURE;
		}
#ifndef ADMIN_ACCESS

		if(agency_address!=NULL)
		{
			iter_agency = bank_agency();

			strcpy(iter_agency->uuid_agency, uuid_agency);

			strcpy(iter_agency->agency_code, agency_code);

			strcpy(iter_agency->agency_id, agency_id);

			iter_agency->status = BANK_OBJECT_EMPTY;

			bank_agency_set_address(iter_agency, agency_address);

			iter_agency->status = BANK_OBJECT_EMPTY;

			bank_state_add_agency(state, iter_agency);

			iter_agency->status = status;

			if (option == 1 && iter_agency->status == BANK_OBJECT_INIT)
			{
				bank_json_parse_agency(iter_agency, function, 0, JSON_ALLOW_NUL);
			}
		}

#else
		iter_agency=bank_agency();

		if(iter_agency->uuid_agency[0]=='\0')
		{
			strcpy(iter_agency->uuid_agency, uuid_agency);

			strcpy(iter_agency->agency_code, agency_code);

			strcpy(iter_agency->agency_id, agency_id);
		}

		iter_agency->status=BANK_OBJECT_EMPTY;

		if(agency_address!=NULL)
		{
			bank_agency_set_address(iter_agency, agency_address);
		}

		iter_agency->status=BANK_OBJECT_EMPTY;

		bank_state_add_agency(state, iter_agency);

		iter_agency->status=status;

		if (option == 1)
		{
			bank_json_parse_agency(iter_agency, function, 1, JSON_ALLOW_NUL);
		}
#endif
	}

	json_decref(root);
	free(path);
	return EXIT_SUCCESS;

}


int bank_json_parse_bank(bank_t *bank, int option, int function)
{
	if(bank==NULL )
	{
		fail("error bank_json_parse_bank(0) NULL");
		return EXIT_FAILURE;
	}
	if( bank->state!=NULL)
	{
		fail("error bank_json_parse_bank(0) BANK HAS STATES ALREADY");
		return EXIT_FAILURE;
	}

	json_t* root=json_load_data_file(BASIC_DATA, JSON_ALLOW_NUL);

	if(!root)
	{
		fail("error bank_json_parse_bank NO DATA WAS FOUND");
		abort();
		exit (EXIT_FAILURE);
	}
	json_t* value;

	json_error_t error;

	size_t index;

	int res;

	int zip_code, status;

	char *uuid, *state_code;

	state_t* iter_state;

	json_array_foreach(root, index, value)
	{

		res = json_unpack_ex(value, &error, JSON_ALLOW_NUL, "{s:i, s:s, s:i, s:s}", "status", &status, "uuid", &uuid, "zipcode", &zip_code, "statecode", &state_code);

		if (res == -1)
		{
			fail("error bank_json_parse_bank(1)");
			fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
			json_decref(root);
			return EXIT_FAILURE;
		}
#ifdef LOGIN

	if(status==BANK_OBJECT_INIT)
	{
		iter_state = bank_state();

		iter_state->status = status;

		strcpy(iter_state->uuid_state, uuid);

		iter_state->zip_code = zip_code;

		strcpy(iter_state->state_code, state_code);

		bank_add_state(bank, iter_state);
	}

#else
		iter_state=bank_state();

		iter_state->status=status;

		strcpy(iter_state->uuid_state, uuid);

		iter_state->zip_code = zip_code;

		strcpy(iter_state->state_code, state_code);

		bank_add_state(bank, iter_state);

#ifndef ADMIN_ACCESS

		if(option==1 && iter_state->status==BANK_OBJECT_INIT)
		{
			bank_json_parse_state(iter_state, 0, 0, JSON_ALLOW_NUL);
		}

#else
		if (option == 1)
		{
			bank_json_parse_state(iter_state, 0, 0, JSON_ALLOW_NUL);
		}

#endif

#endif
	}
	json_decref(root);
	return EXIT_SUCCESS;

}

login_t* bank_json_parse_login(json_t* login_array)
{

	login_t *list_login = bank_login();

	login_t* login= list_login;

	json_t* value;
	json_error_t error;
	size_t index;


	int res, status;

	char* login_id, *login_key, *uuid;

	login_t* iter_login;

	json_array_foreach(login_array, index, value)
	{
#ifdef ADMIN_LOGIN
		res = json_unpack_ex(value, &error, JSON_ALLOW_NUL, "{s:i, s:s, s:s}", "status", &status, "login_id", &login_id, "login_key", &login_key);
#else
		res = json_unpack_ex(value, &error, JSON_ALLOW_NUL, "{s:i, s:s, s:s, s:s}", "status", &status, "uuid", &uuid, "login_id", &login_id, "login_key", &login_key);

#endif
		if (res == -1)
		{
			fail("error bank_json_parse_agency(3): LOGIN INTERFACE");
			fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
			json_decref(login_array);
			return NULL;
		}

		iter_login = bank_login();

#ifndef ADMIN_LOGIN
		iter_login->uuid = calloc(UUID_SIZE, sizeof(char));
		strcpy(iter_login->uuid, uuid);
#endif
		iter_login->status = BANK_LOGIN_ENCRYPTED;


		strcpy(iter_login->login_id, login_id);

		strcpy(iter_login->login_key, login_key);

		list_login->next = iter_login;

		list_login = list_login->next;
	}

	login = login->next;

	return login;
}

login_t* bank_json_parse_admin()
{

	json_t* root=json_load_data_file(ADMIN_DATA,JSON_ALLOW_NUL);

	if(!root)
	{
		fail("error bank_json_parse_Admin(1) NO DATA WERE FOUND");

		return NULL;

	}

	login_t* admin=bank_json_parse_login(root);



	return admin;
}

int bank_json_dump_individual(individual_t *individual, int option, size_t flag)
{
	if(individual==NULL || individual->status!=BANK_OBJECT_INIT)
	{
		fail("error bank_json_dump_individual(1)");
		return EXIT_FAILURE;
	}

	if(individual->changes!=BANK_OBJECT_CHANGED)
	{
		fail("bank_json_dump_individual(0) NO CHANGES WERE RECORDED, DUMPING IGNORED");
		return EXIT_SUCCESS;
	}
	json_error_t error;

	json_t* root=json_pack_ex(&error, JSON_ALLOW_NUL, "{ s:i, s:s, s:i, s:s, s:s, s:s, s:s, s:s, s:s, s:i, s:s, s:s, s:s, s:s, s:s, s:i}", "status", individual->status, "uuid", individual->uuid, "sex", individual->sex, "lastname", individual->lastname, "firstname", individual->firstname, "birthdate",individual->birthdate, "email", individual->email, "address_no1", individual->address_no1, "city", individual->city, "zipcode", individual->zip_code, "address_no2", individual->address_no2, "mobilephone", individual->mobile_phone, "homephone",individual->home_phone, "id_card_no", individual->id_card_no, "joineddate", individual->joineddate, "flag", individual->flag);

	json_t* accounts_array=json_array();

	account_t** iter= individual->bank_account;

	int i;

	for(i=0; i<4; i++)
	{
		if(iter[i]!=NULL)
		{
			json_array_append_new(accounts_array, json_pack("{s:s}", "uuid_account", iter[i]->uuid_account));
		}

	}

	json_object_set_new_nocheck(root, "accounts", accounts_array);

	if(!root)
	{
		fail("error bank_json_dump_individual(2)");
		fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
		return EXIT_FAILURE;
	}

	char *path = calloc(strlen(individual->agency->state->uuid_state) + strlen(individual->agency->uuid_agency) +15+ 1, sizeof(char));

	strcpy(path, individual->agency->state->uuid_state);

	strcat(path, "/");

	strcat(path, individual->agency->uuid_agency);

	strcat(path, "/");

	strcat(path, "individuals");

	strcat(path, "/");

	if(option==0)
	{

		json_dump_data_file(root, path, individual->uuid, flag);
	}
	else
	{
		json_dump_data_file_tmp(root, individual->uuid, flag);
	}

	json_decref(root);
	free(path);
	return EXIT_SUCCESS;

}



int bank_json_dump_account(account_t *account, size_t flag)
{
	if(account==NULL || account->status!=BANK_OBJECT_INIT)
	{

		fail("error bank_json_dump_account(1)");
		return EXIT_FAILURE;
	}

	if (account->changes != BANK_OBJECT_CHANGED)
	{
		fail("bank_json_dump_account(0) NO CHANGES WERE RECORDED, DUMPING IGNORED");
		return EXIT_SUCCESS;
	}

	json_error_t error;
	json_t* root=json_pack_ex(&error, 0, "{s:i, s:s, s:s, s:s, s:s, s:f, s:i, s:i}","status", account->status, "uuid", account->uuid_account, "uuid_activity", account->uuid_activity, "iban", account->iban, "account_no", account->account_no, "balance", account->account_balance, "bank_account_type", account->bank_account_type, "account_type", account->account_type);

	json_t *individuals_array = json_array();

	individual_t **iter = account->account_holder;

	int i;

	for (i = 0; i < 2; i++)
	{
		if (iter[i] != NULL)
		{
			json_array_append_new(individuals_array, json_pack("{s:s}", "uuid_individual", iter[i]->uuid));
		}
	}

	json_object_set_new_nocheck(root, "holders", individuals_array);

	if (!root)
	{
		fail("error bank_json_dump_account(2)");
		fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
		return EXIT_FAILURE;
	}

	char *path = calloc(strlen(account->agency->state->uuid_state) + strlen(account->agency->uuid_agency) + 12 + 1, sizeof(char));

	strcpy(path, account->agency->state->uuid_state);

	strcat(path, "/");

	strcat(path, account->agency->uuid_agency);

	strcat(path, "/");

	strcat(path, "accounts");

	strcat(path, "/");

	json_dump_data_file(root, path, account->uuid_account, flag);

	free(path);
	json_decref(root);
	return EXIT_SUCCESS;

}



int bank_json_dump_agency(agency_t *agency, int option, size_t flags)
{
	if(agency==NULL )
	{
		fail("error bank_json_dump_agency(1)");
		return EXIT_FAILURE;
	}

	json_t *root;
	json_error_t error;

	if (agency->status!=BANK_OBJECT_INIT || agency->agency_address == NULL)
	{
		fail("bank_json_dump_agency(0) EMPTY AGENCY IGNORED");
		return EXIT_SUCCESS;
	}

	if (agency->changes != BANK_OBJECT_CHANGED)
	{
		fail("bank_json_dump_agency(0) NO CGANGES WERE RECORDED, DUMPING IGNORED");
		return EXIT_SUCCESS;
	}

	root = json_pack_ex(&error, JSON_ALLOW_NUL, "{s:i, s:s, s:s, s:s, s:s }", "status", agency->status, "uuid", agency->uuid_agency, "agency_id", agency->agency_id, "agency_address", agency->agency_address, "agency_code", agency->agency_code);

	if (!root)
	{
		fail("error bank_json_dump_agency(2)");
		fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
		return EXIT_FAILURE;
	}


	json_t *employee_array, *individual_array, *accounts_array, *login_array_emp, *login_array;
	json_t *employee, *individual, *account, *login;


	switch (agency->status)
	{
	case BANK_OBJECT_INIT:
	{
		individual_t* iter_ind=agency->individuals;

		individual_array=json_array();
		login_array=json_array();
		login_array_emp=json_array();

		while(iter_ind!=NULL)
		{
			//login

			login = json_pack_ex(&error, 0, "{s:i, s:s, s:s, s:s}", "status", iter_ind->status, "uuid", iter_ind->uuid, "login_id", iter_ind->login->login_id, "login_key", iter_ind->login->login_key);

			if (!login)
			{
				fail("error bank_json_dump_agency(4)");
				fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
				json_decref(root);
				return EXIT_FAILURE;
			}
			int status;
			if (iter_ind->login->status != BANK_LOGIN_EMP)
			{
				json_array_append_new(login_array, login);
				status=iter_ind->status;
			}
			else
			{
				json_array_append_new(login_array_emp, login);
				status=BANK_LOGIN_EMP;
			}

			//individual

			individual=json_pack_ex(&error, JSON_ALLOW_NUL, "{s:i, s:s, s:s, s:s, s:s}", "status", status, "uuid", iter_ind->uuid, "lastname", iter_ind->lastname, "firstname", iter_ind->firstname, "birthdate", iter_ind->birthdate);

			if(!individual)
			{
				fail("error bank_json_dump_agency(3)");
				fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
				json_decref(root);
				return EXIT_FAILURE;
			}

			json_array_append_new(individual_array, individual);

			if (option == 1 && iter_ind->changes == BANK_OBJECT_CHANGED)
			{
				bank_json_dump_individual(iter_ind, 0, flags);
			}

			iter_ind=iter_ind->next;

		}

		json_object_set_new_nocheck(root, "login_client", login_array);

		json_object_set_new_nocheck(root, "login_employee", login_array_emp);

		json_object_set_new_nocheck(root, "individuals", individual_array);


		//accounts

		account_t* iter_account=agency->bank_accounts;

		accounts_array=json_array();

		while(iter_account!=NULL)
		{
			account=json_pack_ex(&error, JSON_ALLOW_NUL, "{s:i, s:s, s:i, s:s}","status", iter_account->status, "uuid", iter_account->uuid_account, "bank_account_type", iter_account->bank_account_type, "iban", iter_account->iban);

			if(!account)
			{
				fail("error bank_json_dump_agency(5)");
				fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
				json_decref(root);
				return EXIT_FAILURE;
			}

			json_array_append_new(accounts_array, account);

			if (option == 1 && iter_account->changes == BANK_OBJECT_CHANGED)
			{
				bank_json_dump_account(iter_account, flags);
			}

			iter_account=iter_account->next;
		}

		json_object_set_new_nocheck(root, "accounts", accounts_array);

		//employees

		employee_t* iter_emp=agency->employees;

		employee_array=json_array();

		while(iter_emp!=NULL)
		{
			employee=json_pack_ex(&error, JSON_ALLOW_NUL, "{s:i, s:i, s:s, s:s, s:s, s:s }", "status", iter_emp->status, "position", iter_emp->position, "uuid",iter_emp->personal_data->uuid, "lastname", iter_emp->personal_data->lastname, "firstname", iter_emp->personal_data->firstname, "birthdate", iter_emp->personal_data->birthdate);

			if(!employee)
			{
				fail("error bank_json_dump_agency(6)");
				fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
				json_decref(root);
				return EXIT_FAILURE;
			}
			json_array_append_new(employee_array, employee);

			iter_emp=iter_emp->next;
		}

		json_object_set_new_nocheck(root, "employees", employee_array);

		char *path = calloc(strlen(agency->state->uuid_state) + 1 + 1, sizeof(char));

		strcpy(path, agency->state->uuid_state);

		strcat(path, "/");

		json_dump_data_file(root, path, agency->uuid_agency, flags);

		json_decref(root);

		free(path);

		return EXIT_SUCCESS;
	}
	default:
		fail("error end of bank_json_dump_agency");
		json_decref(root);
		return EXIT_FAILURE;
	}
}





int bank_json_dump_state(state_t *state, int option, size_t flags)
{
	if(state==NULL)
	{

		fail("error bank_json_dump_state(1)");
		return EXIT_FAILURE;

	}

	if(state->changes!=BANK_OBJECT_CHANGED)
	{

		fail("bank_json_dump_state(0) NO CHANGES WERE RECORDED, DUMPING IGNORED");
		return EXIT_SUCCESS;

	}

	json_t* root;

	json_error_t error;

	root=json_pack_ex(&error, JSON_ALLOW_NUL, "{s:i, s:s, s:i, s:s}", "status", state->status, "uuid", state->uuid_state, "zipcode", state->zip_code, "statecode", state->state_code);

	if(!root)
	{
		fail("error bank_json_dump_state(2)");
		fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
		return EXIT_FAILURE;
	}



		json_t *agency_array=json_array();

		json_t *agency;

		agency_t* iter_agency=state->agencies;

		char *agency_add;

		while (iter_agency != NULL)
		{
			if (iter_agency->agency_address == NULL)
			{
				agency_add="";
			}
			else
				agency_add = iter_agency->agency_address;

			agency = json_pack_ex(&error, JSON_ALLOW_NUL, "{s:i, s:s, s:s, s:s, s:s }", "status", iter_agency->status, "uuid", iter_agency->uuid_agency, "agency_id", iter_agency->agency_id, "agency_address", agency_add, "agency_code", iter_agency->agency_code);

			if (!agency)
			{
				fail("error bank_json_dump_state(3)");
				fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
				json_decref(root);
				return EXIT_FAILURE;
			}

			json_array_append_new(agency_array, agency);
// #ifndef ADMIN_ACCESS
			if (option == 1 && iter_agency->status==BANK_OBJECT_INIT && iter_agency->changes==BANK_OBJECT_CHANGED)
			{
				bank_json_dump_agency(iter_agency, 1, flags);
			}
// #endif
			iter_agency = iter_agency->next;
		}

		json_object_set_new_nocheck(root, "agencies", agency_array);

		json_dump_data_file(root, "", state->uuid_state, flags);

		json_decref(root);

		return EXIT_SUCCESS;


}


int bank_json_dump_bank(bank_t *bank, int option, size_t flags)
{
	if(bank==NULL || bank->status!=BANK_OBJECT_INIT)
	{
		fail("error bank_json_dump_bank(1)");
		return EXIT_FAILURE;
	}

	if(bank->changes==BANK_OBJECT_CONSTANT)
	{
		fail("bank_json_dump_bank(0) NO CHANGES WERE RECORDED, DUMPING IGNORED");
		return EXIT_SUCCESS;
	}

	json_t* state, *state_array=json_array();
	json_error_t error;
	state_t* iter_state;

	iter_state=bank->state;

	while(iter_state!=NULL)
	{
		state = json_pack_ex(&error, JSON_ALLOW_NUL, "{s:i, s:s, s:i, s:s}", "status", iter_state->status, "uuid", iter_state->uuid_state, "zipcode", iter_state->zip_code, "statecode", iter_state->state_code);

		if(!state)
		{
			fail("error bank_json_bank(2)");
			fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
			json_decref(state_array);
			return EXIT_FAILURE;
		}

		if(option==1 && iter_state->changes==BANK_OBJECT_CHANGED)
		{
			bank_json_dump_state(iter_state, 1, flags);
		}

		json_array_append_new(state_array, state);

		iter_state=iter_state->next;
	}

	json_dump_data_file(state_array, "", BASIC_DATA, flags);

	json_decref(state_array);

	return EXIT_SUCCESS;

}


int bank_json_dump_admin(login_t* admin, size_t flag)
{
	if(admin==NULL)
	{
		fail("error bank_json_dump_admin(1)");
		return EXIT_FAILURE;
	}

	json_t* admin_array, *adminj;
	json_error_t error;

	admin_array=json_array();

	login_t* iter_admin=admin;

	while(iter_admin!=NULL)
	{
		adminj = json_pack_ex(&error, JSON_ALLOW_NUL, "{s:i, s:s, s:s}", "status",iter_admin->status, "login_id", iter_admin->login_id, "login_key", iter_admin->login_key);

		if(!adminj)
		{
			fail("error bank_json_admin(2)");
			fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
			json_decref(admin_array);
			return EXIT_FAILURE;
		}

		json_array_append_new(admin_array, adminj);

		iter_admin=iter_admin->next;
	}

	json_dump_data_file(admin_array, "", ADMIN_DATA, flag);

	json_decref(admin_array);

	return EXIT_SUCCESS;
}





/*
int bank_json_dump_account_activity(char* activity_uuid, size_t flags)
{

	json_t* root=json_load_data_file(activity_uuid, flags);

	json_error_t error;

	strcat(activity_uuid, ".csv");

	if (!root)
	{
		fail(" error bank_json_dump_account_activity");
		fprintf(stderr, "Error type: %s %s, at line: %d, column: %d\n", error.source, error.text, error.line, error.column);
		return NULL;
	}


	FILE *fp = fopen(path, "w");
	json_t *op_array = json_object_get(operations_object, "operations");
	json_t *value;

	size_t index;
	char *date_operation, *libelle, *detail, *montant, *devise;

	fputs("Date de l'operation;Libelle;Detail de l'ecriture;Montant de l'operation;Devise", fp);

	json_array_foreach(op_array, index, value)
	{
		if (json_unpack(value, "{s:s, s:s, s:s, s:s, s:s !}", "date_operation", &date_operation, "libelle", &libelle, "detail", &detail, "montant", &montant, "devise", &devise))
		{
			json_decref(operations_object);
			fail("json_unpack object operations: Wrong type, format or incompatible JSON value encountered");
			return NULL;
		}

		fprintf(fp, "%s;%s;%s;%s;%s\n", date_operation, libelle, detail, montant, devise);
		json_decref(value);
	}

	fclose(fp);

	return path;
}
}
*/
