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
#define EMPLOYEES_LOGIN 0x422
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

	if (admin_logs==NULL)
	{
		fprintf(stderr, "\n\nERROR: NO DATA WERE FOUND, aborting...\n\n");
		abort();
	}

	unsigned int userchoice = 0;
	unsigned int choice = 0;
	unsigned int emptydata = 0;
	login_t *results;

	while(1)
	{


		printf("\n Enter q to quit or any other key to continue...");

		if (getchar() == 'q')
		{
			bank_free_logins(admin_logs);
			return 0;
		}
/* LOGIN */
		while(1)
		{

			do
			{

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


			if (!(results = bank_login_authenticate(admin_logs, login)))
			{

				printf("\nWRONG user ID or password\n");

				printf("Please try again!\n");

				free_login(login);
			}
			else
			{

				printf("---------------------------ADMIN-----------------------\n");

				printf("Login Validated!\n");

				if (userchoice == '6' && results != NULL)
				{
					char new_pass[11];

					while(1)
					{
						do
						{

							printf("\nEnter new password:");

						} while (!scanf(" %10s", pass) && clear());

						do
						{

							printf("\nRetype password:");

						} while (!scanf(" %10s", new_pass) && clear());

						if (strcmp(pass, new_pass) != 0)
						{
							printf("\nError: Password mismatch");
						}else
						{
							printf("\nPassword changed");
							break;
						}

					}

					bank_login_set_key(results, pass);

					encrypt_login_pass(results);

					bank_json_dump_admin(admin_logs, 0);

					memset(&(pass[0]), 0, strlen(pass));
					memset(&(new_pass[0]), 0, strlen(new_pass));
				}
				else
				{
					admin = bank_admin();
					bank_admin_set_bank(admin, bank());
					bank_admin_set_login(admin, results);
				}
				break;
			}

		}
/* CHECKING DATA INTEGRITY AND DATA PARSING */

		if(userchoice==0)
		{
			data = bank_admin_get_bank(admin);

			char cmd[60];

			strcpy(cmd, "ls ./database | grep -c \"basic_data\"");

			FILE *fp = popen(cmd, "r");

			if (!fp)
			{
				fprintf(stderr, "\n%d:%d:", __FILE__, __LINE__);
				fprintf(stderr, "error opening pipe: ADMIN INTERFACE");
				exit(EXIT_FAILURE);
			}

			fscanf(fp, "%d", &num);
			pclose(fp);

			if (num == 0)
			{
				emptydata = 1;
			}
			else
			{
				if (num == 1 && bank_json_parse_bank(data, 1, 0) == EXIT_FAILURE)
				{
					strcpy(cmd, "ls ./database -l | grep -v \"basic_data\" | grep -v \"admin\" | grep -c \"json\"");

					fp = popen(cmd, "r");

					if (!fp)
					{
						fprintf(stderr, "\n%s:%d:", __FILE__, __LINE__);
						fprintf(stderr, "error opening pipe: ADMIN INTERFACE");
						exit(EXIT_FAILURE);
					}

					fscanf(fp, "%d", &num);
					pclose(fp);

					if (num != 99)
					{
						emptydata = 2;
						free_admin(admin);
					}
					else
					{
						fprintf(stderr, "\n%s:%d:", __FILE__, __LINE__);
						printf("\n Uknown ERROR, aborting...\n\n");
						abort();
					}
				}
			}
		}
		/* MENU */



		while (1)
		{
			clear();
			printf("\n---------------------------ADMIN-----------------------\n");
			printf("\nEnter 1, 2, 3 or 4 to enter the correspondant menu or l to logout:\n\n");
			printf("1- Create an empty data\n");
			printf("2- Create a new agency \n");
			printf("3- Create an employee for an agency\n");
			printf("4- Search tools menu\n");
			printf("5- View all accounts sorted in types\n");
			printf("6- Reset Password\n");

			do
			{
				userchoice = getchar();
			} while ((userchoice > '6' || userchoice < '1') && userchoice != 'l');

			if (emptydata && userchoice != 'l' && userchoice != '1')
			{
				printf("\nMain Menu is locked:");

				printf("\nYou must create an empty database first");

				printf("\nEnter any key to continue...");

				getchar();

				userchoice='1';
			}

			switch (userchoice)
			{
				case '1':
				{

					if(emptydata==0)
					{
						printf("\nDatabase already exists.");
						printf("\nEnter any key to continue...");
						getchar();
					}
					else
					{
						clear();
						if(emptydata==1)
						{
							printf("\nNo DATABASE were found, do you want to create an empty database? (y):");

						}
						else
						{
							printf("\n Database is corrupted, Do you want to reset it and create a new one? (y):");
						}

						int c=getchar();

						if (c != 'y')
						{
							printf("\n Aborting this menu...");
							abort();
						}

						bank_admin_set_bank(admin, create_blank_bank_data());

						data=bank_admin_get_bank(admin);

						if(bank_json_dump_bank(data, 1, JSON_ALLOW_NUL))
						{
							fprintf(stderr, "\n%s:%d:", __FILE__, __LINE__);
							fprintf(stderr, "Error while dumping data\nAborting...\n\n");
							abort();
						}

						emptydata=0;

						printf("\nMain Menu is Unlocked!");

						printf("\nTo exit this view enter any key...");

						getchar();

						break;
					}

				}
				case '2':
				{

					char zipcode[3], address[50];
					int zip;

					while (1)
					{
						do
						{
							printf("\nPlease enter the state's two digit zipcode where you want to create a new agency:");

						} while ((!scanf(" %2[0-9]", &zipcode) || (zip = atoi(&zipcode)) > 99 || zip < 0) && clear());

						printf("State choosen: %02d -%s", zip, french_states[zip - 1]);

						state = bank_get_state_n(data, 0, (size_t)zip, NULL);

						agency = bank_state_get_empty_agency(state);

						do
						{
							printf("\nPlease enter the address for the new agency with no spaces or any other caracter except dashes '-' (50 caracters max) :");

						} while (!scanf("%50s", &address) && clear());

						if (bank_state_get_agency_n(state, 2, address))
						{
							printf("An agency already exists at this address:%s or agency numbers has exceded its limit in this state:%d", address, zip);

							printf("\nPlease try again...");

							printf("\tWarning if you quit changes will be lost");

							printf("\nEnter q to quit or any other key to continue...");

							int c = getchar();

							if (c == 'q')
							{
								fprintf(stderr, "\n%s:%d:", __FILE__, __LINE__);

								printf(": ABORTING...");

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

					printf("\n\nPlease write down these info or enter \"s\" to choose path were to save them:\n");

					clear();

					bank_print_agency_info(agency);

					printf("\nAgency ID will be used to find it and append data to it as employees etc...\n");

					printf("\nTo save this info into a text file please enter (s):");

					int c = getchar();

					if (c == 's')
					{
						bank_agency_export_info(agency);
					}
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					printf("\nWrite these changes? (y):");
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

					printf("\nDONE");

					printf("\nEnter any key to continue...");

					getchar();

					break;
				}
				case '3':
				{

					char agency_id[6], zipcode[3];
					int zip;

					do
					{
						printf("\nPlease enter the state's two digit zipcode where the agency is located:");

					} while ((!scanf(" %2[0-9]", &zipcode) || (zip = atoi(&zipcode)) > 99 || zip < 0) && clear());

					state = bank_get_state_n(data, 0, zip, NULL);

					if (bank_state_is_status(state, BANK_OBJECT_INIT))
					{
						printf("\nThis state doesnt have any agencies yet.");

						printf("\nTo create an agency in this state go to menu #2");

						printf("\nEnter any key to continue...");

						getchar();

						break;
					}

					do
					{
						printf("\nPlease enter the agency's 5 digit ID:");

					} while (!scanf(" %5[0-9]", &agency_id) && clear());

					agency = bank_state_get_agency_n(state, 0, agency_id);

					bank_json_parse_agency(agency, EMPLOYEES_LOGIN, 0, JSON_ALLOW_NUL);

					bank_json_parse_agency(agency, EMPLOYEES_DATA, 1, JSON_ALLOW_NUL);

					bank_json_parse_agency(agency, CLIENTS_DATA, 0, JSON_ALLOW_NUL);

					if (agency == NULL || bank_agency_is_status(agency, BANK_OBJECT_INIT))
					{
						printf("\nThis agency doesn't exist.");

						printf("\nTry again later!");

						printf("\nEnter any key to continue...");

						getchar();

						break;
					}

					char retype[11];

					while (1)
					{
						do
						{
							printf("\n\tnote: password must be at least 8 caracters and at most 10");
							printf("\nPlease enter password for employees login:");

						} while (!scanf(" %10s", pass) && strlen(pass) < 8 && clear());

						do
						{
							printf("\nPlease retype password for verification:");

						} while (!scanf(" %10s", retype) && clear());

						if (strcmp(pass, retype) == 0)
						{
							printf("\nPassword is set!");

							memset(&retype[0], 0, strlen(retype));

							break;
						}

						printf("\nError: Passwords mismatch.");

						printf("\nPlease try again:\n");
					}

					login = create_login(agency, pass);

					printf("\nPlease write down these info:");

					printf("\nEmployee login ID:%s\n", bank_login_get_id(login));

					printf("\nEnter any key to continue...");

					getchar();

					printf("\n\t---FILLING EMPLOYEE INFO---\n");

					individual = scan_individual(login, agency);

					clear();

					if(bank_individual_get_agency(individual)==NULL)
					{
						clear();

						printf("\nError: Indivdual conflict");

						individual_t* var_ind = bank_search_individual(agency, bank_individual_get(individual, BANK_INDIVIDUAL_FIRSTNAME), bank_individual_get(individual, BANK_INDIVIDUAL_LASTNAME), bank_individual_get(individual, BANK_INDIVIDUAL_BIRTHDATE));

						printf("\nThis identical employee were found:");

						bank_print_employee(bank_individual_get_employee(var_ind));

						printf("\nTo modify the existant employee  with the new data enetered enter (y):");

						if(getchar()=='y')
						{
							userchoice=0;

							modify_individual(var_ind, individual);

							individual=var_ind;
						}
						else
						{
							printf("\n\nEnter any key to continue...");

							break;
						}

					}

					int position;


					do
					{
						printf("\n\t--EMPLOYEE POSITION--");
						printf("\n#1 for MANAGER position");
						printf("\n#2 for ADVISOR position");
						printf("\n#3 for ASSISTANT position");
						printf("\nPlease enter the position of this employee:");

					} while ((position = getchar()) > '3' && position < '1');

					if(userchoice==0)
					{
						employee=bank_individual_get_employee(individual);
						bank_employee_set_postion(employee, position - 48);
					}
					else
					{
						employee = create_employee(individual, position - 48);

					}


					clear();

					bank_print_employee(employee);

					printf("\nTo save this info into a text file please enter (s):");

					int c = getchar();

					if (c == 's')
					{
						bank_employee_export_info(employee);
					}
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					printf("\nWrite these changes? (y):");

					if (getchar() == 'y')
					{
						bank_agency_changed(agency);

						bank_agency_set_status(agency, BANK_OBJECT_INIT);

						bank_state_set_status_init(state);

						bank_json_dump_bank(data, 1, 0);

						bank_json_dump_individual(individual, 0, JSON_ALLOW_NUL);
					}
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					printf("\nDONE");

					printf("\nEnter any key to continue...");

					getchar();

					break;
				}
				case '4':
				{
					while (1)
					{
						if(choice==0)
						{
							printf("\n\t---SEARCH TOOLS---\n");
							printf("\n1-Find a state\n");
							printf("\n2-Find an agency\n");
							printf("\n3-Find an employee\n");
							printf("\nYou can always enter q to quit this menu\n");

							do
							{
								userchoice = getchar();
							} while ((userchoice > '4' || userchoice < '1') && userchoice != 'q');
						}
						switch (userchoice)
						{
							case '1':
							{
								while (1)
								{
									printf("\n\t---STATE SEARCH---\n");
									printf("\n1-Find a state by name\n");
									printf("\n2-Find a state by zipcode\n");
									printf("\nYou can always enter q to quit this menu\n");

									do
									{
										userchoice = getchar();
									} while ((userchoice > '2' || userchoice < '1') && userchoice != 'q');

									switch (userchoice)
									{
									case '1':
									{
										char name[30];

										do
										{
											printf("\nnote:use dahes `-` instead of whitespaces");
											printf("\nType in the name:");
										} while (!scanf("%29s", name) && clear());

										state = bank_get_state_n(data, 3, 0, name);

										if (state != NULL)
										{
											clear();
											bank_print_state_info(state);
											userchoice='a';
										}
										else
										{
											clear();

											printf("\nNO State was found by this name please try searchging with zipcode instead");

											printf("\nEnter any key to exit this view...");

											clear();
										}
										break;
									}
									case '2':
									{
										int zipc;
										do
										{
											printf("\nType in the 2 digit zipcode:");
										} while (!scanf("%2d", &zipc) && clear());

										state = bank_get_state_n(data, 0, (size_t)(zipc), NULL);

										if (state != NULL )
										{
											clear();
											bank_print_state_info(state);
											userchoice='a';
										}
										else
										{
											printf("\nNO State was found by this zipcode please try searchging with zipcode instead");

											printf("\nEnter any key to exit this view...");

											clear();
										}
										break;
									}
									default:
										userchoice = 'q';
										break;
									}

									if (userchoice == 'q')
									{
										break;
									}

									if (userchoice == 'a')
									{
										if(bank_state_is_status(state, BANK_OBJECT_INIT))
										{
											printf("\nP.S: This state has no agencies yet, you can create agencies in main menu\n");

											printf("\nEnter any key to exit this view...");

											clear();

											break;
										}

										printf("\nDo you want to see this state's agencies? (y):");

										if (getchar() == 'y')
										{
											agency_t *var_ag=bank_state_get_agencies(state);

											clear();

											do
											{
												var_ag=bank_agency_get_next(var_ag, 1);

												if(var_ag==NULL)
												{
													break;
												}

												bank_print_agency_info(var_ag);

												printf("\n\nPress enter to view next agency or enter any other key to stop:");

												var_ag=bank_agency_get_next(var_ag, 0);


											} while (var_ag != NULL && getchar() == '\n');

											printf("\nEND: no more agencies");
											printf("\nEnter any key to exit this view...");

											clear();
										}
										break;
									}
								}
								break;
							}
							case '2':
							{

								while (1)
								{

									printf("\n\t---AGENCY SEARCH---\n");
									printf("\n1-Find a AGENCY by address\n");
									printf("\n2-Find a AGENCY by ID\n");

									printf("\nYou can always enter q to quit this menu\n");

									do
									{
										userchoice = getchar();
									} while ((userchoice > '4' || userchoice < '1') && userchoice != 'q');

									switch (userchoice)
									{
										case '1':
										{
											char addres[50];

											do
											{
												printf("\nnote:use dahes `-` instead of whitespaces");
												printf("\nType in the address:");
											} while (!scanf(" %49s", addres) && clear());

											state = bank_get_state(data);

											while (state != NULL && (state = bank_get_next_state(state, 1))!=NULL)
											{

												agency = bank_state_get_agency_n(state, 2, addres);

												if (agency != NULL && !bank_agency_is_status(agency, BANK_OBJECT_INIT))
												{
													userchoice = 'a';

													break;
												}

												state = bank_get_next_state(state, 0);

											}

											if(state==NULL)
											{
												userchoice = 'n';

											}

											break;
										}
										case '2':
										{
											char ag_id[6];
											do
											{
												printf("\nType in the 5 digit agency ID:");
											} while (!scanf(" %5[0-9]", ag_id) && clear());

											ag_id[5] = '\0';

											state = bank_get_state(data);

											while (state != NULL && (state = bank_get_next_state(state, 1)) != NULL)
											{

												agency = bank_state_get_agency_n(state, 0, ag_id);

												if (agency != NULL && !bank_agency_is_status(agency, BANK_OBJECT_INIT))
												{
													userchoice = 'a';

													break;
												}

												state = bank_get_next_state(state, 0);
											}

											if (state == NULL)
											{
												userchoice = 'n';
											}

											break;
										}
										default:
										{
											userchoice = 'q';
											break;
										}
									}

									if (userchoice == 'q')
									{
										choice=0;
										break;
									}
									if (userchoice == 'n')
									{
										clear();

										printf("\nNO Agency was found");

										printf("\nPlease try again");

										printf("\n\nEnter any key to exit this view...");

										getchar();
									}
									if (userchoice == 'a')
									{
										clear();

										printf("\nAgency was found!\n");

										bank_print_agency_info(agency);

										int c;

										if(choice==1)
										{
											printf("\nEnter (y) to choose this agency for employee search:");

											if((c=getchar())=='y')
											{
												choice=2;
												userchoice='3';
											}
											else
											{
												choice = 0;
												printf("\nEnter any key to exit this view...");
												clear();
												break;
											}

										}
										else
										{
											printf("\nTo view all of this agency's employees enter (y)");

											c = getchar();
										}

										if (c == 'y')
										{

											bank_json_parse_agency(agency, EMPLOYEES_LOGIN, 0, JSON_ALLOW_NUL);

											bank_json_parse_agency(agency, EMPLOYEES_DATA, 1, JSON_ALLOW_NUL);

											employee = bank_agency_get_employees(agency);

											if (employee == NULL)
											{
												clear();

												printf("\nSorry, this agency has no employees yet, you can create them in main menu");

												printf("\nEnter any key to exit this view...");

												clear();

												break;
											}
											if(choice==2)
											{
												break;
											}
											clear();
											do
											{
												bank_print_employee(employee);

												printf("\n\nPress enter to view next employee or any enter any other key to stop:");

												employee = bank_employee_get_next(employee);

											} while (employee != NULL && getchar() == '\n');

											printf("\nEND: no more employees");

											clear();

											printf("\n\nEnter any key to exit this view...");
										}
									}

									break;
								}
								break;
							}
							case '3':
							{
								if(choice!=2)
								{
									choice=0;

									clear();

									printf("\nDo you have a specific agency? (y):");

									if (getchar() == 'y')
									{
										choice = 1;
										userchoice = '2';
										break;
									}
									else
									{
										choice=-1;
									}
								}

								char fname_emp[41], lname_emp[41];

								do
								{
									printf("\nPlease enter firstname:");
								} while (!scanf(" %40s", fname_emp) && clear());

								do
								{
									printf("\nPlease enter lastname:");
								} while (!scanf(" %40s", lname_emp) && clear());

								if (choice == 1)
								{
									employee=bank_individual_get_employee(bank_search_individual(agency, fname_emp, lname_emp, NULL));
								}
								else
								{
									state=bank_get_state(data);


									int done=1;

									while (done && state != NULL && (state = bank_get_next_state(state, 1)) != NULL)
									{
										agency=bank_state_get_agencies(state);

										while (done && agency != NULL && (agency = bank_agency_get_next(agency, 1))!=NULL)
										{
											if(bank_agency_get_employees(agency)==NULL)
											{
												bank_json_parse_agency(agency, EMPLOYEES_LOGIN, 0, JSON_ALLOW_NUL);

												bank_json_parse_agency(agency, EMPLOYEES_DATA, 1, JSON_ALLOW_NUL);

											}

											employee = bank_individual_get_employee(bank_search_individual(agency, fname_emp, lname_emp, NULL));

											if(employee!=NULL)
											{
												done=0;
											}
											agency=bank_agency_get_next(agency, 0);
										}
										state = bank_get_next_state(state, 0);
									}
								}

								if(employee==NULL)
								{
									printf("\nSorry No employee was found by info you provided\n");

									printf("\nPlease try again later...");

									clear();

									printf("\nEnter any key to exit this view...");

									getchar();

									choice=0;

									break;

								}else
								{
									clear();

									bank_print_employee(employee);

									choice=0;

									break;
								}
							}
							default:
							{
								break;
							}
						}

						if (userchoice == 'q')
						{
							choice = 0;
							break;
						}
					}
				}
				case '5':
				{
					clear();
					printf("\n---ACCOUNTS VIEW---\n");
					printf("\nEnter 1, 2, 3, 4 or 5 to enter the correspondant menu:\n\n");
					printf("1- View bank accounts of type LIVRETA\n");
					printf("2- View bank accounts of type LIVRETJEUNE\n");
					printf("3- View bank accounts of type PEL\n");
					printf("4- View bank accounts of type COURANT\n");
					printf("5- View bank accounts of type LDD\n");
					printf("\nYou can always enter q to quit this menu\n");

					do
					{
						userchoice = getchar();
					} while ((userchoice > '5' || userchoice < '1') && userchoice != 'q');

					if(userchoice=='q')
					{
						break;
					}

					int count=1;

					state=bank_get_state(data);

					account_t* account;

					while (state != NULL && (state = bank_get_next_state(state, 1)) != NULL)
					{
						agency = bank_state_get_agencies(state);

						while (agency != NULL && (agency = bank_agency_get_next(agency, 1)) != NULL)
						{
							if (bank_agency_get_accounts(agency) == NULL)
							{
								bank_json_parse_agency(agency, ACCOUNTS_PRINTING, 1, JSON_ALLOW_NUL);

							}

							account = bank_agency_get_accounts(agency);

							if(account!=NULL)
							{
								count=bank_print_accounts(account, 195 + userchoice, count);
							}
							agency = bank_agency_get_next(agency, 0);
						}
						state = bank_get_next_state(state, 0);
					}

					if(count==1)
					{
						printf("\n0 accounts were found");

						printf("\nEnter any key to continue...");

						getchar();
					}
				}
				case '6':
				{
					break;
				}
				default:
				{

					break;
				}
			}
			if (userchoice == 'l' || userchoice == '6')
			{
				clear();
				break;
			}
		}

	}
	return 0;
}