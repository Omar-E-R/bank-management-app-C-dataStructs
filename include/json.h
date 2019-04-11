#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <jansson.h>
#include "ville.h"
#include "agence.h"
#include "compte.h"
#include "client.h"


#define ERROR_MESSAGE "message"

enum errors{
	SUCCESS,
	FAILURE
};