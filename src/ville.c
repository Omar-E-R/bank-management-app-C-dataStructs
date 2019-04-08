#include "ville.h"

/*


permet de decrypter les donnees des agences



*/

Ville init_ville()
{
	Ville ville = (Ville)calloc(1, sizeof(struct ville));

	ville->uuid_ville = (const char *)calloc(UUID_SIZE, sizeof(char));
	// ville->code_postale = (const char *)calloc(DOMICILIATION_SIZE, sizeof(char));
	// ville->nom_ville = (const char *)calloc(DOMICILIATION_SIZE, sizeof(char));

	return ville;
}

Admin init_admin()
{
	Admin admin = (Admin)calloc(1, sizeof(struct admin));

	admin->uuid_admin = (const char *)calloc(UUID_SIZE, sizeof(char));

	admin->next_admin=NULL;

	return admin;
}

Admin new_admin()
{
	Admin admin = init_admin();

	uuid_gen(admin->uuid_admin);

	return admin;
}

Ville new_ville(const char* nom_ville, const char* code_postale)
{
	Ville ville = init_ville();

	uuid_gen(ville->uuid_ville);
	ville->code_postale = nom_ville; 
	ville->nom_ville =  code_postale;

	return ville;
}


int addAgence(lAgence liste_agence, Agence agence)
{
	if (liste_agence == NULL || agence == NULL)
	{
		return FAILURE;
	}
	if (liste_agence->next_agence == NULL && isEqualConseiller(liste_agence->agence, agence) != MATCH)
	{
		liste_agence->next_agence = agence;

		return SUCCESS;
	}
	return addAgence(liste_agence->next_agence, agence);
}

return_type isEqualVille(Ville agence1, Ville agence2)
{
	if (strcmp(agence1->uuid_ville, agence2->uuid_ville))
		return MATCH;
	return NO_MATCH;
}
return_type isEqualAdmin(Admin admin1, Admin admin2)
{
	if (strcmp(admin1->uuid_admin, admin2->uuid_admin))
		return MATCH;
	return NO_MATCH;
}

int addVille(lData liste_ville, Ville ville)
{
	if (liste_ville == NULL || ville == NULL)
	{
		return FAILURE;
	}
	if (liste_ville->next_ville == NULL && isEqualConseiller(liste_ville->ville, ville) != MATCH)
	{
		liste_ville->next_ville = ville;

		return SUCCESS;
	}
	return addVille(liste_ville->next_ville, ville);
}
int addAdmin(Admin lAdmin, Admin admin)
{
	if (lAdmin == NULL || admin == NULL)
	{
		return FAILURE;
	}
	if (lAdmin->next_admin == NULL && isEqualConseiller(lAdmin, admin) != MATCH)
	{
		lAdmin->next_admin = admin;

		return SUCCESS;
	}
	return addAdmin(lAdmin->next_admin, admin);
}

Ville decrypt_agence(Login user)
{
    

}