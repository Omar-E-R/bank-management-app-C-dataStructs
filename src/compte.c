#include "compte.h"


Compte init_acc()
{
    Compte newCompte = (Compte)malloc(sizeof(Compte));


    newCompte->numero= 0;

    newCompte->solde=0;

    newCompte->statut=Vierge;
    
    newCompte->joint=NonJoint;

    (newCompte->clients)[0]= NULL;
    (newCompte->clients)[1]= NULL;

    newCompte->next_compte=NULL;

    return newCompte;
}

Compte new_acc(Client client1, Client client2, float solde)
{
    Compte newCompte = init_acc();

    srand(time(NULL));

    int num=(rand() % 1000);

    newCompte->numero= client1->numero_client *1000 + num;

    newCompte->solde=solde;

    newCompte->statut=EnAttente;

    if(client1!=NULL)
    {
        (newCompte->clients)[0] = client1;
        if (client1->comptes == NULL)
            client1->comptes=newCompte;
        else
        {

            Compte count = client1->comptes;

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

        (newCompte->clients)[1] = client2;

        if (client2->comptes == NULL)
            client2->comptes = newCompte;
        else
        {

            Compte count = client2->comptes;

            while (count->next_compte != NULL)
            {
                count = count->next_compte;
            }
            count->next_compte = newCompte;
        }
    }    

    return newCompte;
}