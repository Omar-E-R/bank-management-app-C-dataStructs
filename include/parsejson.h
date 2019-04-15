#ifndef PARSEJSON_H
#define PARSEJSON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>

#include"ville.h"
#include"client.h"



#define failhdr fprintf(stderr, "%s:%d: ", __FILE__, __LINE__)

#define fail(msg)                     \
	do                                \
	{                                 \
		failhdr;                      \
		fprintf(stderr, "%s\n", msg); \
		exit(EXIT_FAILURE);                      \
	} while (0)

#endif