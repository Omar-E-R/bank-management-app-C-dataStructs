#include"random.h"

#define RAND_MAX_SEARCH 9999999
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

static size_t SIZE=6; //size of random generated numbers
static size_t SIZE_FULL=10;//size of login id
static size_t SIZE_RAND_DUP=2;//size of random generated int that can be equals

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

static int rand_tab_has_duplicates(int *rand_tab)
{
	int i = 0, j;
	/* FIRST 4 CARACTERS MUST NOT HAVE DUPLICATES*/
	for (i = 0; i < SIZE - SIZE_RAND_DUP; i++)
	{
		for (j = 0; j < SIZE - SIZE_RAND_DUP; j++)
		{
			if (i != j && rand_tab[i] == rand_tab[j])//IF THERE IS A DUPLICATE
			{
				return 1;
			}
		}
	}
	return 0;
}

/* GENERATES [SIZE] (6) INTEGERS THAT AT MOST HAS TWO OF THE SAME INTEGERS */
static int randd(int *rand_tab)
{
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

	return EXIT_SUCCESS;
}


static char* to_array(Arbre arbre)
{
	Arbre var=arbre;

	char* tab=malloc((SIZE_FULL + 1)*sizeof(char));

	int i;

	for (i = 0; i < SIZE_FULL; i++)
	{
		tab[i] = 48+ var->valeur;

		var = var->next_arbre;
	}
	tab[SIZE_FULL]='\0';
	return tab;
}

/* GENERATES 10 INTEGERS ID ENCODED WITH THE 4 INTEGERS CODE PROVIDED */
char* login_id_generator(int *code)
{
	SIZE=6;
	SIZE_FULL=10;
	SIZE_RAND_DUP=2;
	//MUST BE DONE IN MAIN
	// struct timeval time; //accurate time to the microseconds

	// gettimeofday(&time, NULL);

	// // microsecond has 1 000 000
	// // Assuming we do not need quite that accuracy
	// // Also we do not assume the system clock has that accuracy.

	// srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

	int i;

	int *rand_tab = calloc(6 , sizeof(int));

	if(randd(rand_tab))//FILLING THE ARRAY WITH RANDOM VALUES (6)
	{
		fputs("ERROR: generating 6 integeres values is impossible",stderr);
		return NULL;

	}

	Arbre arbre = (Arbre)calloc(1, sizeof(struct arbre)), count = arbre;

	/*INIT ARBRE CIRCULAIRE */

	for (i = 0; i < SIZE_FULL; i++)
	{
		count->valeur = -i;
		count->parcouru = false;

		if (i == SIZE_FULL - 1)
		{
			break;
		}

		count->next_arbre = (Arbre)calloc(1, sizeof(struct arbre));

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
		if (i >= SIZE-SIZE_RAND_DUP)
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

char* login_id_decoder(char* login_id)
{
	char* id_decoded=malloc(5*sizeof(char));
	id_decoded[4]='\0';
	SIZE=6;
	SIZE_FULL=10;
	SIZE_RAND_DUP=2;
	Arbre arbre = (Arbre)malloc(sizeof(struct arbre)), count = arbre;

	int i;
	/* TRANSFORMING ARRAY OF LOGIN ID TO CIRCULAR STACK */
	for (i = 0; i < SIZE_FULL; i++)
	{
		count->valeur = login_id[i] - 48; // -48 FOR THE CHAR TO INT
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
	for (i = 0; i < SIZE - SIZE_RAND_DUP; i++)
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
		/* REWRITING THE CODE RESULT(4 INTEGERS) TO THE STRING */
		id_decoded[i] = 48 + count->valeur; //48 FOR THE INT TO CHAR
		count->parcouru = false;
	}

	return id_decoded;
}

int uuid_gen(char* uuid)
{
	uuid_t binuuid;
	/*
     * Generate a UUID. We're not done yet, though,
     * for the UUID generated is in binary format
     * (hence the variable name). We must 'unparse'
     * binuuid to get a usable 36-character string.
     */
	uuid_generate_random(binuuid);

	/*
     * uuid_unparse() doesn't allocate memory for itself. 37 is the length of a UUID (36 characters), plus '\0'.
     */


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
	uuid[36]='\0';
	return EXIT_SUCCESS;
}

static char* arraytostring(int* array)
{
	int i;
	char *res=malloc(6*sizeof(char));
	for(i=0; i<5; i++)
	{
		res[i]=48+array[i];
	}
	res[5]='\0';
	return res;
}

char* agency_id_generator()
{
	SIZE=5;
	SIZE_RAND_DUP=3;
	int id[5];

	size_t i=0;

	if(randd(id))
	{
		fprintf(stderr, "Error generating agency id no: %d",(int)i);
		return NULL;
	}
	return arraytostring(id);
}

int global_account_number=10000;//init starts at 10000

char* iban_gen(const char *indicatif_agence, const char* code)
{
	char *iban=calloc(38, sizeof(char));
	if(global_account_number>10000000)
	{
		fprintf(stderr,"SEED FOR BANK ACCOUNTS NUMBERS EXCEEDED LIMIT\n");
		exit(EXIT_FAILURE);
	}

	int i, error=0;
	do
	{
	 	i=(rand() % 1000);
		error++;
		if(error>RAND_MAX_SEARCH)
		{
			fprintf(stderr, "ERROR iban_gen(1) \n");
			exit(EXIT_FAILURE);
		}
	} while (i==0);

	global_account_number+=i;

	char account_no[12];
	sprintf(account_no, "%010d", global_account_number);

	//A CAPITAL RANDOM LETTER
	char key= 65 + (rand () % 26);

	error=0;
	int rib_key;
	do
	{
	 	rib_key=(rand() % 100);
		error++;
		if(error>RAND_MAX_SEARCH)
		{
			fprintf(stderr, "ERROR iban_gen(2) \n");
			exit(EXIT_FAILURE);
		}
	} while (rib_key == 0);

	sprintf(iban, "FR%.2s%.5s%.5s%.10s%c%02d", code, BANK_NUMBER, indicatif_agence, account_no, key, rib_key);

	iban[37]='\0';

	return iban;

}