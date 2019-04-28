
#include <stdio.h>
#include <stdlib.h>
int clear()
{
	char c;
	while (((c = getchar()) != '\n'))
		; //&& (c != EOF) && (c != '\0'))
	return 1;
}
int main()
{
	char id[11], pass[10], at, rest[10];
	do
	{
		//system("clear");
		printf("--------------------------------------------------\n");
		printf("--------------------ADMINS LOGIN PAGE------------------\n");
		printf("\nEnter user ID:");

	} while (!scanf(" %10[0-9]", id) && clear());

	clear();

	do
	{
		do
		{

			printf("\nEnter email:");

		} while (!scanf("%15[a-zA-z0-9.]", pass) && clear());

	} while (!scanf("%[@]%15[a-zA-z0-9.]",&at, rest) && clear());

	printf("Pass:\"%s\"",pass);
	printf("\"%c%s\"",at,rest);
	printf("id:\"%s\"",id);
	return 0;
}