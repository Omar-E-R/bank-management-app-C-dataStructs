/* For malloc() */
#include <stdlib.h>
/* For puts()/printf() */
#include <stdio.h>
/* For uuid_generate() and uuid_unparse() */
#include <uuid/uuid.h>

/* Uncomment to always generate capital UUIDs. */
//#define capitaluuid true

/* Uncomment to always generate lower-case UUIDs. */
//#define lowercaseuuid true

/*
 * Don't uncomment either if you don't care (the case of the letters
 * in the 'unparsed' UUID will depend on your system's locale).
 */

const char* uuid_gen(const char* uuid);

const char *indicatif_gen(const char *indicatif, const char *uuid_ville); //5chiffres