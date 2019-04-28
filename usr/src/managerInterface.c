#include <stdio.h>
#include <stdlib.h>
#include "bankmanagement.h"

// #define EMPLOYEES_LOGIN 0x0122
// #define EMPLOYEE_RESET_LOGIN 0x222
#define ACCOUNTS_PRINTING 0X322
#define EMPLOYEE_LOGIN 0x422
#define EMPLOYEES_DATA 0x622


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

	int ag_code, st_code;

	json_set_alloc_funcs(secure_malloc, secure_free);

	bank_json_parse_bank(data, 0, 0);


	char id[11], pass[11], *decoded;

	static state_t *state;
	static agency_t *agency;
	static employee_t *employee;
	static individual_t *individual;
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

				state = bank_get_state_n(data, 1, st_code);

				if (state != NULL)
				{
					printf("\nPlease wait...\n");

					bank_json_parse_state(state, 0, 0, JSON_ALLOW_NUL);

					printf("\nPlease wait...\n");

					agency = bank_state_get_agency_n(state, 1, ag_code, NULL);

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

						bank_json_parse_agency(agency, EMPLOYEE_LOGIN, 0, JSON_ALLOW_NUL);

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
			printf("1- Manage you personal informations\n");
			printf("2- Manage your personal bank accounts\n");
			printf("3- Clients and agency services\n");
			printf("5- Reset your password\n");
			do
			{
				userchoice = getchar();
			} while ((userchoice > '5' || userchoice < '1') && userchoice != 'l');

			//system("clear");

			switch (userchoice)
			{
				case '1':

					printf("\naddress line 1: %s\n", employee->personal_data->address_no1);

					printf("\naddress line 2: %s\n", employee->personal_data->address_no2);

					printf("\nbirthdate: %.2s/%.2s/%.4s\n", employee->personal_data->birthdate, employee->personal_data->birthdate + 2, employee->personal_data->birthdate + 4);

					printf("\ncity: %s\n", employee->personal_data->city);

					printf("\nemail: %s\n", employee->personal_data->email);

					printf("\nfirstname: %s\n", employee->personal_data->firstname);

					printf("\nlastname: %s\n", employee->personal_data->lastname);

					printf("\nemployee since: %s\n", employee->personal_data->joineddate);

					printf("\nEnter any key to exit this view...");

					clear();

					break;
				case '2':
				//bank account type
				{
					switch (employee->personal_data->bank_account[0]->bank_account_type)
					{
						case BANK_ACCOUNT_LDD:
							{
								printf("\n\t---LDD---\n");
								break;
							}
						case BANK_ACCOUNT_LIVRETJEUNE:
							{
								printf("\n\t---Livretjeune---\n");
								break;
							}
						case BANK_ACCOUNT_LIVRETA:
							{
								printf("\n\t---LivretA---\n");
								break;
							}
						case BANK_ACCOUNT_COURANT:
							{
								printf("\n\t---Livretjeune---\n");
								break;
							}
						case BANK_ACCOUNT_PEL:
							{
								printf("\n\t---PEL---\n");
								break;
							}
						default:
						{
							break;
						}
					}

					printf("\niban: %s\n", employee->personal_data->bank_account[0]->iban);

					printf("\nbalance= %lf euros\n", employee->personal_data->bank_account[0]->account_balance);

					printf("\nbank account number: %s\n", employee->personal_data->bank_account[0]->account_no);

					//account type
					switch (employee->personal_data->bank_account[0]->account_type)
					{
					case BANK_ACCOUNT_INDIVIDUAL:
						printf("\naccount type: AN INDIVIDUAL BANK ACCOUNT\n");
						break;
					case BANK_ACCOUNT_SHARED:
						printf("\naccount type: A SHARED BANK ACCOUNT\n");
						break;
					default:
						break;
					}

					break;

			case '4':
				break;
			case '5':
				break;
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
	}


	return 0;
}