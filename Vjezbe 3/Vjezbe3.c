//3. Prethodnom zadatku dodati funkcije:
//A. dinamički dodaje novi element iza određenog elementa,
//B. dinamički dodaje novi element ispred određenog elementa,
//C. sortira listu po prezimenima osoba,
//D. upisuje listu u datoteku,
//E. čita listu iz datoteke.
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50
#define EXIT_SUCCESS 0
#define MALLOC_ERROR -2
#define SCANF_ERROR -3
#define FILE_ERROR_OPEN -4

//Pokazivac na strukturu osoba
struct _person;
typedef struct _person* Position;

//Struktura osoba
typedef struct _person {
    char name[MAX_SIZE];
    char surname[MAX_SIZE];
    int birthYear;
    Position next;
} Person;

//Funkcije 
Position createPerson(void);
int addToFrontOfTheList(Position);
int addToEndOfTheList(Position);
int printPerson(Position);
int printList(Position);
Position findPerson(Position, char*);
Position findPersonPrev(Position, char*);
int deletePerson(Position, char*);
int addAfter(Position, char*);
int addBefore(Position, char*);
int fileWrite(Position);
int _fileRead();
int fileRead(Position);


int main(void)
{
    char in = 0;
    char surname[MAX_SIZE];
    Position found = NULL;

    //Definiranje head elementa liste
    Person head = {
        .next = NULL,
        .name = {0},
        .surname = {0},
        .birthYear = 0
    };

    do {
        printf("\nA-dodaj na pocetak, C-dodaj na kraj, B-ispis, D-trazi, E-brisi, X-izlaz: ");
        if (scanf(" %c", &in) != 1) return SCANF_ERROR;

        switch (in) {
        case 'A':
        case 'a':
            addToFrontOfTheList(&head);
        break;

        case 'C':
        case 'c': 
            addToEndOfTheList(&head);   
        break;

        case 'B': 
        case 'b': 
            printList(head.next);       
        break;

        case 'D': 
        case 'd':
            printf("Prezime: ");
            scanf(" %49s", surname);
            found = findPerson(head.next, surname);
            if (found) printPerson(found);
            else printf("\tNije pronadjeno.\n");
        break;

        case 'E': 
        case 'e':
            printf("Prezime: ");
            scanf(" %49s", surname);
            deletePerson(&head, surname);
        break;
        }
    } while (in != 'X' && in != 'x');

    // oslobadanje cijele liste
    while (head.next != NULL) {
        Position temp = head.next;
        head.next = temp->next;
        free(temp);
    }
 
    return EXIT_SUCCESS;
}


//Funkcija za kreiranje osobe te dinamicko alociranje memorije za nju. 
//Ukoliko je alokacija uspjesna, funkcija vraca pokazivac na kreiranu osobu, a ukoliko nije, funkcija vraca NULL.
Position createPerson(void)
{
    Position person = NULL;

    person = (Position)malloc(sizeof(Person));

    if (person == NULL)
        return NULL;

    printf("\tName: ");

    if (scanf(" %49s", person->name) != 1) {
        free(person);
        return NULL;
    }

    printf("\tSurname: ");

    if (scanf(" %49s", person->surname) != 1) {
        free(person);
        return NULL;
    }

    printf("\tBirth year: ");

    if (scanf("%d", &person->birthYear) != 1) {
        free(person);
        return NULL;
    }

    person->next = NULL;

    return person;
}

//Funkcija za dodavanje osobe na pocetak liste.
int addToFrontOfTheList(Position head)
{
    Position newPerson = createPerson();

    if (newPerson == NULL) {
        printf("Error creating person!\n");
        return MALLOC_ERROR;
    }

    newPerson->next = head->next;
    head->next = newPerson;

    return EXIT_SUCCESS;
}
//Funkcija za dodavanje osobe na kraj liste.
int addToEndOfTheList(Position head)
{
    Position newPerson = createPerson();

    if (newPerson == NULL) {
        printf("Error creating person!\n");
        return MALLOC_ERROR;
    }

    while (head->next != NULL)
        head = head->next;

    newPerson->next = NULL;
    head->next = newPerson;

    return EXIT_SUCCESS;
}

//Funkcija za ispis osobe.
int printPerson(Position person)
{
    printf( " %s %s %d\n", person->name, person->surname, person->birthYear);

    return EXIT_SUCCESS;
}

//Funkcija za ispis liste.
int printList(Position current)
{
    if (current == NULL) {
        printf("\tEmpty list!\n");
        return EXIT_SUCCESS;
    }

    while (current != NULL) {
        printPerson(current);
        current = current->next;
    }

    return EXIT_SUCCESS;
}

//Funkcija za pronalazenje osobe u listi po prezimenu.
Position findPerson(Position current, char* sur)
{
    while (current != NULL && strcmp(current->surname, sur) != 0) {
		current = current->next;
    }

    return current;
}

//Funkcija za pronalazenje prethodnog elementa osobe u listi po prezimenu.
Position findPersonPrev(Position current, char* sur)
{
    while (current->next != NULL && strcmp(current->next->surname, sur) != 0) {
        current = current->next;
    }

    if (current->next == NULL)
        return NULL;

    return current;
}


//Funkcija za brisanje osobe iz liste po prezimenu.
int deletePerson(Position head, char* sur)
{
    Position temp = NULL;
    Position prev = findPersonPrev(head, sur);

    if (prev != NULL) {
        temp = prev->next;
        prev->next = temp->next;
        free(temp);

        printf("\tPerson deleted.\n");
    }
    else {
        printf(
            "\tPerson with surname '%s' was not found.\n",
            sur
        );
    }

    return EXIT_SUCCESS;
}

//Funkcija za dodavanje osobe iza odredene osobe u listi po prezimenu.
int addAfter(Position head, char* sur)
{
	Position newPerson = NULL, personWithSurname = NULL;

	personWithSurname = findPerson(head, sur);

	if (personWithSurname != NULL)
	{
		newPerson = createPerson();

        //Provjera je malloc uspio
		if (!newPerson) {
			printf("Malloc error!\n");
			return MALLOC_ERROR;
		}

		newPerson->next = personWithSurname->next;
		personWithSurname->next = newPerson;
	}
	else printf("\tPerson with surname '%s' was not found.\n", sur);

	return EXIT_SUCCESS;
}

//Funkcija za dodavanje osobe ispred odredene osobe u listi po prezimenu.
int addBefore(Position head, char* sur)
{
	Position newPerson = NULL, personBeforeSurname = NULL;

	personBeforeSurname = findPersonPrev(head, sur);
	
	if (personBeforeSurname != NULL)
	{
		newPerson = createPerson();

        //Provjera je malloc uspio
		if (!newPerson) {
			printf("Malloc error!\n");
			return MALLOC_ERROR;
		}

		newPerson->next = personBeforeSurname->next;
		personBeforeSurname->next = newPerson;
	}
	else printf("\tPerson with surname '%s' was not found.\n", sur);
	
	return EXIT_SUCCESS;
}

//Upisuje listu u datoteku
int fileWrite(Position current)
{
	FILE* filePointer = NULL;
	filePointer = fopen("persons.txt", "w");

	if (filePointer == NULL) {
		printf("\nFile could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	while (current != NULL) {
		fprintf(filePointer, "%s %s %d\n", current->name, current->surname, current->birthYear);
		current = current->next;
	}

	fclose(filePointer);

	printf("\tSuccess!\n");

	return EXIT_SUCCESS;
}

//Cita i ispisuje datoteku
int _fileRead()
{
	char c = 0;
	FILE* fp = NULL;
	fp = fopen("persons.txt", "r");

	if (fp == NULL) {
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

    //Citamo i ispisujemo znak po znak dok ne dodjemo do kraja datoteke
	c = getc(fp);
	while (c != EOF) {
		printf("%c", c);
		c = getc(fp);
	}
		
	fclose(fp);

	return EXIT_SUCCESS;
}

//Cita datoteku i dodaje elemente u listu
int fileRead(Position head)
{
	FILE* filePointer = NULL;
	filePointer = fopen("persons.txt", "r");

	if (!filePointer) {
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}


	Position prev = NULL, current = NULL;

	deleteList(head); // so repeated reading won't append file content to previous content
	prev = head;

	while (!feof(filePointer))
	{
		current = (Position)malloc(sizeof(Person)); // pointer to newly read person
		if (!current) {
			printf("Malloc error!\n");
			return MALLOC_ERROR;
		}
		if (fscanf(filePointer, " %s %s %d ", current->name, current->surname, &current->birthYear) != 3)
			return SCANF_ERROR;

		// add new person after previous
		current->next = prev->next;
		prev->next = current;

		prev = current; // newly read person will be the previous person in next iteration
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}