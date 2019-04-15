#include "compte.h"
union client_s
{
	Client client;

	Client clients[2];

};
struct rib
{
	const char* iban;
	const char* code_bic;
	const char* numero_compte;
	const char* indicatif_agence;
	const char* domiciliation_agence;

};
struct compte
{
	const char* uuid_compte;

	compte_t statut; //compte supprime ? i.e actif ou non actif
	compte_t nature_compte;
	compte_t type_compte;//LIVRET...

	double solde;

	const char *operations;//A FILENAME OR MAYBE I WILL CHANGE IT TO A FILE POINTER

	Rib rib;

	Compte next_compte;

	Client_s titulaire;
};



/* A client can and only have one shared (joint) bank account*/

// int get_solde()
// {
// 	return Compte_Courant->solde;
// }

int get_operations()
{
	/*
		CODE HERE
	*/
	return 0;
}

Compte popCompte_LIFO(Compte c)
{
	if (c == NULL || c->next_compte == NULL)
		return NULL;
	if (c->next_compte->next_compte==NULL)
	{
		c->next_compte=NULL;
		return c;

	}
	return popCompte_LIFO(c->next_compte);

}
Compte popCompte_FIFO(Compte c)
{
	if (c == NULL || c->next_compte == NULL)
		return NULL;
	else
	{
		Compte cpop=c;
		c=c->next_compte;

		return cpop;

	}

}
Compte getLastCompte(Compte c)
{
	if(c==NULL)
		return NULL;
	if(c->next_compte==NULL)
		return c;
	return getLastCompte(c->next_compte);

}

/* determine si les deux comptes designe le meme, i.e ont le meme code, i.e sont egaux */
int isEqualCompte(Compte c1, Compte c2)
{
	if(strcmp(c1->uuid_compte, c2->uuid_compte))
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;

}

/* renvoie le compte parmis les "comptes" qui est de type "type" */
Compte getCompte(Compte comptes, compte_t type)
{
	if(comptes==NULL)
		return NULL;

	if (comptes->type_compte == type
	 || comptes->nature_compte == type || comptes->statut == type)
	{
		return comptes;
	}
	else
		return isTypeCompte(comptes->next_compte, type);
}

int compareComptes(Compte c1, Compte c2,  compte_element_t element)
{
	if(c1==NULL || c2==NULL)
		return NULL;

	switch (element)
	{
		case IBAN:
			if (strcmp(c1->rib->iban, c2->rib->iban))
				return EXIT_SUCCESS;
			break;

		case CODE_BIC:
			if (strcmp(c1->rib->code_bic, c2->rib->code_bic))
				return EXIT_SUCCESS;
			break;

		case NUMERO_COMPTE :
			if (strcmp(c1->rib->numero_compte, c2->rib->numero_compte))
				return EXIT_SUCCESS;
			break;

		case INDICATIF_AGENCE :
			if (strcmp(c1->rib->indicatif_agence, c2->rib->indicatif_agence))
				return EXIT_SUCCESS;
			break;

		case DOMICILIATION :
			if (strcmp(c1->rib->domiciliation_agence, c2->rib->domiciliation_agence))
				return EXIT_SUCCESS;
			break;

		case STATUT :
			if (c1->statut == c2->statut)
				return EXIT_SUCCESS;
			break;

		case JOINT_OU_INDIVIDUEL :
			if (c1->statut == c2->statut)
				return EXIT_SUCCESS;
			break;

		case TYPE :
			if (c1->statut == c2->statut)
				return EXIT_SUCCESS;
			break;

		case RIB :
			compte_element_t elt=IBAN;

			while (elt <= DOMICILIATION && compareComptes(c1, c2, elt) == EXIT_SUCCESS)
			{
				elt++;
			}
			if(elt>DOMICILIATION)
				return EXIT_SUCCESS;
			break;

		case SOLDE :
			if (c1->solde == c2->solde)
				return EXIT_SUCCESS;
			break;

		case UUID_COMPTE:
			return isEqualCompte(c1, c2);
		break;
	default:
		break;
	}
	return EXIT_FAILURE;

}
int add_compte(Compte comptes, Compte c)
{
	Compte var = comptes;

	while (var != NULL)
	{

		if (isEqualCompte((var), c) == EXIT_SUCCESS)
		{
			return EXIT_FAILURE;
		}
		if (var->next_compte == NULL)
		{
			var->next_compte= c;
		}
		var = var->next_compte;
	}
	return EXIT_SUCCESS;
}
Compte containsCompte(Compte comptes, Compte var)
{
	if(comptes==NULL)
		return NULL;
	Compte count=comptes;
	do{
		if (isEqualCompte(count, var)==EXIT_SUCCESS) {
			return count;
		}
	} while (count->next_compte!=NULL && (count = count->next_compte) != NULL);

	return NULL;
}

int add_Compte(Compte c, Compte newC)
{
	if (c==NULL || newC==NULL)
		return EXIT_FAILURE;

	switch (newC->nature_compte)
	{
		case JOINT:
			if (getCompte(c, JOINT)==NULL && containsCompte(c,newC)== NULL)
			{
				getLastCompte(c)->next_compte= newC; //adding the new account(joint) to the end of the stack
				return EXIT_SUCCESS;
			}
			break;

		case INDIVIDUEL:
			if(containsCompte(c,newC)== NULL)
			{
				newC->next_compte = c; //adding the new account(INDIVIDUEL) to the begining of the stack
				return EXIT_SUCCESS;
			}
			break;

		default:
			return EXIT_FAILURE;
	}

	return  EXIT_FAILURE;


}

int addCompteJoint(Compte c1, Compte c2, Compte cJoint)
{
	Compte var1, var2;
	if ( c1 == NULL ||  c2 == NULL)
		return EXIT_FAILURE;

	switch (cJoint->nature_compte)
	{
		case JOINT:
			if(addCompte(c1, cJoint)==EXIT_SUCCESS)
			{
				if(addCompte(c2, cJoint)==EXIT_SUCCESS)
				{
					return EXIT_SUCCESS;

				}
				else//cJoint will be added only to c1 so we need to remove it
				{
					popCompte_LIFO(c1);
					return EXIT_FAILURE;
				}
			}
			return EXIT_FAILURE;


		default://INDIVIDUEL
			return EXIT_FAILURE;
	}


}

Compte init_compte()
{
	Compte newCompte = (Compte)calloc( 1, sizeof(struct compte) );



	/* TYPE COMPTE */
	newCompte->statut = UNINTIALIZED;
	newCompte->nature_compte = UNINTIALIZED;
	newCompte->type_compte = UNINTIALIZED;
	/*-----------*/


	newCompte->solde = UNINTIALIZED;

	newCompte->uuid_compte = (const char *)calloc(UUID_SIZE, sizeof(char));

	newCompte->rib=(Rib)calloc(1, sizeof(struct rib));
	newCompte->next_compte=NULL;


	return newCompte;
}


Compte init_compte_arg(char* uuid_compte, char* iban, int nature_compte, int type_compte, char* operations, Client_s titulaire)
{
	Compte newCompte = (Compte)calloc( 1, sizeof(struct compte) );

	newCompte->uuid_compte = (const char *)calloc(UUID_SIZE, sizeof(char));
	newCompte->operations = (const char *)calloc(UUID_SIZE, sizeof(char));

	strcpy(newCompte->uuid_compte, uuid_compte);
	strcpy(newCompte->operations, operations);

	/* TYPE COMPTE */
	newCompte->nature_compte = nature_compte;
	newCompte->type_compte = type_compte;
	/*-----------*/
	newCompte->titulaire=titulaire;
	newCompte->rib->iban = (const char *)calloc(IBAN_SIZE, sizeof(char));
	newCompte->rib=(Rib)calloc(1, sizeof(struct rib));
	strcpy(newCompte->rib->iban, iban);

	newCompte->next_compte=NULL;


	return newCompte;
}

Rib new_rib(const char *indicatif_agence, const char *domiciliation_agence, const char *code_bic)
{
	Rib newRib = (Rib)calloc(1, sizeof(struct rib));

		newRib->iban = (const char *)calloc(IBAN_SIZE, sizeof(char));
		newRib->code_bic = (const char *)calloc(CODE_BIC_SIZE, sizeof(char));
		newRib->numero_compte = (const char *)calloc(NUMERO_COMPTE_SIZE, sizeof(char));
		newRib->indicatif_agence = (const char *)calloc(INDICATIF_AGENCE_SIZE, sizeof(char));
		newRib->domiciliation_agence = (const char *)calloc(DOMICILIATION_SIZE, sizeof(char));

}

Compte new_acc(Client_s client, Rib rib, float solde, compte_t joint, compte_t type)
{

	Compte newCompte = init_compte();

	Client client1, client2;

	if (joint == JOINT)
	{
		client1=client.clients[0];
		client2=client.clients[1];
		if(client1 ==NULL || client2==NULL)
			return NULL;
		if (addCompteJoint(client1->comptes, client2->comptes, newCompte) != EXIT_SUCCESS)
			return NULL;
	}else
	{
		client1=client.client;
		if (client1 == NULL)
			return NULL;
		if (addCompte(client1->comptes, newCompte) != EXIT_SUCCESS)
			return NULL;
	}


	uuid_gen(newCompte->uuid_compte);

	newCompte->solde=solde;

	newCompte->statut=ENATTENTE;

	newCompte->type_compte=type;

	newCompte->rib=rib;

	return newCompte;
}