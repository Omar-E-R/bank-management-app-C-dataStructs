#include"login.h"


int _getrandom(void *buf, size_t buflen, unsigned int flags)
{
    return (int)syscall(SYS_getrandom, buf, buflen, flags);
}

int encrypt_login_pass(login_t *login)
{
    unsigned char randbytes[16]; //unpredicted bytes
    char salt[20];
    const char *const saltchars =
        "./0123456789ABCDEFGHIJKLMNOPQRST"
        "UVWXYZabcdefghijklmnopqrstuvwxyz";
    char *hash;
    int i;

    /* Retrieve 16 unpredictable bytes from the operating system using getentropy. */
    if (!_getrandom(randbytes, sizeof randbytes, 0))
    {
        perror("getrandom");
        return EXIT_FAILURE;
    }

    /* Use them to fill in the salt string. */
    salt[0] = '$';
    salt[1] = '6'; /* SHA-2-512 */
    salt[2] = '$';

    for (i = 0; i < 16; i++)
        salt[3 + i] = saltchars[randbytes[i] & 0x3f];
    salt[3 + i] = '\0';

    /* Read in the userâ€™s passphrase and hash it. */
    hash = crypt(login->login_key, salt);
    if (!hash || hash[0] == '*')
    {
        perror("crypt");
        return EXIT_FAILURE;
    }

    /* modify the login struct. */
    strcpy(login->login_key, hash);

    /* clearing the memory used to store the password */

    memset(&hash[0], 0, strlen(hash));

    login->status=BANK_LOGIN_ENCRYPTED;

    return EXIT_SUCCESS;
}

int validate_login(login_t *hashed_login, login_t *normal_login)
{
    if(strcmp(hashed_login->login_id, normal_login->login_id)==0)
	{
		if (strcmp(crypt(normal_login->login_key, hashed_login->login_key), hashed_login->login_key) == 0)
		{
			return EXIT_SUCCESS;
		}
	}
    return EXIT_FAILURE;
}


// login_t* init_login()
// {
//     login_t* login = (login_t*)calloc(1, sizeof(struct login));
//     login->login_id = (const char *)calloc(LOGIN_ID_SIZE, sizeof(char));
//     login->login_key = (const char *)calloc(LOGIN_KEY_SIZE, sizeof(char));

//     return login;
// }
// login_t* init_login_arg(login_size_t alloc_size, char *id, char *key)
// {
//     login_t* login = (login_t*)calloc(1, sizeof(struct login));

//     login->login_id = (const char *)calloc(alloc_size.id, sizeof(char));
//     login->login_key = (const char *)calloc(alloc_size.key, sizeof(char));

//     strcpy(login->login_id, id);
//     strcpy(login->login_key, key);

//     return login;
// }

// login_t* init_login_admin()
// {
//     login_t* login_admin = (login_t*)calloc(1, sizeof(struct login_admin));

//     login_admin->login_id = (const char *)calloc(LOGIN_ID_SIZE, sizeof(char));
//     login_admin->login_key = (const char *)calloc(LOGIN_KEY_SIZE, sizeof(char));
//     login_admin->code_agence = (const char *)calloc(CODE_AGENCE_SIZE, sizeof(char));

//     return login_admin;
// }

// login_t* create_admin_login(char* pass, const char* code_agence, const char* code_ville)
// {
//     login_t* login= init_login_admin();

//     int *code=malloc(4*sizeof(int));

//     int nb1=atoi(code_ville)/1000;
//     int nb2=atoi(code_agence)/1000;

//     code[0]=nb1/10;
//     code[1]=(nb1 % 10);
//     code[2]=nb2/10;
//     code[3]=nb2 % 10;

//     strcpy(login->code_agence, code_agence);

//     sprintf(login->login_id,"%.10s", login_id_generator(code));

//     sprintf(login->login_key,"%.10s", pass);

//     return login;


// }


/* TAKES TWO STRINGS EACH CONSISTENT OF 4 NUMBERS AND RETURN A USER LOGIN, INITIALIZED WITH 1234567890 PASSWORD */
// login_t* create_login(char* pass, const char* code_agence, const char* code_ville)
// {
//     login_t* login= init_admin();

//     int *code=malloc(4*sizeof(int));

//     int nb1=atoi(code_ville)/1000;
//     int nb2=atoi(code_agence)/1000;

//     code[0]=nb1/10;
//     code[1]=nb1 % 10;
//     code[2]=nb2/10;
//     code[3]=nb2 % 10;

//     sprintf(login->login_id,"%.10s", login_id_generator(code));

//     sprintf(login->login_key,"1234567890");

//     return login;


// }


// void printout_log_ad(login_t* login)
// {
//     printf(",%s,%s,%s,\n", login->code_agence, login->login_id, login->login_key);
// }

// void printout_log(login_t* login)
// {
//     printf("%s,%s,\n", login->login_id, login->login_key);
// }

// int hash_login_id_credentials(const char* code_agence, const char* code_ville)
// {

// }



// int validate_login_admin(login_t* hashed_login, login_t* normal_login)
// {
//     if(strcmp(crypt(normal_login->login_key, hashed_login->login_key), hashed_login->login_key)==0)
//     {
//         return EXIT_SUCCESS;
//     }
//     return EXIT_FAILURE;
// }




// int encrypt_code(const char *code)
// {
//     char key[64];

//     char buf[64];

//     size_t length = strlen(code);

//     char *txt = malloc((length + 1) * sizeof(char));

//     int i, j;

//     for (i = 0; i < 64; i++)
//     {
//         key[i] = rand() & 1;
//     }

//     for (i = 0; i < length; i++)
//     {
//         for (j = 0; j < length; j++)
//         {
//             buf[i * length + j] = code[i] >> j & 1;
//         }
//         setkey(key);
//     }
//     printf("Before encrypting: %s\n", code);

//     encrypt(buf, 0);
//     for (i = 0; i < length; i++)
//     {
//         for (j = 0, txt[i] = '\0'; j < length; j++)
//         {
//             txt[i] |= buf[i * length + j] << j;
//         }
//         txt[length] = '\0';
//     }
//     printf("After encrypting:  %s\n", txt);

//     strcpy(code, txt);

//     return EXIT_SUCCESS;
// }



// /////////////////////////
// static int decrypt_code(const char *hash_code)
// {
//     char key[64];

//     char buf[64];
//     size_t length = strlen(hash_code);
//     char txt[length];
//     int i, j;


//     encrypt(buf, 1);
//     for (i = 0; i < length; i++)
//     {
//         for (j = 0, txt[i] = '\0'; j < length; j++)
//         {
//             txt[i] |= buf[i * length + j] << j;
//         }
//         txt[length] = '\0';
//     }
//     printf("After decrypting:  %s\n", txt);
//     exit(EXIT_SUCCESS);
// }

/*
implementation des fonctions de identifications
qui utilisent la struct de ville pour decrypter le ID
ce qui permet de trouver le code de l'agence
et donc trouver apres le compte du client et valider si le login_key prive correspond

*/

// const char* scan_client_username()
// {
//     const char* username=calloc(USERNAME_SIZE, sizeof(char));

//     /*
//         CODE HERE
//     */

//     return username;

// }
// const char* scan_client_password()
// {
//     const char *password = malloc(PASSWORD_SIZE * sizeof(char));

//     /*
//         CODE HERE
//     */

//     return password;

// }

// int client_login()
// {
//     const char* username=scan_client_username();

//     const char* encrypted_pass= scan_client_password();



//     login->login_key=encrypted_pass;

//     login->login_id=username-ENCRYPTION_KEY;

//     identifier=decrypt_ville(login);
// }
