#include"login.h"
#define ENCRYPTION_KEY 32534
#define USERNAME_SIZE 20
#define PASSWORD_SIZE 50


Client identifier;

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

Login init_login()
{
	Login login=(const char*)calloc(1,sizeof(struct login));
	login->login_id=(const char*)calloc(LOGIN_ID_SIZE,sizeof(char));
	login->login_key=(const char *) calloc(LOGIN_ID_SIZE, sizeof(char));

	return login;
}
Login init_login_arg(login_size_t alloc_size, char* id, char* key)
{
	Login login=(const char*)calloc(1,sizeof(struct login));

	login->login_id=(const char*)calloc(alloc_size.id,sizeof(char));
	login->login_key=(const char *) calloc(alloc_size.key, sizeof(char));

	strcpy(login->login_id, id);
	strcpy(login->login_key, key);

	return login;
}
Login init_login_admin()
{
	Login_Admin login_admin = (const char *)calloc(1, sizeof(struct login_admin));

	login_admin->login_id = (const char *)calloc(LOGIN_ID_SIZE, sizeof(char));
	login_admin->pin = (const char *)calloc(LOGIN_ID_SIZE, sizeof(char));
	login_admin->code_agence = (const char *)calloc(LOGIN_ID_SIZE, sizeof(char));

	return login_admin;
}
int encrypt_login_pass(Login user)
{

    unsigned char randbytes[16];//unpredicted bytes
    char salt[20];
    const char *const saltchars =
        "./0123456789ABCDEFGHIJKLMNOPQRST"
        "UVWXYZabcdefghijklmnopqrstuvwxyz";
    char *hash;
    int i;

    /* Retrieve 16 unpredictable bytes from the operating system using getentropy. */
    if (getentropy(randbytes, sizeof randbytes))
    {
        perror("getentropy");
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
    hash = crypt(user->login_key, salt);
    if (!hash || hash[0] == '*')
    {
        perror("crypt");
        return EXIT_FAILURE;
    }

    /* modify the login struct. */

    strcpy(user->login_key, hash);

    /* clearing the memory used to store the password */

    memset(&hash[0], 0, sizeof(hash));


    return EXIT_SUCCESS;
}
const char* decrypt_login_id(Login user){


    return 0;

}

int decrypt_login_pass(Login user)
{
    char login_key[64];
    char orig[9] = "eggplant";
    char buf[64];
    char txt[9];
    int i, j;

    for (i = 0; i < 64; i++)
    {
        login_key[i] = rand() & 1;
    }

    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            buf[i * 8 + j] = orig[i] >> j & 1;
        }
        setkey(login_key);
    }
    printf("Before encrypting: %s\n", orig);

    encrypt(buf, 0);
    for (i = 0; i < 8; i++)
    {
        for (j = 0, txt[i] = '\0'; j < 8; j++)
        {
            txt[i] |= buf[i * 8 + j] << j;
        }
        txt[8] = '\0';
    }
    printf("After encrypting:  %s\n", txt);

    encrypt(buf, 1);
    for (i = 0; i < 8; i++)
    {
        for (j = 0, txt[i] = '\0'; j < 8; j++)
        {
            txt[i] |= buf[i * 8 + j] << j;
        }
        txt[8] = '\0';
    }
    printf("After decrypting:  %s\n", txt);
    exit(EXIT_SUCCESS);
}