# Projet-s

All src codes are compiled with -fPIC (position indep code), dynamic librairies will be made. Library PATH must be exported during the session so the excutables will be able to find it. For example: export LD_LIBRARY_PATH=/path/to/Projet-s/lib:${LD_LIBRARY_PATH}.

Therefore executables are able to be used anywhere after compilation.

#1 Developper interface must be compiled first in order to create admins.
#2 After creating admins, Admin interface must be compiled in order to create the blank database. This would be possible after executing the interface and logging in using one of the admin logins created above.
#3 In order to be able to use the Managers interface agencies and employees must be created using the admin interface. Then using one of the employee logins made, manager interface would be usable.

Normally login and useful info would be suggested to save them in txt files in order for the user to be abe to access login data or print useful info for clients.

IMPORTANT: CSV files will be used to write bank account activity and they will be saved in database directory as well

There might be some glitches but im going to fixing them as soon as possible

/.OLD
Projet S4 UE: structures de donees
A simple bank management application. It consists of creating, modifying and showing bank accounts as well as the clients info and user accounts (only for admins). Also it lets non admin (clients) limited access to their account info and do some limited modifications also reset their login password. I will be working on creating and GUI.
Data will be encrypted and saved in a Json format. Admins and clients would be able to consult accounts operations and import them in a CSV format.
/
