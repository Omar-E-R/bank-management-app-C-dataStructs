#include "compte.h"


Compte init_acc()
{
    Compte newCompte = (Compte)malloc(sizeof(Compte));


    newCompte->numero= 0;

    newCompte->solde=0;

    newCompte->statut=Vierge;
    
    newCompte->joint=NonJoint;

    (newCompte->titulaires)[0]= NULL;
    (newCompte->titulaires)[1]= NULL;

    newCompte->next_compte=NULL;

    return newCompte;
}

Compte new_acc(Client client1, Client client2, float solde)
{
    Compte newCompte = init_acc();

    srand(time(NULL));

    int num=(rand % 1000);

    newCompte->numero= client->numero_client *1000 + num;

    newCompte->solde=solde;

    newCompte->statut=Actif;

    if(client1!=NULL)
    {
        (newCompte->titulaires)[0] = client1;
        if (client1->compte == NULL)
            client1->compte=newCompte;
        else
        {

            Compte count = client1->compte;

            while (count->next_compte != NULL)
            {
                count = count->next_compte;
            }
            count->next_compte = newCompte;
        }
       
    }
    if(client2!=NULL)
    {
        newCompte->joint=Joint;

        (newCompte->titulaires)[1] = client2;

        if (client2->compte == NULL)
            client2->compte = newCompte;
        else
        {

            Compte count = client2->compte;

            while (count->next_compte != NULL)
            {
                count = count->next_compte;
            }
            count->next_compte = newCompte;
        }
    }    

    return newCompte;
}