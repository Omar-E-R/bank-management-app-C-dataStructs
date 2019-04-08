#include "agence.h"



lComptes init_liste_comptes()
{
	lComptes liste_comptes = (lComptes)calloc(1, sizeof(struct liste_compte));

	liste_comptes->next_compte=NULL;

	return liste_comptes; 

}

lClients init_liste_clients()
{
	lClients liste_clients = (lClients)calloc(1, sizeof(struct liste_clients));

	liste_clients->next_client=NULL;

	return liste_clients; 

}

Conseiller init_conseiller()
{
	Conseiller conseiller=(Conseiller)calloc(1, sizeof(struct conseiller));
	
	conseiller->uuid_conseiller = (const char *)calloc(UUID_SIZE, sizeof(char));

	return conseiller; 

}
lConseiller init_liste_conseiller()
{
	lConseiller liste_conseillers=(lConseiller)calloc(1, sizeof(struct liste_conseiller));
	

	liste_conseillers->next_conseiller=NULL;

	return liste_conseillers; 

}

Agence init_agence()
{
	Agence agence=(Agence)calloc(1, sizeof(struct agence));

	agence->liste_clients=init_liste_clients();
	agence->liste_comptes=init_liste_comptes();
	agence->liste_conseillers=init_liste_conseiller();

	agence->uuid_agence = (const char*)calloc(UUID_SIZE, sizeof(char));
	agence->code_bic=(const char *) calloc(CODE_BIC_SIZE, sizeof(char));
	agence->indicatif_agence = (const char *)calloc(INDICATIF_AGENCE_SIZE, sizeof(char));
	agence->domiciliation_agence = (const char *)calloc(DOMICILIATION_SIZE, sizeof(char));

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


return_type isEqualConseiller(Conseiller employe1, Conseiller employe2)
{
	if(strcmp(employe1->uuid_conseiller, employe2->uuid_conseiller))
		return MATCH;
	return NO_MATCH; 
}

return_type isEqualAgence(Agence agence1, Agence agence2)
{
	if(strcmp(agence1->uuid_agence, agence2->uuid_agence))
		return MATCH;
	return NO_MATCH; 
}

int addConseiller(lConseiller liste_conseiller, Conseiller conseiller)
{
	if(liste_conseiller==NULL || conseiller==NULL)
	{
		return FAILURE;
	}
	if(liste_conseiller->next_conseiller==NULL && isEqualConseiller(liste_conseiller->conseiller,conseiller)!=MATCH)
	{
		liste_conseiller->next_conseiller=conseiller;

		return SUCCESS;

	}
	return addConseiller(liste_conseiller->next_conseiller, conseiller);

}