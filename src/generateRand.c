
#include "generateRand.h"

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