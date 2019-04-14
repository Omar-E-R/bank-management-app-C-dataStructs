
#include "generateRand.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#define RAND_MAX_SEARCH 9999999999
#define SIZE 6 //size of random generated numbers
#define SIZE_FULL 10//size of login id
#define SIZE_RAND 2//size of random generated int that can be equals

enum boolean
{
	true,
	false
};

typedef struct arbre *Arbre;

struct arbre
{
	int valeur;
	enum boolean parcouru;
	Arbre next_arbre;
};

int rand_tab_has_duplicates(int *rand_tab)
{
	int i = 0, j;
	/* FIRST 4 CARACTERS MUST NOT HAVE DUPLICATES*/
	for (i = 0; i < SIZE - SIZE_RAND; i++)
	{
		for (j = 0; j < SIZE - SIZE_RAND; j++)
		{
			if (i != j && rand_tab[i] == rand_tab[j])//IF THERE IS A DUPLICATE
			{
				return 1;
			}
		}
	}
	return 0;
}

void zeros_tab(int *rand_tab)
{
	int i;
	for (i = 0; i < SIZE; i++)
	{
		rand_tab[i] = 0;
	}
}
/* GENERATES [SIZE] (6) INTEGERS THAT AT MOST HAS TWO OF THE SAME INTEGERS */
int randd(int *rand_tab)
{
	zeros_tab(rand_tab);
	unsigned long int errors=0;
	int i;

	do
	{
		for (i = 0; i < SIZE; i++)
		{
			rand_tab[i] = (rand() % 10);
		}
		errors++;
	} while (rand_tab_has_duplicates(rand_tab) && errors < RAND_MAX_SEARCH);

	if(errors >= RAND_MAX_SEARCH)
		return EXIT_FAILURE;
	
	return 0;
}

unsigned long mix(unsigned long a, unsigned long b, unsigned long c)
{
	a = a - b;
	a = a - c;
	a = a ^ (c >> 13);
	b = b - c;
	b = b - a;
	b = b ^ (a << 8);
	c = c - a;
	c = c - b;
	c = c ^ (b >> 13);
	a = a - b;
	a = a - c;
	a = a ^ (c >> 12);
	b = b - c;
	b = b - a;
	b = b ^ (a << 16);
	c = c - a;
	c = c - b;
	c = c ^ (b >> 5);
	a = a - b;
	a = a - c;
	a = a ^ (c >> 3);
	b = b - c;
	b = b - a;
	b = b ^ (a << 10);
	c = c - a;
	c = c - b;
	c = c ^ (b >> 15);
	return c;
}

int* to_array(Arbre arbre)
{
	Arbre var=arbre;

	int* tab=malloc(SIZE_FULL*sizeof(int));

	int i;

	for (i = 0; i < SIZE_FULL; i++)
	{
		tab[i] = var->valeur;
		
		var = var->next_arbre;
	}
	return tab;
}

/* GENERATES 10 INTEGERS ID ENCODED WITH THE 4 INTEGERS CODE PROVIDED */
int* login_id_generator(int *code)
{
	struct timeval time; //accurate time to the microseconds

	gettimeofday(&time, NULL);

	// microsecond has 1 000 000
	// Assuming we do not need quite that accuracy
	// Also we do not assume the system clock has that accuracy.

	srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

	int i;

	int *rand_tab = malloc(6 * sizeof(int));

	if(randd(rand_tab))//FILLING THE ARRAY WITH RANDOM VALUES (6)
	{
		fputs("ERROR: generating 6 integeres values is impossible",stderr);
		return NULL;

	}

	Arbre arbre = (Arbre)malloc(sizeof(struct arbre)), count = arbre;

	/*INIT ARBRE CIRCULAIRE */

	for (i = 0; i < SIZE_FULL; i++)
	{
		count->valeur = -i;
		count->parcouru = false;

		if (i == SIZE_FULL - 1)
		{
			break;
		}
		if (count->next_arbre == NULL)
		{
			count->next_arbre = (Arbre)malloc(sizeof(struct arbre));
		}
		count = count->next_arbre;
	}

	// making it circular by pointing last element of stack to the its head
	count->next_arbre = arbre;

	count = arbre; //pointing the pointer to the head of stack

	/*---------we have now circular arbre that can contains [SIZE] numbers -------- */

	int j;

	for (i = 0; i < SIZE - 1; i++)
	{

		if (count->parcouru == true)
		{
			while (count->parcouru == true)
			{
				count = count->next_arbre;
			}
		}

		/*  Adding the random int value rand_tab[i] to the arbre */
		count->valeur = rand_tab[i];
		count->parcouru = true;

		count = arbre; //pointing back to the head
		
		for (j = 0; j < rand_tab[i]; j++)//going to rand_tab[i] index
		{
			count = count->next_arbre;
		}
		if (count->parcouru == true)
		{
			while (count->parcouru == true)
			{
				count = count->next_arbre;
			}
		}
		/*  Adding the VALUE code to the arbre */
		if (i >= SIZE-SIZE_RAND)
		{
			count->valeur = rand_tab[i + 1];
			count->parcouru = true;
		}
		else
		{
			count->valeur = code[i]; //ADDING INTEGERS FROM int code[] to the ID
			count->parcouru = true;
		}
	}
	
	return to_array(arbre);
}

int login_id_decoding(int* login_id)
{
	Arbre arbre = (Arbre)malloc(sizeof(struct arbre)), count = arbre;

	int i;
	/* TRANSFORMING ARRAY OF LOGIN ID TO CIRCULAR STACK */
	for (i = 0; i < SIZE_FULL; i++)
	{
		count->valeur = login_id[i];
		count->parcouru = true;//OPPOSIT OF WHAT I DID IN THE ABOVE FUNCTION

		if (i == SIZE_FULL - 1)
			break;
		
		count->next_arbre = (Arbre)malloc(sizeof(struct arbre));

		count = count->next_arbre;
	}

	count->next_arbre = arbre; //MAKING IT CIRCULAR

	count = arbre; //pointing to the head

	int valeur;
	int j;
	for (i = 0; i < SIZE - SIZE_RAND; i++)
	{

		if (count->parcouru != true)
		{
			while (count->parcouru != true)
			{
				count = count->next_arbre;
			}
		}

		/*  Adding the random int value rand[i] to the arbre */

		valeur = count->valeur;
		count->parcouru = false;

		count = arbre; //pointing to the head
		
		for (j = 0; j < valeur; j++)
		{
			count = count->next_arbre;
		}
		if (count->parcouru != true)
		{
			while (count->parcouru != true)
			{
				count = count->next_arbre;
			}
		}
		/* REWRITING THE CODE RESULT(4 INTEGERS) TO THE ARRAY LOGIN_ID */
		login_id[i] = count->valeur;
		count->parcouru = false;
	}

	return 0;
}

const char* uuid_gen(const char* uuid)
{
	uuid_t binuuid;
	/*
     * Generate a UUID. We're not done yet, though,
     * for the UUID generated is in binary format 
     * (hence the variable name). We must 'unparse' 
     * binuuid to get a usable 36-character string.
     */
	uuid_generate_random(binuuid);

	// /*
    //  * uuid_unparse() doesn't allocate memory for itself, so do that with
    //  * malloc(). 37 is the length of a UUID (36 characters), plus '\0'.
    //  */
	// const char *uuid = (char*)calloc(37, sizeof(char));

#ifdef capitaluuid
	/* Produces a UUID string at uuid consisting of capital letters. */
	uuid_unparse_upper(binuuid, uuid);
#elif lowercaseuuid
	/* Produces a UUID string at uuid consisting of lower-case letters. */
	uuid_unparse_lower(binuuid, uuid);
#else
	/*
     * Produces a UUID string at uuid consisting of letters
     * whose case depends on the system's locale.
     */
	uuid_unparse(binuuid, uuid);
#endif

	// Equivalent of printf("%s\n", uuid); - just my personal preference
	//puts(uuid);

	return uuid;
}