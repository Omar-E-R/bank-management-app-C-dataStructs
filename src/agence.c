#include "agence.h"



lCompte init_liste_compte()
{
	lCompte liste_compte = (lCompte)calloc(1, sizeof(struct liste_compte));

	liste_compte->compte=NULL;
	liste_compte->next_compte=NULL;

	return liste_compte;

}

lClient init_liste_client()
{
	lClient liste_client = (lClient)calloc(1, sizeof(struct liste_client));

	liste_client->client=NULL;
	liste_client->next_client=NULL;

	return liste_client;

}

Conseiller init_conseiller()
{
	Conseiller conseiller=(Conseiller)calloc(1, sizeof(struct conseiller));
	int privilege_manager;
	conseiller->uuid_conseiller = (const char *)calloc(UUID_SIZE, sizeof(char));

	return conseiller;

}
Conseiller init_conseiller_arg(char* uuid_conseiller, int statut)
{
	Conseiller conseiller=(Conseiller)calloc(1, sizeof(struct conseiller));

	conseiller->uuid_conseiller = (const char *)calloc(UUID_SIZE, sizeof(char));

	strcpy(conseiller->uuid_conseiller, uuid_conseiller);

	conseiller->statut= statut;

	return conseiller;

}
lConseiller init_liste_conseiller()
{
	lConseiller liste_conseiller=(lConseiller)calloc(1, sizeof(struct liste_conseiller));


	liste_conseiller->next_conseiller=NULL;

	return liste_conseiller;

}

Agence init_agence()
{
	Agence agence=(Agence)calloc(1, sizeof(struct agence));

	agence->liste_client=init_liste_client();
	agence->liste_compte=init_liste_compte();
	agence->liste_conseiller=init_liste_conseiller();

	agence->uuid_agence = (const char*)calloc(UUID_SIZE, sizeof(char));
	agence->code_bic=(const char *) calloc(CODE_BIC_SIZE, sizeof(char));
	agence->indicatif_agence = (const char *)calloc(INDICATIF_AGENCE_SIZE, sizeof(char));
	agence->domiciliation_agence = (const char *)calloc(DOMICILIATION_SIZE, sizeof(char));

	return agence;

}

Agence init_agence_arg(agence_size_t allocation_size,char *uuid_agence, char *code_bic, int indicatif_agence, char *domiciliation_agence, char *hash_code)
{
	Agence agence=(Agence)calloc(1, sizeof(struct agence));


	agence->uuid_agence = (const char*)calloc(allocation_size.uuid_agence, sizeof(char));
	agence->code_bic=(const char *) calloc(allocation_size.code_bic, sizeof(char));
	agence->indicatif_agence = (const char *)calloc(allocation_size.indicatif_agence, sizeof(char));
	agence->domiciliation_agence = (const char *)calloc(allocation_size.domiciliation_agence, sizeof(char));

	strcpy(agence->uuid_agence, uuid_agence);
	strcpy(agence->code_bic, code_bic);
	strcpy(agence->domiciliation_agence, domiciliation_agence);
	strcpy(agence->hash_code, hash_code);
	sprintf(agence->indicatif_agence, "%05d", indicatif_agence);

	return agence;

}
Agence new_agence(const char* domiciliation, const char* uuid_ville)
{
	Agence agence=init_agence();

	uuid_gen(agence->uuid_agence);

	//agence->code_bic

	indicatif_gen(agence->indicatif_agence, uuid_ville);

	agence->domiciliation_agence = domiciliation;

	return agence;

}

Conseiller new_conseiller(Donnees_Personnelles info_conseiller)
{
	Conseiller conseiller = init_conseiller();

	uuid_gen(conseiller->uuid_conseiller);

	conseiller->info_conseiller = info_conseiller;

	return conseiller;
}


int isEqualConseiller(Conseiller employe1, Conseiller employe2)
{
	if(strcmp(employe1->uuid_conseiller, employe2->uuid_conseiller))
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

int isEqualAgence(Agence agence1, Agence agence2)
{
	if (strcmp(agence1->uuid_agence, agence2->uuid_agence) == 0 || strcmp(agence1->code_bic, agence2->code_bic) == 0 || strcmp(agence1->domiciliation_agence, agence2->domiciliation_agence) == 0 || strcmp(agence1->hash_code, agence2->hash_code)==0 || strcmp(agence1->indicatif_agence, agence2->indicatif_agence)==0)
		return EXIT_SUCCESS;
	return EXIT_FAILURE;
}

int addConseiller(lConseiller liste_conseiller, Conseiller conseiller)
{
	if(liste_conseiller==NULL || conseiller==NULL)
	{
		return EXIT_FAILURE;
	}
	if(liste_conseiller->next_conseiller==NULL && isEqualConseiller(liste_conseiller->conseiller,conseiller)!=EXIT_SUCCESS)
	{
		liste_conseiller->next_conseiller=init_liste_conseiller();
		liste_conseiller->next_conseiller->conseiller=conseiller;

		return EXIT_SUCCESS;

	}
	return addConseiller(liste_conseiller->next_conseiller, conseiller);

}

int addCompte(lCompte liste_compte, Compte compte)
{
	if (liste_compte == NULL || compte == NULL)
	{
		return EXIT_FAILURE;
	}
	if (liste_compte->compte == NULL && compte != NULL)
	{
		liste_compte->compte = compte;
		return EXIT_SUCCESS;
	}
	if (liste_compte->next_compte == NULL && isEqualCompte(liste_compte->compte, compte) != EXIT_SUCCESS)
	{
		liste_compte->next_compte = init_liste_compte();
		liste_compte->next_compte->compte = compte;

		return EXIT_SUCCESS;
	}
	return addcompte(liste_compte->next_compte, compte);
}

int addClient(lClient liste_client, Client client)
{
	lClient var = liste_client;

	while (var != NULL)
	{
		if (var->client != NULL)
		{
			if (isEqualClient((var->client), client) == EXIT_SUCCESS)
			{
				return EXIT_FAILURE;
			}
		}
		else
		{
			var->client = client;
			return EXIT_SUCCESS;
		}
		if (var->next_client == NULL)
		{
			var->next_client = init_client();
		}
		var = var->next_client;
	}
	return EXIT_SUCCESS;