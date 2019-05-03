
#include<stdio.h>
#include<string.h>
#include<sys/time.h>
#include "bankmanagement.h"

admin_t *create_admin(admin_t *admin_list, bank_t *bank, agency_t *agency, char *pass)
{
	admin_t *admin = bank_admin();

	if (bank_admin_set_agency(admin, agency))
	{
		fprintf(stderr, "error create_admin(2)");
		free(admin);
		return NULL;
	}

	if (bank_admin_set_bank(admin, bank))
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

	if (admin_list != NULL)
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

int main(int argc, char* argv[])
{
	struct timeval time; //accurate time to the microseconds

	gettimeofday(&time, NULL);

	// microsecond has 1 000 000
	// Assuming we do not need quite that accuracy
	// Also we do not assume the system clock has that accuracy.

	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

	char id[11], pass[11];

	static admin_t *admin;

	static login_t *login;

	json_set_alloc_funcs(secure_malloc, secure_free);

	login_t *admin_logs;

	static int userchoice, choice=0,num=-1;

	int admin_code[4]={1,9,9,6};

	while(1)
	{
		if(choice==0)
		{
			printf("\nEnter q to quit or any other key to continue...");

			if (getchar() == 'q')
			{
				bank_free_logins(admin_logs);
				return 0;
			}
			printf("\n---------------------------DEVELOPPER-----------------------\n");
			printf("\nEnter 1, 2, 3 or 4 to enter the correspondant menu or q to exit:\n\n");
			printf("1- Check if there is already an admin database\n");
			printf("2- Create a new admin \n");
			printf("3- Delete an admin\n");
			printf("4- View all admins\n");

			do
			{
				userchoice = getchar();
			} while ((userchoice > '4' || userchoice < '1') && userchoice != 'q');
		}

		switch (userchoice)
		{
			case '1':
				{
					char cmd[40];
					strcpy(cmd, "ls ./database | grep -c \"admin_data\"");

					FILE *fp = popen(cmd, "r");

					if (!fp)
					{
						fprintf(stderr, "error opening pipe: DEVELOPPER INTERFACE");
						exit(EXIT_FAILURE);
					}

					fscanf(fp, "%d", &num);
					pclose(fp);

					if (num == 0 || (num == 1 && (admin_logs = bank_json_parse_admin()) == NULL) || (num == 1 && bank_login_get_id(admin_logs) == NULL))
					{
						num = 0;


						if(choice==0)
						{
							printf("\n\n0 admins were found, you can create one in menu #2\n");
							printf("\n Enter any key to back to main menu...");

							getchar();
							break;
						}
						printf("\n\n0 admins were found, this one will be the first:\n");
						userchoice='2';

						break;
					}

					if (num > 1)
					{
						printf("DATABASE has duplicates... exiting!\n");

						exit(EXIT_FAILURE);
					}
					else
					{
						num=1;

						if(choice==0)
						{
							printf("\n\nAdmins were found\n");

							clear();

							printf("\nTo exit this view enter any key...");

							getchar();

							break;
						}
						userchoice='2';
						break;
					}
					break;
				}
			case '2':
				{
					choice=0;

					if (num == -1)
					{
						userchoice = '1';
						choice = 1;
						break;
					}

					login = bank_login();

					bank_login_set_id(login, login_id_generator(admin_code));

					char retype[11];

					while (1)
					{
						do
						{
							printf("\n\tnote: password must be at least 8 caracters and at most 10");
							printf("\nPlease enter password for the new admin login:");

						} while (!scanf(" %10s", pass) && strlen(pass) < 8 && clear());

						do
						{
							printf("\nPlease retype password for verification:");

						} while (!scanf(" %10s", retype) && clear());

						if (strcmp(pass, retype) == 0)
						{
							break;
							printf("\n Password is set!");
						}

						printf("\n The two passwords didn't match!");
						printf("\n Please try again:");
					}

					printf("\nAdmin ID: ", bank_login_get_id(login));

					clear();

					printf("\nTo save this info into a text file please enter:(s)");

					int c = getchar();

					if (c == 's')
					{
						char txtfile[29];

						printf("\nData are saved to admin_%s.txt", bank_login_get_id(login));

						sprintf(txtfile, "./root/admin_%s.txt", bank_login_get_id(login));

						txtfile[28] = '\0';

						FILE *fp = fopen(txtfile, "w");

						if (!fp)
						{
							perror("Text file creation");
							printf("\nfailed to save it into a text file");
							return EXIT_FAILURE;
						}
						fprintf(fp, "\nAdmin-ID: %10s.", bank_login_get_id(login));
						fprintf(fp, "\nAdmin-KEY:%10s", pass);
						fclose(fp);
					}

					bank_login_set_key(login, pass);

					encrypt_login_pass(login);

					if (num == 0)
					{
						bank_json_dump_admin(login, JSON_ALLOW_NUL);
					}
					else
						bank_json_dump_admin(bank_login_add(admin_logs, login), JSON_ALLOW_NUL);

					printf("\nDONE\n");

					clear();

					printf("\nTo exit this view enter any key...");

					getchar();

					break;
				}
			default:
				break;
		}
		if(userchoice=='q')
		{
			free_admin(admin);
			free_login(login);
			bank_free_logins(admin_logs);
			return 0;
		}
	}
	return 0;


}