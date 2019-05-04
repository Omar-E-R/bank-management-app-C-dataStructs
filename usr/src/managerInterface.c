#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "bankmanagement.h"

// #define EMPLOYEES_LOGIN 0x0122
// #define EMPLOYEE_RESET_LOGIN 0x222
#define ACCOUNTS_PRINTING 0X322
#define EMPLOYEES_LOGIN 0x422
#define EMPLOYEES_DATA 0x622
#define CLIENTS_LOGIN 0x0122
#define CLIENTS_DATA 0x522

char *french_states[99] = {"Ain", "Aisne", "Allier", "Alpes-de-Haute-Provence", "Hautes-Alpes", "Alpes-Maritimes", "Ardèche", "Ardennes", "Ariège", "Aube", "Aude", "Aveyron", "Bouches-du-Rhône", "Calvados", "Cantal", "Charente", "Charente-Maritime", "Cher", "Corrèze", "Côte-d'or", "Côtes-d'armor", "Creuse", "Dordogne", "Doubs", "Drôme", "Eure", "Eure-et-Loir", "Finistère", "Corse-du-Sud", "Gard", "Haute-Garonne", "Gers", "Gironde", "Hérault", "Ille-et-Vilaine", "Indre", "Indre-et-Loire", "Isère", "Jura", "Landes", "Loir-et-Cher", "Loire", "Haute-Loire", "Loire-Atlantique", "Loiret", "Lot", "Lot-et-Garonne", "Lozère", "Maine-et-Loire", "Manche", "Marne", "Haute-Marne", "Mayenne", "Meurthe-et-Moselle", "Meuse", "Morbihan", "Moselle", "Nièvre", "Nord", "Oise", "Orne", "Pas-de-Calais", "Puy-de-Dôme", "Pyrénées-Atlantiques", "Hautes-Pyrénées", "Pyrénées-Orientales", "Bas-Rhin", "Haut-Rhin", "Rhône", "Haute-Saône", "Saône-et-Loire", "Sarthe", "Savoie", "Haute-Savoie", "Paris", "Seine-Maritime", "Seine-et-Marne", "Yvelines", "Deux-Sèvres", "Somme", "Tarn", "Tarn-et-Garonne", "Var", "Vaucluse", "Vendée", "Vienne", "Haute-Vienne", "Vosges", "Yonne", "Territoire de Belfort", "Essonne", "Hauts-de-Seine", "Seine-Saint-Denis", "Val-de-Marne", "Val-d'oise", "Guadeloupe", "Martinique", "Guyane", "La Réunion"};

account_t *create_account(agency_t *agency, individual_t *individual, account_type account_type, bank_account_type bank_account_type)
{
	account_t *account = bank_account(bank_account_type, account_type);

	bank_account_set_uuid(account);

	bank_account_set_agency(account, agency);

	bank_account_set_iban(account);

	int res;

	if (account_type == BANK_ACCOUNT_SHARED)
	{
		res = bank_account_shared_set_holder(account, individual);
	}
	else
	{
		res = bank_account_set_holder(account, individual);
	}

	if (res)
	{
		fprintf(stderr, "error create_account\n");
		//free(account);
		return NULL;
	}
	if (bank_agency_account_add(account))
	{
		fprintf(stderr, "error adding account to agency failed: create_account\n");

		return account;
	}
	bank_account_changed(account);
	bank_individual_changed(individual);
	bank_agency_changed(bank_account_get_agency(account));

	return account;
}



account_t *create_account_shared(agency_t *agency, individual_t *individual1, individual_t *individual2, bank_account_type bank_account_type)
{
	account_t *account = create_account(agency, individual1, BANK_ACCOUNT_SHARED, bank_account_type);
	if (account == NULL)
	{
		fprintf(stderr, "error create_shared_account(0)");
		return NULL;
	}
	int res;

	res = bank_account_shared_set_holder(account, individual2);

	if (res)
	{
		fprintf(stderr, "error create_shared_account");
		//free(account);
		return NULL;
	}
	bank_account_changed(account);
	bank_individual_changed(individual1);
	bank_individual_changed(individual2);
	bank_agency_changed(bank_account_get_agency(account));

	return account;
}


int modify_account(account_t *account, status_type flag)
{
	if (account == NULL || bank_account_is_status(account, BANK_OBJECT_INIT))
	{
		fprintf(stderr, "error modify_account\n");
		return EXIT_FAILURE;
	}
	bank_account_set_status(account, flag);
	bank_account_changed(account);
	bank_individual_changed(bank_account_get_holder_n(account,0));
	bank_individual_changed(bank_account_get_holder_n(account,1));

	bank_agency_changed(bank_account_get_agency(account));

	return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
	struct timeval time; //accurate time to the microseconds

	gettimeofday(&time, NULL);

	// microsecond has 1 000 000
	// Assuming we do not need quite that accuracy
	// Also we do not assume the system clock has that accuracy.

	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

	char ag_code[3], st_code[3];


	int choice = 0;

	unsigned int userchoice = 0;

	static individual_t *new_individual;
	static individual_t *shared_individual;
	char id[11], pass[11], *decoded;

	static state_t *state;
	static agency_t *agency;
	static employee_t *employee;
	static individual_t *individual;
	static account_t *account;
	static login_t *login;
	static login_t *results;

	json_set_alloc_funcs(secure_malloc, secure_free);

	static bank_t* data;


	int acctype, bktype;

	char firstname[30], lastname[30], birthdate[DATE_SIZE];

	char iban[IBAN_SIZE];

	double money;

	char currency;

	data=bank();

	bank_json_parse_bank(data, 0, 0);

	if (data == NULL || bank_get_state(data)==NULL)
	{
		fprintf(stderr, "\n\nERROR: NO DATA WERE FOUND, aborting...\n\n");
		abort();
	}

	while(1)
	{
		printf("\n Enter q to quit or any other key to continue...");

		if (getchar() == 'q')
		{

			return 0;
		}

		while(1)
		{

			do
			{
				printf("----------------------------------------------------------\n");
				printf("--------------------EMPLOYEES LOGIN PAGE------------------\n");
				printf("\nEnter user ID:");

			} while (!scanf(" %10[0-9]", id) && clear());

			do
			{

				printf("\nEnter password:");

			} while (!scanf(" %10s", pass) && clear());

			decoded = login_id_decoder(id);

			if (decoded == NULL)
			{
				printf("\nWRONG user ID or password\n");
				printf("Please try again!\n");
			}
			else
			{
				strncpy(ag_code, decoded, 2);

				strncpy(st_code, decoded + 2, 2);

				ag_code[2]='\0';
				st_code[2]='\0';

				state = bank_get_state_n(data, 1, 0, st_code);

				if (state != NULL)
				{

					bank_json_parse_state(state, 0, 0, JSON_ALLOW_NUL);

					agency = bank_state_get_agency_n(state, 1, ag_code);

					if (agency == NULL)
					{
						printf("\nWRONG user ID or password\n");

						printf("Please try again!\n");
					}
					else
					{
						bank_json_parse_agency(agency, EMPLOYEES_LOGIN, 0, JSON_ALLOW_NUL);

						if (bank_agency_get_logins(agency) != NULL)
						{
							login = bank_login();

							bank_login_set_key(login, pass);

							bank_login_set_id(login, id);

							if (!(results = bank_login_authenticate(bank_agency_get_logins(agency), login)))
							{
								printf("\nWRONG user ID or password\n");

								printf("Please try again!\n");

								//free_login(login);

								//free_agency(agency);

								//free_state(state);

								//bank_agency_//free_logins(agency);

							}
							else
							{

								printf("---------------------------EMPLOYEE-----------------------\n");

								printf("\nLogin Validated!\n");

								if (userchoice == '4')
								{
									userchoice=0;
									char new_pass[11];
									char retype_new_pass[11];

									while (1)
									{
										do
										{

											printf("\nEnter new password:");

										} while (clear() && !scanf(" %10s", new_pass) && clear());

										do
										{

											printf("\nRetype password:");

										} while (clear() && !scanf(" %10s", retype_new_pass) && clear());

										if (strcmp(retype_new_pass, new_pass) != 0)
										{
											printf("\nError: Password mismatch");
										}
										else
										{
											if (strcmp(pass, new_pass) == 0)
											{
												printf("\nWarning Password must be different from old one");
												printf("\nPlease try again...\n");
											}
											else
											{
												printf("\nPassword changed");
												break;
											}
										}
									}
									bank_login_set_key(results, new_pass);

									encrypt_login_pass(results);

									bank_agency_changed(agency);

									memset(&(pass[0]), 0, strlen(pass));
									memset(&(retype_new_pass[0]), 0, strlen(retype_new_pass));
									memset(&(new_pass[0]), 0, strlen(new_pass));

								}
								bank_json_parse_agency(agency, EMPLOYEES_DATA, 1, JSON_ALLOW_NUL);

								employee = bank_agency_get_employee_n(agency, bank_login_get_uuid(results));

								bank_json_parse_agency(agency, CLIENTS_DATA, 0, JSON_ALLOW_NUL);

								bank_json_parse_agency(agency, ACCOUNTS_PRINTING, 0, 0);


								//bank_agency_free_logins(agency);

								break;
							}
						}
					}
				}

				printf("\nWRONG user ID or password\n");
				printf("Please try again!\n");
				////free(decoded);

			}
		}
		while (1)
		{
			if(choice==0)
			{
				clear();
				printf("\n---------------------------EMPLOYEE-----------------------\n");
				printf("\nEnter 1, 2, 3 or 4 to enter the correspondant menu or l to logout:\n\n");
				printf("1- View your personal informations and bank accounts\n");
				printf("2- Manage your personal info\n");
				printf("3- Clients and agency services\n");
				printf("4- Reset your password\n");

				do
				{
					userchoice = getchar();
				} while ((userchoice > '4' || userchoice < '1') && userchoice != 'l');
			}

			switch (userchoice)
			{
				case '1':
				{
					clear();
					bank_print_employee(employee);
					break;
				}
				case '2':
				{
					new_individual=scan_modify_individual();

					clear();

					bank_print_individual(individual);

					printf("Enter `y` to approve modifications above:");

					if(getchar()=='y')
					{
						modify_individual(bank_employee_get_individual(employee), new_individual);
					}

					break;
				}
				case '3':
				{

					while (1)
					{
						if(choice==0)
						{
							shared_individual=NULL;
							new_individual=NULL;
							individual=NULL;
							account=NULL;
							acctype=0;

							clear();
							printf("\n---------------------------EMPLOYEE-----------------------\n");
							printf("\nEnter 1, 2, 3 or 4 to enter the correspondant menu or s to save data:\n\n");
							printf("1- Find a Client(only that belongs to this agency)\n");
							printf("2- Create a new Client\n");
							printf("3- Modify a Client\n");
							printf("4- Create a bank account\n");
							printf("5- Find a bank account(only that belongs to this agency)\n");
							printf("6- Money Deposit\n");
							printf("7- Money Transfer\n");
							printf("8- Review changes made by clients and save them\n");
							printf("\nYou can always enter s to quit this menu and write data to disk\n");

							do
							{
								userchoice = getchar();
							} while ((userchoice > '8' || userchoice < '1') && userchoice != 's');
						}
						switch (userchoice)
						{
							case '1':
							{
								do
								{
									printf("\nnote: firstname and lastname must not exceed 30 caracters nor contain whitespaces,");
									printf("\n      use '-' for spaces instead");

									printf("\nFirstname: ");
								} while (!scanf(" %30s", firstname) && clear());

								do
								{

									printf("\nLastname: ");
								} while (!scanf(" %30s", lastname) && clear());

								int c=0;

								if(choice==0)
								{
									clear();
									printf("\nBirthdate search? (y):");

									c=getchar();
								}

								if (choice!=0 || c == 'y')
								{
									do
									{
										printf("\nnote: date must be entered in this format DDMMYYYY");
										printf("\nBirthdate: ");
									} while (!scanf(" %8[0-9]", birthdate) && clear());

									individual = bank_search_individual(agency, firstname, lastname, birthdate);
								}
								else
								{
									individual = bank_search_individual(agency, firstname, lastname, NULL);
								}

								if (individual != NULL)
								{
									bank_json_parse_individual(individual, 1, JSON_ALLOW_NUL);
									clear();
									bank_print_individual(individual);

									if (choice != 0)
									{
										userchoice = choice + 48;
										choice=1;
									}
									break;
								}
								else
								{
									if(choice!=0)
									{
										userchoice=choice+48;
										choice=-1;
									}
									printf("\n0 match");

									printf("\nEnter any key to exit this view...");

									getchar();

									break;
								}
							}
							case '2':
							{
								new_individual = scan_individual(NULL, agency);

								if (bank_individual_get_agency(new_individual) == NULL)
								{
									clear();
									if (choice == 0)
									{
										printf("\nError: Individual conflict");
									}
									else
									{
										userchoice= 48 + choice;
									}

									individual = bank_search_individual(agency, bank_individual_get(new_individual, BANK_INDIVIDUAL_FIRSTNAME), bank_individual_get(new_individual, BANK_INDIVIDUAL_LASTNAME), bank_individual_get(new_individual, BANK_INDIVIDUAL_BIRTHDATE));

									printf("\nThis identical client were found:");

									bank_json_parse_individual(individual, 1, JSON_ALLOW_NUL);
									clear();
									bank_print_individual(individual);

									clear();

									printf("\nTo modify the existant individual with the new data enetered enter (y):");

									if (getchar() == 'y')
									{
										modify_individual(individual, new_individual);

										new_individual = individual;

										bank_export_client_info(individual);

										bank_json_dump_individual(individual, 0, 0);

										printf("\nModification is done");

									}
									choice=0;
								}
								else
								{
									if (choice != 0)
									{
										bank_individual_remove_agency(new_individual);
										bank_agency_remove_individual(agency, new_individual);
										choice = -1;
										printf("\nFailure: This client were not found");
									}
									if (choice == 0)
									{
										bank_export_client_info(new_individual);

										bank_json_dump_individual(new_individual, 0, 0);

										printf("\nClient created and added successfully");
									}
								}

								printf("\n\nEnter any key to continue...");
								getchar();
								break;
							}
							case '3':
							{
								if(choice==-1)
								{
									choice=0;
									break;
								}
								if(choice==0)
								{
									choice=3;
									userchoice='2';
									break;
								}
							}
							case '4':
							{

								if(choice==-1)
								{
									choice=0;
									clear();
									printf("\nTo try again enter `y`:");

									if(getchar()!='y')
									{
										break;
									}
								}

								if(choice==0)
								{
									printf("\nNote: Make sure you created a client data before creating him an account");

									printf("\nFinding the client:");

									choice=4;
									userchoice='1';
									break;
								}

								if(acctype==0 && shared_individual==NULL)
								{
									clear();
									printf("\nFilling bank account:\n");
									do
									{
										printf("\n\t--ACCOUNT TYPE--");
										printf("\n#1 for SHARED");
										printf("\n#2 for INDIVIDUAL");
										printf("\nPlease enter the account type:");

									} while ((acctype = getchar()) > '2' && acctype < '1');
								}

								if(acctype=='1' && shared_individual==NULL)
								{
									shared_individual=individual;
									choice=4;
									userchoice='1';
									break;
								}

								do
								{
									clear();
									printf("\n\t--BANK ACCOUNT TYPE--\n");
									printf("#1 LIVRETA\n");
									printf("#2 LIVRETJEUNE\n");
									printf("#3 LDD\n");
									printf("#4 PEL\n");
									printf("#5 COURANT\n");
									printf("\nPlease enter the account type:");

								} while ((bktype = getchar()) > '5' && bktype < '1');

								if(acctype=='1')
								{
									account=create_account_shared(agency, individual, shared_individual, 244 + bktype - 48);
									if(account==NULL)
									{
										printf("Error: This client has already a maximum of shared accounts\n");
										break;
									}
									bank_export_bank_account_info(individual);
									bank_export_bank_account_info(shared_individual);
								}else
								{
									account = create_account(agency, individual, acctype - 48, 244 + bktype - 48);

									if (account == NULL || bank_account_get_agency(account)==NULL)
									{
										printf("\nError: This client has already a maximum of accounts permitted\n");
										printf("\nAccount was not added\n\n");
										break;
									}
									bank_export_bank_account_info(individual);
								}

								clear();

								bank_print_account(account);

								bank_json_dump_account(account, JSON_ALLOW_NUL);
								bank_json_dump_individual(individual,0, JSON_ALLOW_NUL);

								choice=0;

								break;
							}
							case '5':
							{

								printf("\nEnter iban to search for:");
								do
								{

									printf("\niban: ");
								} while (!scanf(" %37s", iban) && clear());

								account=bank_account_get_n(bank_agency_get_accounts(agency), iban);

								if(account!=NULL)
								{
									if(choice!=0)
									{
										userchoice=choice+48;
										choice=5;
									}
									bank_json_parse_account(account, 1, JSON_ALLOW_NUL);
									clear();
									bank_print_account(account);
									break;
								}
								else
								{
									if (choice != 0)
									{
										userchoice= 48 +choice;
										choice = -1;
									}
									printf("\nBank account was not found");
									printf("\n\nEnter any key to continue...");
									getchar();
									break;
								}
							}
							case '6':
							{
								if (choice == -1)
								{
									choice = 0;
									clear();
									printf("\nTo try again enter `y`:");

									if (getchar() != 'y')
									{
										break;
									}
								}

								if (choice == 0)
								{
									clear();
									printf("\nDo you want to search by name or iban?");
									printf("\nEnter `i` for iban (or anything else for name):");

									if (getchar() == 'i')
									{
										userchoice = '5';
									}
									else
									{
										userchoice = '1';
									}

									choice = 6;
									break;
								}

								if (choice == 1)
								{
									choice = 0;

									int i = bank_individual_has_account(individual), j = '1';

									if (i > 1)
									{
										printf("\nThis client has %d bank accounts", i);
										do
										{
											printf("\nPlease choose one of them by entering 1 for first, 2 for second etc..");

										} while ((j = getchar()) > i + 48 && j < '1');
									}
									if (i == 0)
									{
										printf("\nThis client has no bank accounts");
										printf("\nEnter any key to continue...");
										getchar();
										break;
									}

									account = bank_individual_get_account_n(individual, j-48);
								}
								clear();
								do
								{

									printf("\nEnter E for euros and $ for dollars");
									printf("\ncurrency:");
								} while ((currency = getchar()) != 'E' && currency != '$' && clear());

								do
								{

									printf("\nEnter amount");
									printf("\namount: ");
								} while (!scanf(" %lf", &money) && clear());

								if(!bank_account_is_shared(account))
								{
									bank_account_money_depot(bank_account_get_n(bank_agency_get_accounts(agency),bank_account_get(account,BANK_ACCOUNT_IBAN)), money, currency, 0);
								}
								bank_account_money_depot(account, money, currency, 1);

								bank_json_dump_account(account, JSON_ALLOW_NUL);

								printf("\nDONE");
								printf("\n\nEnter any key to exit this view...");

								getchar();

								choice = 0;
								break;
							}
							case '7':
							{
								do
								{

									printf("\nEnter reciever's bank account iban");
									printf("\niban: ");
								} while (!scanf(" %40s", iban) && clear());

								do
								{

									printf("\nEnter E for euros and D for dollars");
									printf("\ncurrency:");
								} while (clear() && !scanf(" %c", &currency) && clear());

								do
								{

									printf("\nEnter amount");
									printf("\namount: ");
								} while (!scanf(" %lf", &money) && clear());

								bank_money_transfer(account, iban, money, currency);

								break;
							}
							case '8':
							{

							}
							default:
							{
								bank_json_dump_agency(agency, 1, JSON_ALLOW_NUL);
								break;
							}
						}
						if(userchoice=='s')
						{
							break;
						}

					}

				}
				case '4':
				{
					break;
				}
				case '5':
				{
					break;
				}
				case 'q':

					//free_login(login);
					//free_individual(individual);
					//free_agency(agency);
					//free_state(state);
					//free(data);

					break;

				default:

					printf("Good Bye...\n");
					//free_login(login);
					//free_agency(agency);
					//free_state(state);
					break;
			}

			if (userchoice == 'l' || userchoice == '4')
			{
				if (agency != NULL)
				{
					bank_json_dump_agency(agency, 1, 0);
				}
				clear();
				break;
			}
		}

	}
	return 0;
}