#include <stdio.h>
#include <stdlib.h>
#include "bankmanagement.h"

// #define EMPLOYEES_LOGIN 0x0122
// #define EMPLOYEE_RESET_LOGIN 0x222
#define ACCOUNTS_PRINTING 0X322
#define EMPLOYEES_LOGIN 0x422
#define EMPLOYEES_DATA 0x622
#define CLIENTS_LOGIN 0x0122
#define CLIENTS_DATA 0x522

int clear()
{
	while ( getchar() != '\n');

	return 1;
}


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

		return NULL;
	}
	if (bank_agency_account_add(account))
	{
		fprintf(stderr, "error adding account to agency failed: create_account\n");

		return account;
	}
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
		return NULL;
	}

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

	return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
	bank_t* data=bank();

	char ag_code[3], st_code[3];

	json_set_alloc_funcs(secure_malloc, secure_free);

	bank_json_parse_bank(data, 0, 0);


	char id[11], pass[11], *decoded;

	static state_t *state;
	static agency_t *agency;
	static employee_t *employee;
	static individual_t *individual;
	static account_t *account;
	static login_t *login;

	while(1)
	{


		printf("\n Enter q to quit or any other key to continue...");

		if (getchar() == 'q')
		{
			free(data);
			return 0;
		}

		while(1)
		{

			do
			{
				//system("clear");
				printf("--------------------------------------------------\n");
				printf("--------------------EMPLOYEES LOGIN PAGE------------------\n");
				printf("\nEnter user ID:");

			} while (!scanf("%s", id) && clear());

			do
			{

				printf("\nEnter password:");

			} while (!scanf("%s", pass) && clear());

			decoded = login_id_decoder(id);

			if (decoded == NULL)
			{
				//system("clear");
				printf("\nWRONG user ID or password\n");
				printf("Please try again!\n");
			}
			else
			{
				ag_code = atoi(decoded) / 100;
				st_code = atoi(decoded) % 100;

				printf("\nPlease wait...\n");

				state = bank_get_state_n(data, 1, 0,st_code);

				if (state != NULL)
				{
					printf("\nPlease wait...\n");

					bank_json_parse_state(state, 0, 0, JSON_ALLOW_NUL);

					printf("\nPlease wait...\n");

					agency = bank_state_get_agency_n(state, 1, ag_code);

					if (agency == NULL)
					{
						//system("clear");

						printf("\nWRONG user ID or password\n");

						printf("Please try again!\n");

						free_state(state);
					}
					else
					{
						printf("\nPlease wait...\n");

						bank_json_parse_agency(agency, EMPLOYEES_LOGIN, 0, JSON_ALLOW_NUL);

						if (bank_agency_get_logins(agency) != NULL)
						{
							login = bank_login();

							bank_login_set_key(login, pass);

							bank_login_set_id(login, id);

							login_t *results;

							printf("\nPlease wait...\n");

							if (!(results = bank_login_authenticate(bank_agency_get_logins(agency), login)))
							{
								//system("clear");
								printf("\nWRONG user ID or password\n");
								printf("Please try again!\n");

								free_login(login);
								free_agency(agency);
								free_state(state);
								free(decoded);
							}
							else
							{
								//system("clear");

								printf("---------------------------EMPLOYEE-----------------------\n");
								printf("Login Validated!\n");

								employee=bank_employee(BANK_EMPLOYEE_ADVISOR);

								bank_employee_set_login(employee, results);

								bank_employee_set_agency(employee, agency);

								bank_agency_employee_add(employee);

								bank_json_parse_agency(agency, EMPLOYEES_DATA, 1, JSON_ALLOW_NUL);
								bank_json_parse_agency(agency, CLIENTS_DATA, 1, JSON_ALLOW_NUL);

								printf("\nPlease wait...\n");

								bank_agency_free_logins(agency);

								printf("\nPlease wait...\n");

								break;
							}
						}
						else
						{
							fprintf(stderr, "error %s:%d", __FILE__, __LINE__);
							abort();
						}

					}
				}
				else
				{
					//system("clear");
					printf("\nWRONG user ID or password\n");
					printf("Please try again!\n");
					free(decoded);
				}
			}
		}
		unsigned int userchoice = 0;
		while (1)
		{
			clear();
			printf("\n---------------------------EMPLOYEE-----------------------\n");
			printf("\nEnter 1, 2, 3 or 4 to enter the correspondant menu or l to logout:\n\n");
			printf("1- Manage you personal informations and bank accounts\n");
			printf("2- Manage your personal bank accounts\n");
			printf("3- Clients and agency services\n");
			printf("4- Reset your password\n");

			do
			{
				userchoice = getchar();
			} while ((userchoice > '5' || userchoice < '1') && userchoice != 'l');

			switch (userchoice)
			{
				case '1':
				{
					bank_print_employee(employee);
					break;
				}
				case '2':
				{
					break;
				}
				case '3':
				{
					clear();
					printf("\n---------------------------EMPLOYEE-----------------------\n");
					printf("\nEnter 1, 2, 3 or 4 to enter the correspondant menu:\n\n");
					printf("1- Find a Client\n");
					printf("2- Create a new Client\n");
					printf("3- Modify a Client\n");
					printf("4- Create a bank account\n");
					printf("5- Find a bank account\n");
					printf("6- Money Deposit\n");
					printf("7- Money Transfer\n");
					printf("\nYou can always enter q to quit this menu\n");

					do
					{
						userchoice = getchar();
					} while ((userchoice > '5' || userchoice < '1') && userchoice != 'l');

					switch (userchoice)
					{
					case '1':
						char firstname[30], lastname[30], birthdate[DATE_SIZE];
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

						printf("\nBirthdate search? (y):");

						if (getchar() == 'y')
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
							bank_print_individual(individual);
							break;
						}
						else
						{
							printf("\n0 match");
							printf("\nEnter any key to exit this view...");

							getchar();
							break;
						}
					case '2':
						individual_t *new_individual = scan_individual(NULL, agency);
					case '3':
						modify_individual(individual, new_individual);
					case '4':
						int acctype, bktype;
						do
						{
							printf("\n\t--ACCOUNT TYPE--");
							printf("\n#1 for INDIVIDUAL");
							printf("\n#2 for SHARED");
							printf("\nPlease enter the account type:");

						} while ((acctype = getchar()) > '2' && acctype < '1');
						do
						{
							printf("\n\t--BANK ACCOUNT TYPE--");
							printf("#1 LIVRETA\n");
							printf("#2 LIVRETJEUNE\n");
							printf("#3 PEL\n");
							printf("#4 COURANT\n");
							printf("#5 LDD\n");
							printf("\nPlease enter the account type:");

						} while ((bktype = getchar()) > '5' && bktype < '1');
						account=create_account(agency, individual, acctype - 48, 195 + bktype);
					case '5':
					char iban[IBAN_SIZE];

						bank_json_parse_agency(agency, ACCOUNTS_PRINTING, 1, JSON_ALLOW_NUL);

						do
						{

							printf("\niban: ");
						} while (!scanf(" %37s", iban) && clear());

						bank_account_get_n(account, iban);
					case '6':
						double money;
						char currency;
						bank_account_money_depot(account, money, currency );
					case '7':
						double money;
						char currency;
						bank_money_transfer(account, bank_account_get_n(account, iban) ,money, currency);

					default:
						break;
					}
				}
				case '4':
				{

				}
				case '5':
				{

				}
				case 'q':

					free_login(login);
					free_individual(individual);
					free_agency(agency);
					free_state(state);
					free(bank);

					break;

				default:

					printf("Good Bye...\n");
					free_login(login);
					free_agency(agency);
					free_state(state);
					break;
			}

		}
		if (userchoice == 'l')
		{
			break;
		}

	}
	return 0;
}