
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
	char id[11], pass[11];
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

		printf("\nEnter password:");

	} while (!scanf(" %10s", pass));


	printf("Pass:\"%s\"",pass);
	printf("id:\"%s\"",id);
	return 0;
}