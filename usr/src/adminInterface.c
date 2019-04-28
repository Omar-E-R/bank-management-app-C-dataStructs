#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <time.h>
#include <sys/time.h>
#include "bankmanagement.h"

#define CLIENTS_DATA 0x522
#define ADMINS_DATA 0x622
#define ADMIN_LOGIN 0x422
#define CLIENTS_LOGIN 0x0122
#define CLIENT_RESET_LOGIN 0x222
#define ACCOUNTS_PRINTING 0X322
#define EMPLOYEE_LOGIN 0x422
#define CLIENTS_DATA 0x522
#define EMPLOYEES_DATA 0x622

#define AGENCIES_PER_STATE 99
#define NUMBER_OF_STATES 99
#define MAX_RANDOM_SEARCH 9999






char *french_states[99] = {"Ain", "Aisne", "Allier", "Alpes-de-Haute-Provence", "Hautes-Alpes", "Alpes-Maritimes", "Ardèche", "Ardennes", "Ariège", "Aube", "Aude", "Aveyron", "Bouches-du-Rhône", "Calvados", "Cantal", "Charente", "Charente-Maritime", "Cher", "Corrèze", "Côte-d'or", "Côtes-d'armor", "Creuse", "Dordogne", "Doubs", "Drôme", "Eure", "Eure-et-Loir", "Finistère", "Corse-du-Sud", "Gard", "Haute-Garonne", "Gers", "Gironde", "Hérault", "Ille-et-Vilaine", "Indre", "Indre-et-Loire", "Isère", "Jura", "Landes", "Loir-et-Cher", "Loire", "Haute-Loire", "Loire-Atlantique", "Loiret", "Lot", "Lot-et-Garonne", "Lozère", "Maine-et-Loire", "Manche", "Marne", "Haute-Marne", "Mayenne", "Meurthe-et-Moselle", "Meuse", "Morbihan", "Moselle", "Nièvre", "Nord", "Oise", "Orne", "Pas-de-Calais", "Puy-de-Dôme", "Pyrénées-Atlantiques", "Hautes-Pyrénées", "Pyrénées-Orientales", "Bas-Rhin", "Haut-Rhin", "Rhône", "Haute-Saône", "Saône-et-Loire", "Sarthe", "Savoie", "Haute-Savoie", "Paris", "Seine-Maritime", "Seine-et-Marne", "Yvelines", "Deux-Sèvres", "Somme", "Tarn", "Tarn-et-Garonne", "Var", "Vaucluse", "Vendée", "Vienne", "Haute-Vienne", "Vosges", "Yonne", "Territoire de Belfort", "Essonne", "Hauts-de-Seine", "Seine-Saint-Denis", "Val-de-Marne", "Val-d'oise", "Guadeloupe", "Martinique", "Guyane", "La Réunion"};
// "Mayotte", "Polynésie Française", "Nouvelle Calédonie"};

int create_empty_agencies(state_t *state)
{
	agency_t *agency;

	int i, j, error = 0;

	int codes[AGENCIES_PER_STATE];

	for (i = 0; i < AGENCIES_PER_STATE; i++)
	{
		codes[i] = (rand()% AGENCIES_PER_STATE) + 1;

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

				codes[i] = (rand()% AGENCIES_PER_STATE) + 1;

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

	}

	return EXIT_SUCCESS;
}

bank_t *create_blank_bank_data()
{
	bank_t *data = bank();

	state_t *state;

	int i, j, error = 0;

	int codes[NUMBER_OF_STATES];

	for (i = 0; i < NUMBER_OF_STATES; i++)
	{
		codes[i] = (rand()% NUMBER_OF_STATES) + 1;
		error = 0;
		for (j = 0; j < i; j++)
		{
			if (codes[i] == codes[j])
			{
				error++;

				if (error > MAX_RANDOM_SEARCH)
				{
					fprintf(stderr, "error create_blank_bank_data(1)\n");
					return NULL;
				}

				codes[i] = (rand()% NUMBER_OF_STATES) + 1;

				j = 0;
			}
		}
	}

	for (i = 0; i < NUMBER_OF_STATES; i++)
	{
		state = bank_state();

		bank_state_set_uuid(state);

		bank_state_set_statecode(state, codes[i]);

		bank_state_set_zipcode(state, i + 1);

		if (!(create_empty_agencies(state) == EXIT_SUCCESS))
		{
			fprintf(stderr, "error create_blank_bank_data(2)\n");
			return NULL;
		}

		bank_add_state(data, state);

		bank_state_set_status_empty(state);
	}

	bank_set_status(data, BANK_OBJECT_INIT);

	return data;
}

agency_t *create_agency(state_t *state, char *address)
{
	agency_t *agency = bank_state_get_empty_agency(state);

	bank_agency_set_address(agency, address);

	bank_state_set_status_init(state);

	return agency;
}

employee_t *create_employee(individual_t *individual, employee_position position)
{
	if (individual == NULL )//|| bank_individual_is_status(individual, BANK_OBJECT_INIT))
	{
		fprintf(stderr, "error create_employe\n");
		return NULL;
	}
	if (bank_individual_get_login(individual) == NULL || bank_individual_get_agency(individual) == NULL)
	{

		fprintf(stderr, "error create_employee(1)\n");
		return NULL;
	}

	employee_t *employee = bank_employee(position);

	// bank_employee_set_uuid(employee);

	bank_employee_set_individual(employee, individual);

	bank_employee_set_agency(employee, bank_individual_get_agency(individual));

	if (bank_agency_employee_add(employee))
	{
		fprintf(stderr, "error adding employee to agency\n");
		bank_employee_remove_agency(employee);
	}

	bank_login_set_status(bank_employee_get_login(employee), BANK_LOGIN_EMP);

	return employee;
}






/* Decodes login id and finds the correspondant
	 state in bank_t then on success,
	 it find the correspondant agency
	 then parses agency json and searches
	 for login credentials and verify them,
	on success it parses individual_t json and return it*/
individual_t *login_client(bank_t *bank, login_t *login)
{
}
employee_t *login_employee(bank_t *bank, login_t *login)
{
}
admin_t *login_admin(bank_t *bank, login_t *login)
{
}
int individual_forgot_password(login_t *login, char *id_card_no, char *iban, char *email)
{
}
int individual_forgot_id(login_t *login, char *id_card_no, char *iban, char *email)
{
}

// int bank_money_transfer(account_t* account_sender, account_t* account_reciever, double transaction_amount, char currency)
// {

// }












int main(int argc, char *argv[])
{

	struct timeval time; //accurate time to the microseconds

	gettimeofday(&time, NULL);

	// microsecond has 1 000 000
	// Assuming we do not need quite that accuracy
	// Also we do not assume the system clock has that accuracy.

	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

	char id[11], pass[11];

	static admin_t* admin;
	static employee_t* employee;
	static bank_t *data;
	static state_t *state;
	static agency_t *agency;
	static individual_t *individual;
	static login_t *login;

	static int num = -1;

	json_set_alloc_funcs(secure_malloc, secure_free);

	login_t *admin_logs = bank_json_parse_admin();

	while(1)
	{


		printf("\n Enter q to quit or any other key to continue...");

		if (getchar() == 'q')
		{
			bank_free_logins(admin_logs);
			return 0;
		}

		while(1)
		{

			do
			{
				//system("clear");
				printf("--------------------------------------------------\n");
				printf("--------------------ADMINS LOGIN PAGE------------------\n");
				printf("\nEnter user ID:");

			} while (!scanf(" %10[0-9]", id) && clear());



			do
			{

				printf("\nEnter password:");

			} while (!scanf(" %10s", pass) && clear());


			if(login==NULL)
			{
				login=bank_login();
			}


			bank_login_set_key(login, pass);

			bank_login_set_id(login, id);

			memset(&(pass[0]), 0, strlen(pass));
			memset(&(id[0]), 0, strlen(id));

			login_t *results;

			if (!(results = bank_login_authenticate(admin_logs, login)))
			{
				//system("clear");
				printf("\nWRONG user ID or password\n");
				printf("Please try again!\n");

				free_login(login);
			}
			else
			{
				//system("clear");

				printf("---------------------------ADMIN-----------------------\n");

				printf("Login Validated!\n");

				admin = bank_admin();

				bank_admin_set_login(admin, results);

				break;
			}

		}

		unsigned int userchoice = 0;
		while (1)
		{
			clear();
			printf("\n---------------------------ADMIN-----------------------\n");
			printf("\nEnter 1, 2, 3 or 4 to enter the correspondant menu or l to logout:\n\n");
			printf("1- Check if there is already a database or Create empty data\n");
			printf("2- Create a new agency \n");
			printf("3- Create an employee for an agency\n");
			printf("4- Search tools menu\n");
			printf("5- Detailed view menu\n");
			printf("6- View all accounts sorted in types\n");

			do
			{
				userchoice = getchar();
			} while ((userchoice > '6' || userchoice < '1') && userchoice != 'l');

			//system("clear");

			switch (userchoice)
			{
			case '1':
				{
					char cmd[40];
					strcpy(cmd, "ls ./database | grep -c \"basic_data\"");

					FILE *fp = popen(cmd, "r");

					if (!fp)
					{
						fprintf(stderr, "error opening pipe: ADMIN INTERFACE");
						exit(EXIT_FAILURE);
					}

					fscanf(fp, "%d", &num);
					pclose(fp);
					// clear();

					if (num == 1)
					{
						printf("DATABASE already exists!\n");

						printf("\nTo exit this view enter any key...");

						getchar();

						break;
					}
					if (num > 1)
					{
						printf("DATABASE has duplicates... exiting!\n");
						exit(EXIT_FAILURE);
					}
					else
					{
						printf("no DATABASE were found, do you want to create it? (y):\n");
						int c=getchar();
						if (c != 'y')
						{
							printf("\n exiting this menu...");
						}

						data = create_blank_bank_data();

						bank_admin_set_bank(admin, data);

						printf("\n Empty DATABASE creation is done\n");

						printf("\n Dumping data...\n");

						bank_json_dump_bank(data, 1, JSON_ALLOW_NUL);

						printf("\n Dumping data is done\n");

						printf("\n Now you can open and create new agencies in menu #2\n");

						printf("\nTo exit this view enter any key...");

						getchar();

						break;
					}
					break;

				case '2':
					{
						if (num != 1 && bank_get_state(data) == NULL)
						{
							printf("\nYou must verify database first in menu #1 before going any further\n");

							printf("\nPlease try again...");

							printf("\nEnter any key to continue...");

							getchar();

							break;
						}

						if (num == 1 && bank_get_state(data) == NULL)
						{
							printf("\nPARSING DATA...");

							if(data==NULL)
							{
								data=bank();
							}

							bank_json_parse_bank(data, 1, 0);

							printf("\nPARSING DATA is done");

							num = 0;
						}

						if (bank_get_state(data) != NULL)
						{
							char zipcode[3], address[50];
							int zip;

							while (1)
							{
								do
								{
									printf("\nPlease enter the state's two digit zipcode where you want to create a new agency:");

								} while ((!scanf(" %2[0-9]", &zipcode) || (zip = atoi(&zipcode)) > 99 || zip < 0) && clear());

								printf("State choosen:%02d-%s", zip, french_states[zip-1]);

								state = bank_get_state_n(data, 0, (size_t)zip);

								agency = bank_state_get_empty_agency(state);

								do
								{
									printf("\nPlease enter the address for the new agency with no spaces or any other caracter except dashes '-' (50 caracters max) :");

								} while (!scanf("%50s", &address) && clear());

								if (bank_state_get_agency_n(state, 2, 0, address))
								{
									printf("An agency already exists at this address:%s or agency numbers has exceded its limit in this state:%d", address, zip);

									printf("\nPlease try again...");

									printf("\nEnter q to quit or any other key to continue...\n(Warning if you quit you changes won't be saved and you may experience a possible data loss):");

									int c=getchar();

									if (c == 'q')
									{
										printf("\nABORTING...");

										abort();
									}
								}
								else
								{
									break;
								}
							}

							bank_agency_set_address(agency, address);

							printf("\nAgency creation is done successfully!");
							printf("\nPlease write down these info or enter \"s\" to choose path were to save them:\n");

							printf("\nAgency state:%02d-%s.", zip, french_states[zip - 1]);
							printf("\nAgency id:%s", bank_agency_get(agency, BANK_AGENCY_ID));
							printf("\nAgency address:%s", bank_agency_get(agency, BANK_AGENCY_ADDRESS));

							printf("\nAgency ID will be used to find it and append data to it as employees etc...\n");
							clear();
							printf("\nTo save this info into a text file please enter:(s)");

							int c=getchar();

							if (c == 's')
							{
								bank_agency_export_info(agency);
							}
							/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							printf("\nWrite these changes? (y)");
							{
								if (getchar() == 'y')
								{
									bank_agency_changed(agency);

									bank_agency_set_status(agency, BANK_OBJECT_INIT);

									bank_state_set_status_init(state);

									bank_json_dump_bank(data, 1, 0);
								}
							}
							///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
							printf("\nDONE\nGoing to the main menu\n");
							break;
						}
						fprintf(stderr, "\nerror in switch in file%s: on line%d\n", __FILE__, __LINE__);
						break;
					}

					case '3':
						if (num != 1 && bank_get_state(data) == NULL)
						{
							printf("\nYou must verify database first in menu #1 before going any further\n");

							printf("\nPlease try again...");

							printf("\nEnter any key to continue...");

							getchar();

							break;
						}
						if (num == 1 && bank_get_state(data) == NULL)
						{
							printf("\nPARSING DATA...");

							if(data==NULL)
							{
								data=bank();
							}

							bank_json_parse_bank(data, 1, 0);

							printf("\nPARSING DATA is done");

							num = 0;
						}
						if (bank_state_get_agencies(bank_get_state(data)) == NULL)
						{
							printf("\nPARSING DATA...");

							if(data==NULL)
							{
								data=bank();
							}

							bank_json_parse_bank(data, 1, 0);

							printf("\nPARSING DATA is done");

							num = 0;
						}
						char agency_id[6], zipcode[3];
						int zip;

						while (1)
						{
							do
							{
								printf("\nPlease enter the state's two digit zipcode where the agency is located:");

							} while ((!scanf(" %2[0-9]", &zipcode) || (zip = atoi(&zipcode)) > 99 || zip < 0) && clear());

							state = bank_get_state_n(data, 0, zip);

							if (!bank_state_is_status(state, BANK_OBJECT_EMPTY))
							{
								printf("\nThis state doesnt have any agencies yet, please go back to menu #2 and create one for it or choose another state:");
								printf("\nEnter (r) to try another state or any other key to go back to main menu");

								if (getchar() != 'r')
								{
									zip = -1;
									break;
								}
							}
							break;
						}
						if (zip == -1)
							break;

						while (1)
						{
							do
							{
								printf("\nPlease enter the agency's 5 digit ID:");

							} while (!scanf(" %5[0-9]", &agency_id) && clear());

							agency = bank_state_get_agency_n(state, 0, atoi(agency_id), NULL);

							bank_json_parse_agency(agency, EMPLOYEE_LOGIN, 0, JSON_ALLOW_NUL);
							bank_json_parse_agency(agency, EMPLOYEES_DATA, 0, JSON_ALLOW_NUL);
							bank_json_parse_agency(agency, CLIENTS_DATA, 0, JSON_ALLOW_NUL);

							if (agency==NULL || bank_agency_is_status(agency, BANK_OBJECT_INIT))
							{
								printf("\nThis agency doesnt exist yet, please go back to menu #2 and create it or choose another ID:");
								printf("\nEnter (r) to try another agency ID or any other key to go back to main menu");

								if (getchar() != 'r')
								{
									zip = -1;
									break;
								}
							}
							break;
						}

						if (zip == -1)
							break;

						char retype[11];

						while(1)
						{
							do
							{
								printf("\n\tnote: password must be at least 8 caracters and at most 10");
								printf("\nPlease enter password for employees login:");

							} while (!scanf(" %10s", pass) && strlen(pass) < 8 && clear());

							do
							{
								printf("\nPlease retype password for verification:");

							} while (!scanf(" %10s", retype)&& clear());

							if (strcmp(pass, retype) == 0)
							{
								break;
								printf("\n Password is set!");
							}

							printf("\n The two passwords didn't match!");
							printf("\n Please try again:");
						}

						login=create_login(agency, pass );

						printf("\nPlease write down these info:");

						printf("\nEmployee login ID:%s\n", bank_login_get_id(login));

						printf("\n\t---FILLING EMPLOYEE INFO---\n");
						individual=scan_individual(login, agency);

						int position;

						clear();

						do
						{
							printf("\n\t--EMPLOYEE POSITION--");
							printf("\n#1 for MANAGER position");
							printf("\n#2 for ADVISOR position");
							printf("\n#2 for ASSISTANT position");
							printf("\nPlease enter the position of this employee:");

						} while ( (position=getchar()) > 3 && position < 1 );

						employee=create_employee(individual, position);

						clear();

						printf("\nTo save this info into a text file please enter:(s)");

						int c = getchar();

						if (c == 's')
						{
							bank_employee_export_info(employee);
						}
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						printf("\nWrite these changes? (y)");
						{
							if (getchar() == 'y')
							{
								bank_agency_changed(agency);

								bank_agency_set_status(agency, BANK_OBJECT_INIT);

								bank_state_set_status_init(state);

								bank_json_dump_bank(data, 1, 0);
								bank_json_dump_individual(individual, 0, JSON_ALLOW_NUL);
							}
						}
						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						printf("\nDONE\nEnter any key to go back to the main menu\n");
						getchar();
						break;
					}

			case '4':
				printf("\n\t---SEARCH TOOLS---\n");
				printf("\n1-Find a state\n");
				printf("\n2-Find an agency\n");
				printf("\n3-Find an employee\n");
				printf("\n4-Find a client\n");
				printf("\nYou can always enter q to quit this menu\n");

				do
				{
					userchoice = getchar();
				} while ((userchoice > '4' || userchoice < '1') && userchoice != 'q');
				switch (userchoice)
				{
				case '1':

					break;
				case '2':

					break;
				case '3':

					break;
				case '4':

					break;
				default:
					break;
				}
				break;


			case '5':
				break;
			case 'q':

				break;

			default:
				free_admin(admin);
				break;
			}
			if (userchoice == 'l')
			{
				clear();
				break;
			}
		}

	}
	return 0;
}