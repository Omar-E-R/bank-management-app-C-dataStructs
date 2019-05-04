#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bankmanagement.h"

#define CLIENTS_LOGIN 0x0122
#define CLIENT_RESET_LOGIN 0x222

int main(int argc, char *argv[])
{
	bank_t *data = bank();

	char ag_code[3], st_code[3];
	char month[12], year[5], iban[IBAN_SIZE];
	double money;
	char currency;
	json_set_alloc_funcs(secure_malloc, secure_free);
	bank_json_parse_bank(data, 0, 0);

	char id[11], pass[11], *decoded;

	static state_t *state;
	static agency_t *agency;
	static individual_t *individual;
	static login_t *login;

	while (1)
	{

		printf("\n Enter q to quit or any other key to continue...");

		if (getchar() == 'q')
		{
			//free(data);
			return 0;
		}

		while (1)
		{

			do
			{
				//system("clear");
				printf("--------------------------------------------------\n");
				printf("--------------------CLIENTS LOGIN PAGE------------------\n");
				printf("\nEnter user ID:");

			} while (!scanf("%s", id) &&clear());

			do
			{
				printf("\nEnter password:");

			} while (!scanf("%s", pass) &&clear());

			decoded = login_id_decoder(id);

			if (decoded == NULL)
			{
				//system("clear");
				printf("\nWRONG user ID or password\n");
				printf("Please try again!\n");
			}
			else
			{
				strncpy(ag_code, decoded, 2);

				strncpy(st_code, decoded + 2, 2);

				ag_code[2] = '\0';
				st_code[2] = '\0';

				printf("\nPlease wait...\n");
				state = bank_get_state_n(data, 1,0, st_code);

				if (state != NULL)
				{
					printf("\nPlease wait...\n");

					bank_json_parse_state(state, 0, 0, 0);

					printf("\nPlease wait...\n");

					agency = bank_state_get_agency_n(state, 1, ag_code);

					if (agency == NULL)
					{
						//system("clear");
						printf("\nWRONG user ID or password\n");
						printf("Please try again!\n");

						//free_state(state);
					}
					else
					{
						printf("\nPlease wait...\n");

						bank_json_parse_agency(agency, CLIENTS_LOGIN, 0, 0);

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

								//free_login(login);
								//free_agency(agency);
								//free_state(state);
								//free(decoded);
							}
							else
							{
								//system("clear");

								printf("---------------------------CLIENT-----------------------\n");
								printf("Valid login!\n");

								individual = bank_individual(BANK_ACCOUNT_EMPTY);

								bank_individual_set_login(individual, results);

								bank_individual_set_agency(individual, agency);

								printf("\nPlease wait...\n");

								//bank_agency_free_logins(agency);

								bank_individual_set_status(individual, BANK_OBJECT_INIT);

								printf("\nPlease wait...\n");

								bank_json_parse_individual(individual, 1, JSON_ALLOW_NUL);
								break;
							}
						}
					}
				}
				else
				{
					//system("clear");
					printf("\nWRONG user ID or password\n");
					printf("Please try again!\n");
					//free(decoded);
				}
			}
		}
		unsigned int userchoice = 0;
		while (1)
		{
			clear();
			printf("\n---------------------------CLIENT-----------------------\n");
			printf("\nEnter 1, 2, 3 or 4 to enter the correspondant menu or l to logout:\n\n");
			printf("1- Edit your personal informations\n");
			printf("2- View you personal informations and bank accounts\n");
			printf("3- Export bank accounts activity in a CSV format\n");
			printf("4- Transfer money\n");
			printf("5- Reset your password\n");

			do
			{
				userchoice = getchar();
			} while ((userchoice > '5' || userchoice < '1') && userchoice != 'l');

			//system("clear");
			individual_t* new_individual;
			switch (userchoice)
			{
				case '1':
				{
					new_individual=scan_modify_individual();
					modify_individual(individual, new_individual);
					bank_json_dump_individual(individual, 1, 0);
					break;
				}
				case '2':
				{
					bank_print_individual(individual);
					break;
				}
				case '3':
				{

					do
					{
						printf("\nEnter which month's activity to export:\n1-Jan\n2-Feb\n3-Apr\n4-Mar\n5-May\n6-Jun\n7-Jul\n8-Aug\n9-Sep\n10-Oct\n11-Nov\n12-Dec\n");
						scanf("%d", &userchoice);
					} while ((userchoice > 12 || userchoice < 1));

					switch(userchoice)
					{
						case 1:
							strcpy(month, "Jan");
							break;
						case 2:
							strcpy(month, "Feb");
							break;
						case 3:
							strcpy(month, "Mar");
							break;
						case 4:
							strcpy(month, "Avr");
							break;
						case 5:
							strcpy(month, "May");
							break;
						case 6:
							strcpy(month, "Jun");
							break;
						case 7:
							strcpy(month, "Jul");
							break;
						case 8:
							strcpy(month, "Aug");
							break;
						case 9:
							strcpy(month, "Sep");
							break;
						case 10:
							strcpy(month, "Oct");
							break;
						case 11:
							strcpy(month, "Nov");
							break;
						case 12:
							strcpy(month, "Dec");
							break;
						default:
							break;
					}

					do
					{
						printf("\nEnter which month's activity to export:\n1-Jan\n2-Feb\n3-Apr\n4-Mar\n5-May\n6-Jun\n7-Jul\n8-Aug\n9-Sep\n10-Oct\n11-Nov\n12-Dec\n");
						scanf("%s", year);
					} while ((atoi(year) < 2019 ));

					int i = bank_individual_has_account(individual), j = '1';

					bank_print_individual(individual);

					if (i > 1)
					{
						printf("\nThis client has %d bank accounts", i);
						do
						{
							printf("\nPlease choose one of them by entering 1 for first, 2 for second etc..");

						} while ((j = getchar()) > i + 48 && j < '1');
					}

					bank_export_account_activity(bank_individual_get_account_n(individual, j-48), month, year);

					break;
				}
				case '4':
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
					int i = bank_individual_has_account(individual), j = '1';

					bank_print_individual(individual);

					if (i > 1)
					{
						printf("\nThis client has %d bank accounts", i);
						do
						{
							printf("\nPlease choose one of them by entering 1 for first, 2 for second etc..");

						} while ((j = getchar()) > i + 48 && j < '1');
					}

					bank_money_transfer(bank_individual_get_account_n(individual, j), iban, money, currency);

					break;
				}
				case '5':
					break;
				case 'q':

					//free_login(login);
					//free_individual(individual);
					//free_agency(agency);
					//free_state(state);
					//free(bank);

					break;

				default:

					printf("Good Bye...\n");
					//free_login(login);

					//free_agency(agency);
					//free_state(state);
					break;
			}
			if (userchoice == 'l')
			{
				break;
			}
		}
	}

	return 0;
}