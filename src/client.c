#include "client.h"

/* CREATES A NEW CLIENT STRUCT, ALLOCATES DYNAMIC MEMORY FOR IT AND IT INITIALIZE ITS PARAMETERS */ 
Client init_donnees_perso()
{
	Donnees_Personnelles info_client = (Donnees_Personnelles)calloc(1, sizeof(struct donnees_personnelles));

	info_client->nom = (const char *)calloc(NOM_SIZE, sizeof(char));

	info_client->prenom = (const char *)calloc(PRENOM_SIZE, sizeof(char));

	info_client->date_de_naissance = (const char *)calloc(DATE_DE_NAISSANCE_SIZE, sizeof(char));

	info_client->email = (const char *)calloc(EMAIL_SIZE, sizeof(char));

	info_client->adresse = (const char *)calloc(ADRESSE_SIZE, sizeof(char));

	info_client->code_postale = UNINTIALIZED;

	info_client->MouMme = UNINTIALIZED;

	info_client->nom_ville = (const char *)calloc(NOM_VILLE_SIZE, sizeof(char));

	info_client->complement_ad = (const char *)calloc(COMPLEMENT_AD_SIZE, sizeof(char));

	info_client->numero_mobile = (const char *)calloc(NUMERO_MOBILE_SIZE, sizeof(char));

	info_client->numero_fixe = (const char *)calloc(NUMERO_FIXE_SIZE, sizeof(char));

	return info_client;

}
Client init_client()
{
    Client newClient=(Client)calloc(1,sizeof(struct client));
    
    newClient->uuid_client = (const char *)calloc(UUID_SIZE, sizeof(char));

    newClient->statut=UNINTIALIZED;

    return newClient;

}

return_type isEqualClients(Client client1, Client client2)
{
	if(strcmp(client1->uuid_client, client2->uuid_client))
		return SUCCESS;
	else
		return FAILURE;


}

/* 
	CHECKS IF THE TWO CLIENTS HAVE THE SAME VALUE OF THE "ELEMENT" (PASSED AS ARGUMENT)

*/ 
type_donnees compareClients(Client client1, Client client2, type_donnees element)
{
    
    switch (element)
    {
        case NOM:
			if (strcmp(client1->donnees_perso->nom, client2->donnees_perso->nom))
				return SUCCESS;
			break;
        case PRENOM:
			if (strcmp(client1->donnees_perso->prenom, client2->donnees_perso->prenom))
				return SUCCESS;
			break;
        case DATE_DE_NAISSANCE:
			if (strcmp(client1->donnees_perso->date_de_naissance, client2->donnees_perso->date_de_naissance))
				return SUCCESS;
			break;
        case EMAIL:
			if (strcmp(client1->donnees_perso->email, client2->donnees_perso->email))
				return SUCCESS;
			break;
        case ADRESSE:
			if (strcmp(client1->donnees_perso->adresse, client2->donnees_perso->adresse))
				return SUCCESS;
			break;
        case CODE_POSTALE:
			if (strcmp(client1->donnees_perso->code_postale, client2->donnees_perso->code_postale))
				return SUCCESS;
			break;
        case NOM_VILLE:
			if (strcmp(client1->donnees_perso->nom_ville, client2->donnees_perso->nom_ville))
				return SUCCESS;
			break;
        case COMPLEMENT_AD:
			if (strcmp(client1->donnees_perso->complement_ad, client2->donnees_perso->complement_ad))
				return SUCCESS;
			break;
        case NUMERO_MOBILE:
			if (strcmp(client1->donnees_perso->numero_mobile, client2->donnees_perso->numero_mobile))
				return SUCCESS;
			break;
        case NUMERO_FIXE:
			if (strcmp(client1->donnees_perso->numero_fixe, client2->donnees_perso->numero_fixe))
				return SUCCESS;
			break;
        default:
			return ERROR;
            break;
    }

	return FAILURE;
    
}

/* CHECKS IF THE TWO CLIENTS HAVE ANY SAME VALUE AND RETURNS THE TYPES OF SIMILARITIES IN AN ARRAY OF "type_donnees" */
type_donnees* isEqualAllClients(Client client1, Client client2)
{
    /*
        "resultat[]" is an array of "type_donnees" that will contain in order the type of element in case of equalities or NULL_ in case of none.
        
    */
    type_donnees resultat[13];
    
    type_donnees element;

    int i=1, indicator=0, danger=0;

    //SEARCH WILL START WITH "NOM" and ENDS WITH "NUMERO_FIXE" BEFORE REACHING NULL_ ELEMENT
    for(element=NOM; element<NULL_; element++,i++)
    {
        if((resultat[i]=compareClients(client1, client2, element))!=NULL_)
        {
            indicator++;

            if (resultat[i] >= NOM && resultat[i] <= EMAIL)
            {
                if (resultat[i] == EMAIL)
                {
                    danger+=3;
                }else
                {
                    danger++;
                }
                
                
            }
        }

    }
    // sizeof(resultat)=13: 12 of elements each client have 
    // and the first element of the array is an indicator if there was many equalities or not i.e if (indicator != 0)
    //AN indicator bigger than 2 means there is a possible collusion between the two clients
    if (indicator < 3)
    {
        resultat[0]=NULL_;
    }else
    {
    //AN "danger" indicator bigger than 2 means there is a collusion between the two clients at least by "email"
        if(danger >= 3)
            resultat[0] = COLLUSION;
        else
        {
            resultat[0] = NOT_NULL_;
        }
    }

    return resultat;


}

/* CREATES A NEW CLIENT STRUCT USING THE "info_client" DATA*/
Client new_client(Donnees_Personnelles info_client)
{
	Client newClient=init_client();

	newClient->donnees_perso=info_client;

	uuid_gen(newClient->uuid_client);

}


Client_s new_clients(Client client1, Client client2)
{

	if(client1!=NULL & client2!=NULL)
	{
		Client_s clients;

		clients.clients[0]=client1;
		clients.clients[1]=client2;

		return clients;

	}else
	{
		Client_s client;

		if(client2==NULL)
		{
			client.client = client1;
		}else
		{
			client.client = client1;
		}
		
	}

}