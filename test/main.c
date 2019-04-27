#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <time.h>
#include <ctype.h>
#include "bank.h"
#include "jsonbank.h"
#define CLIENTS_DATA 0x522
#define EMPLOYEES_DATA 0x622
#define EMPLOYEE_LOGIN 0x422

#define AGENCIES_PER_STATE 99
#define NUMBER_OF_STATES 99
#define MAX_RANDOM_SEARCH 9999

char* french_states[99] = {"Ain", "Aisne", "Allier", "Alpes-de-Haute-Provence", "Hautes-Alpes", "Alpes-Maritimes", "Ardèche", "Ardennes", "Ariège", "Aube", "Aude", "Aveyron", "Bouches-du-Rhône", "Calvados", "Cantal", "Charente", "Charente-Maritime", "Cher", "Corrèze", "Côte-d'or", "Côtes-d'armor", "Creuse", "Dordogne", "Doubs", "Drôme", "Eure", "Eure-et-Loir", "Finistère", "Corse-du-Sud", "Gard", "Haute-Garonne", "Gers", "Gironde", "Hérault", "Ille-et-Vilaine", "Indre", "Indre-et-Loire", "Isère", "Jura", "Landes", "Loir-et-Cher", "Loire", "Haute-Loire", "Loire-Atlantique", "Loiret", "Lot", "Lot-et-Garonne", "Lozère", "Maine-et-Loire", "Manche", "Marne", "Haute-Marne", "Mayenne", "Meurthe-et-Moselle", "Meuse", "Morbihan", "Moselle", "Nièvre", "Nord", "Oise", "Orne", "Pas-de-Calais", "Puy-de-Dôme", "Pyrénées-Atlantiques", "Hautes-Pyrénées", "Pyrénées-Orientales", "Bas-Rhin", "Haut-Rhin", "Rhône", "Haute-Saône", "Saône-et-Loire", "Sarthe", "Savoie", "Haute-Savoie", "Paris", "Seine-Maritime", "Seine-et-Marne", "Yvelines", "Deux-Sèvres", "Somme", "Tarn", "Tarn-et-Garonne", "Var", "Vaucluse", "Vendée", "Vienne", "Haute-Vienne", "Vosges", "Yonne", "Territoire de Belfort", "Essonne", "Hauts-de-Seine", "Seine-Saint-Denis", "Val-de-Marne", "Val-d'oise", "Guadeloupe", "Martinique", "Guyane", "La Réunion"} ;
// "Mayotte", "Polynésie Française", "Nouvelle Calédonie"};

int create_empty_agencies(state_t *state)
{
	agency_t *agency;

	int i, j, error = 0;

	int codes[AGENCIES_PER_STATE];

	for (i = 0; i < AGENCIES_PER_STATE; i++)
	{
		codes[i] = (rand() % AGENCIES_PER_STATE) + 1;

		for (j = 0; j < i; j++)
		{
			if (codes[i] == codes[j])
			{
				error++;

				if (error > MAX_RANDOM_SEARCH)
				{
					fprintf(stderr, "error create_empty_agencies(1)\n");
					return EXIT_FAILURE;
				}

				codes[i] = (rand() % AGENCIES_PER_STATE) + 1;

				j = 0;
			}
		}
	}

	char *agencies_id[99];
	error = 0;
	for (i = 0; i < AGENCIES_PER_STATE; i++)
	{
		agencies_id[i] = agency_id_generator();

		for (j = 0; j < i; j++)
		{
			if (strcmp(agencies_id[i], agencies_id[j]) == 0)
			{
				error++;

				if (error > MAX_RANDOM_SEARCH)
				{
					fprintf(stderr, "error create_empty_agencies(2)\n");
					return EXIT_FAILURE;
				}

				agencies_id[i] = agency_id_generator();

				j = 0;
			}
		}
	}

	for (i = 0; i < AGENCIES_PER_STATE; i++)
	{
		agency = bank_agency();

		bank_agency_set_uuid(agency);

		bank_agency_set_code(agency, codes[i]);

		bank_agency_set_id(agency, agencies_id[i]);

		bank_state_add_agency(state, agency);

		//-----------
		printf("\t\t NO:%d\n", i);
		printf("\t\t\tuuid_agency:%s\n", agency->uuid_agency);
		printf("\t\t\tagencycode:\"%s\"\n", agency->agency_code);
		printf("\t\t\tagency-id:\"%s\"\n", agency->agency_id);
		//-----------
	}

	return EXIT_SUCCESS;
}

bank_t* create_blank_bank_data()
{
	bank_t* data=bank();

	state_t* state;

	int i,j, error=0;

	int codes[NUMBER_OF_STATES];

	for(i=0; i<NUMBER_OF_STATES; i++)
	{
		codes[i] = (rand() % NUMBER_OF_STATES) + 1;
		error=0;
		for (j = 0; j < i; j++)
		{
			if(codes[i]==codes[j])
			{
				error++;

				if(error>MAX_RANDOM_SEARCH)
				{
					fprintf(stderr, "error create_blank_bank_data(1)\n");
					return NULL;

				}

				codes[i] = (rand() % NUMBER_OF_STATES) + 1;

				j=0;
			}
		}
	}

	for(i=0; i<NUMBER_OF_STATES; i++)
	{
		state=bank_state();

		bank_state_set_uuid(state);

		bank_state_set_statecode(state, codes[i]);

		bank_state_set_zipcode(state, i+1);

		//-----------
		printf("%s\n", french_states[i]);
		printf("\tuuid_state:\"%s\"\n", state->uuid_state);
		printf("\tstatecode:\"%s\"\n", state->state_code);
		printf("\tzipcode:\"%d\"\n", state->zip_code);
		printf("\tagencies\n");
		//-----------

		if(!(create_empty_agencies(state)==EXIT_SUCCESS))
		{
			fprintf(stderr, "error create_blank_bank_data(2)\n");
			return NULL;
		}

		bank_add_state(data, state );
	}

	bank_set_status(data, BANK_OBJECT_INIT);

	return data;

}

agency_t* create_agency(state_t* state, char* address)
{
	agency_t* agency=bank_state_get_empty_agency(state);

	bank_agency_set_address(agency, address);

	bank_state_set_status_init(state);

	return agency;


}

int modify_individual(individual_t* individual, char* email, char* address_no1, char* city, int zipcode, char* address_no2, char* mobile_phone, char* home_phone, char* id_card_no)
{
	if(individual==NULL || bank_individual_is_status(individual, BANK_OBJECT_INIT))
	{
		fprintf(stderr, "error modify_individual\n");
		return EXIT_FAILURE;
	}

	if(email!=NULL)
	{
		bank_individual_set_email(individual, email);
	}
	if(address_no1!=NULL)
	{
		bank_individual_set_address_no1(individual, address_no1);
	}
	if(address_no2!=NULL)
	{
		bank_individual_set_address_no2(individual, address_no1);
	}
	if(city!=NULL)
	{
		bank_individual_set_city(individual, city);
	}
	if(mobile_phone!=NULL)
	{
		bank_individual_set_mobilephone(individual, mobile_phone);
	}
	if(home_phone!=NULL)
	{
		bank_individual_set_home_phone(individual, home_phone);
	}
	if(id_card_no!=NULL)
	{
		bank_individual_set_idcard(individual, id_card_no);
	}
	if(zipcode!=0)
	{
		bank_individual_set_zipcode(individual, zipcode);
	}
	return EXIT_SUCCESS;

}

login_t* create_login(agency_t* agency, char* pass)
{
	if(agency==NULL || bank_agency_is_status(agency, BANK_OBJECT_INIT))
	{
		fprintf(stderr, "error create_login\n");
		return NULL;
	}

	login_t* login=bank_login_generate(agency);

	bank_login_set_key(login, pass);

	encrypt_login_pass(login);

	memset(&(pass[0]), 0, strlen(pass));

	return login;


}

individual_t* create_individual(agency_t* agency, login_t* login, char sex, char* lastname, char* firstname, char* birthdate, char* email, char* address_no1, char* city, int zipcode, char* address_no2, char* mobile_phone, char* home_phone, char* id_card_no)
{
	individual_t* individual=bank_individual(0);
	bank_individual_set_uuid(individual);
	bank_individual_set_sex(individual, sex);
	bank_individual_set_firstname(individual, firstname);
	bank_individual_set_lastname(individual, lastname);
	bank_individual_set_birthdate(individual, birthdate);
	bank_individual_set_email(individual, email);
	bank_individual_set_address_no1(individual, address_no1);

	if(address_no2!=NULL)
		bank_individual_set_address_no2(individual, address_no2);

	bank_individual_set_city(individual, city);
	bank_individual_set_zipcode(individual, zipcode);
	bank_individual_set_mobilephone(individual, mobile_phone);

	if (home_phone != NULL)
		bank_individual_set_home_phone(individual, home_phone);

	bank_individual_set_idcard(individual, id_card_no);

	time_t timeoftheday;

	time(&timeoftheday);

	bank_individual_set_joindate(individual, ctime(&timeoftheday));


	if(agency==NULL)
	{
		return individual;
	}

	bank_individual_set_agency(individual, agency);

	if(login==NULL)
	{
		bank_individual_set_new_login(individual, birthdate);
	}else
	{
		if(bank_individual_set_login(individual, login))
		{
			fprintf(stderr, "error adding login to individual\n");
			return individual;
		}
	}


	if(bank_agency_individual_add(individual))
	{
		fprintf(stderr, "error adding individual to agency\n");
		bank_individual_remove_agency(individual);
	}

	return individual;
}


employee_t* create_employee(individual_t* individual, employee_position position)
{
	if(individual==NULL || bank_individual_is_status(individual, BANK_OBJECT_INIT))
	{
		fprintf(stderr, "error create_employe\n");
		return NULL;
	}
	if(bank_individual_get_login(individual)==NULL || bank_individual_get_agency(individual)==NULL)
	{

		fprintf(stderr, "error create_employee(1)\n");
		return NULL;
	}

	employee_t* employee=bank_employee(position);

	// bank_employee_set_uuid(employee);


	bank_employee_set_individual(employee, individual);

	bank_employee_set_agency(employee, bank_individual_get_agency(individual));

	if (bank_agency_employee_add(employee))
	{
		fprintf(stderr, "error adding employee to agency\n");
		bank_employee_remove_agency(employee);
	}

	bank_login_set_status(employee->personal_data->login, BANK_LOGIN_EMP);

	return employee;
}


account_t* create_account(agency_t* agency, individual_t* individual, account_type account_type, bank_account_type bank_account_type)
{
	account_t* account=bank_account(bank_account_type, account_type);

	bank_account_set_uuid(account);

	bank_account_set_agency(account, agency);

	bank_account_set_iban(account);

	int res;

	if(account_type==BANK_ACCOUNT_SHARED)
	{
		res = bank_account_shared_set_holder(account, individual);

	}else
	{
		res = bank_account_set_holder(account, individual);
	}

	if(res)
	{
		fprintf(stderr, "error create_account\n");

		return NULL;
	}
	if(bank_agency_account_add(account))
	{
		fprintf(stderr, "error adding account to agency failed: create_account\n");

		return account;
	}
	return account;
}


account_t* create_account_shared(agency_t* agency, individual_t* individual1, individual_t* individual2, bank_account_type bank_account_type)
{
	account_t* account=create_account(agency, individual1, BANK_ACCOUNT_SHARED, bank_account_type);
	if(account==NULL)
	{
		fprintf(stderr, "error create_shared_account(0)");
		return NULL;
	}
	int res;

	res=bank_account_shared_set_holder(account, individual2);

	if(res)
	{
		fprintf(stderr, "error create_shared_account");
		return NULL;
	}

	return account;

}


admin_t* create_admin(admin_t* admin_list, bank_t* bank, agency_t* agency, char* pass)
{
	admin_t* admin=bank_admin();

	// if(!bank_admin_set_uuid(admin))
	// {
	// 	fprintf(stderr, "error create_admin(0)");
	// 	free(admin);
	// 	return NULL;
	// }


	if(bank_admin_set_agency(admin, agency))
	{
		fprintf(stderr, "error create_admin(2)");
		free(admin);
		return NULL;
	}

	if(bank_admin_set_bank(admin, bank))
	{
		fprintf(stderr, "error create_admin(3)");
		free(admin);
		return NULL;
	}

	if (!bank_admin_set_new_login(admin, pass))
	{
		fprintf(stderr, "error create_admin(1)");
		free(admin);
		return NULL;
	}

	if(admin_list!=NULL)
	{
		if (bank_admin_add(admin_list, admin))
		{
			fprintf(stderr, "error create_admin(4)");
			free(admin);
			return NULL;
		}
	}

	return admin;
}

int modify_account(account_t* account, status_type flag)
{
	if(account==NULL || bank_account_is_status(account, BANK_OBJECT_INIT))
	{
		fprintf(stderr, "error modify_account\n");
		return EXIT_FAILURE;
	}
	bank_account_set_status(account, flag);

	return EXIT_SUCCESS;
}


/* Decodes login id and finds the correspondant
	 state in bank_t then on success,
	 it find the correspondant agency
	 then parses agency json and searches
	 for login credentials and verify them,
	on success it parses individual_t json and return it*/
individual_t* login_client(bank_t* bank, login_t* login)
{

}
employee_t* login_employee(bank_t* bank, login_t* login)
{

}
admin_t* login_admin(bank_t* bank, login_t* login)
{

}
int individual_forgot_password(login_t* login, char* id_card_no, char* iban, char* email)
{

}
int individual_forgot_id(login_t* login, char* id_card_no, char* iban, char* email)
{

}


// int bank_money_transfer(account_t* account_sender, account_t* account_reciever, double transaction_amount, char currency)
// {

// }

static void *guaranteed_memset(void *v, int c, size_t n)
{
	volatile char *p = v;
	while (n--)
		*p++ = c;
	return v;
}

static void *secure_malloc(size_t size)
{
	/* Store the memory area size in the beginning of the block */
	void *ptr = malloc(size + 8);
	*((size_t *)ptr) = size;
	return ptr + 8;
}

static void secure_free(void *ptr)
{
	size_t size;

	ptr -= 8;
	size = *((size_t *)ptr);

	guaranteed_memset(ptr, 0, size + 8);
	free(ptr);
}

int main(int argc, char *argv[])
{

	struct timeval time; //accurate time to the microseconds

	gettimeofday(&time, NULL);

	// microsecond has 1 000 000
	// Assuming we do not need quite that accuracy
	// Also we do not assume the system clock has that accuracy.

	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
	bank_t* data=bank();

	bank_json_parse_bank(data, 0,0);

	printf("%s\n", data->state->state_code);
	printf("%s\n", data->state->uuid_state);
	printf("%d\n", data->state->zip_code);

	state_t* state=bank_get_state_n(data, 0, 31);

	printf("%s\n", state->state_code);
	printf("%s\n", state->uuid_state);
	printf("%d\n", state->zip_code);


	bank_json_parse_state(state, 0, 0, 0);
	agency_t *agency = bank_state_get_agency_n(state, 0, 82066);
	bank_json_parse_agency(agency, CLIENTS_DATA, 0, 0);
	bank_json_parse_agency(agency, EMPLOYEE_LOGIN, 0, 0);
	bank_json_parse_agency(agency, EMPLOYEES_DATA, 0, 0);

	printf("AGENCE:%s\n", agency->agency_address);
	printf("ac:%s\n",agency->agency_code);
	printf("ua:%s\n", agency->uuid_agency);
	printf("id:%s\n", agency->agency_id);
	printf("client:%s\n", agency->individuals->uuid);
	// printf("client:%s\n", agency->individuals->lastname);
	// printf("client:%s\n", agency->individuals->firstname);

	free(data);
	return 0;





















	/*bank_t* bank=create_blank_bank_data();

	state_t* state=bank_get_state_n(bank, 31);

	agency_t* agency=create_agency(state, "TOULOUSE-BAYARD-(04036)");

	char *birthdate = malloc(11 * sizeof(char));
	strcpy(birthdate, "01091965");
	birthdate[10]='\0';
	char *birthdate1 = malloc(11 * sizeof(char));
	strcpy(birthdate1, "22061996");
	birthdate1[10]='\0';
	login_t* log=create_login(agency, birthdate1);
	individual_t* individual=create_individual(agency,NULL, 'M', "ELRIFAI", "ELIE", birthdate, "ELRIFAI.ELIE@HOTMAIL.FR", "51 RUE DE LA FAOURETTE", "TOULOUSE", 31000, "APT 12", "0695664044", NULL, "11111111111" );
	individual_t* individual1=create_individual(agency, log,'M', "ELRIFAI", "OMAR", "22061996", "ELRIFAI.OMAR@OUTLOOK.FR", "4 AVENUE EDOUARD BELIN", "TOULOUSE", 31400, "APT 2208", "0695664044", NULL, "33333333333" );
	employee_t* employee=create_employee(individual, BANK_EMPLOYEE_MANAGER);
	account_t* account=create_account_shared(agency, individual, individual1, BANK_ACCOUNT_LIVRETA);

	char *pass = malloc(11 * sizeof(char));
	strcpy(pass, "omar1234");
	pass[10]='\0';

	admin_t* admin=create_admin(NULL, bank, agency, pass);
	admin_t* admin1=create_admin(admin, bank, agency, pass);


	printf("Sex: %c\n", individual->sex);
	printf("Nom: %s\n", individual->lastname);
	printf("Prenom: %s\n", individual->firstname);
	printf("Date de naissance: %s\n", individual->birthdate);
	printf("Email: %s\n", individual->email);
	printf("Adresse: %s\n", individual->address_no1);
	printf("Complement d'adresse: %s\n", individual->address_no2);
	printf("Ville: %s\n", individual->city);
	printf("Code Postale: %d\n", individual->zip_code);
	printf("Date de creation: %s\n", individual->joineddate);
	printf("Num carte d'ID: %s\n", individual->id_card_no);
	printf("Numero tel mobile: %s\n", individual->mobile_phone);
	printf("agency code: %s\n", individual->agency->agency_code);
	printf("agency id: %s\n", individual->agency->agency_id);
	printf("Domiciliation: %s\n", individual->agency->agency_address);
	printf("statecode: %s\n", individual->agency->state->state_code);
	printf("state zipcode: %d\n", individual->agency->state->zip_code);
	printf("iban: %s\n", individual->bank_account[0]->iban);
	printf("numero compte: %s\n", individual->bank_account[0]->account_no);
	printf("identifiant: %s\n", individual->login->login_id);
	printf("decoded identifiant: %s\n", login_id_decoder(individual->login->login_id));
	printf("encrypted pass: %s\n", individual->login->login_key);
	printf("ADMIN ID: %s\n", admin->login->login_id);
	printf("ADMIN pass: %s\n", admin->login->login_key);
	login_t* login=bank_login();
	strcpy(login->login_id, individual->login->login_id);
	strcpy(login->login_key, "01091965");

	printf("if 0==%d the login is valid\n",bank_employee_authenticate_login(employee, login));
	char *newpass=malloc(11*sizeof(char));
	strcpy(newpass,"omar1234");
	newpass[8]='\0';
	strcpy(login->login_id, individual->login->login_id);
	strcpy(login->login_key, "01091965");
	bank_employee_reset_login(employee, login, newpass);
	printf("encrypted pass: %s\n", individual->login->login_key);
	printf("encrypted pass: %s\n", login->login_key);
	printf("newpass: %s\n",newpass);

	json_set_alloc_funcs(secure_malloc, secure_free);

	// bank_json_dump_individual(individual, 0, 0);
	modify_individual(individual, "elrifai.omar@icloud.com",NULL, NULL, 0, NULL, NULL, "066666666", "1234578");

	printf("new Email: %s\n", individual->email);
	printf("new HOME PHONE: %s\n", individual->home_phone);

	printf("new Num carte d'ID: %s\n", individual->id_card_no);

	// bank_json_dump_account(account, 0);


	// bank_json_dump_agency(agency, 0, 0);
	// bank_json_dump_state(state, 1, 0);

	bank_json_dump_bank(bank, 1, 0);
	bank_json_dump_admin(admin, 0);
	individual_t *ind = bank_search_individual(agency, "OMAR", "ELRIFAI", "22061996");
	if(ind!=NULL)
	{
		printf("NAME:%s\n", ind->firstname);
		printf("EMAIL:%s\n", ind->email);

	}
	return 0;
	*/
}