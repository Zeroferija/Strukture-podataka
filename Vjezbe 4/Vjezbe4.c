/* Napisati program za zbrajanje i množenje polinoma.
Koeficijenti i eksponenti se čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani. */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024
#define MAX_SIZE 100

#define EXIT_SUCCESS 0
#define FILE_ERROR_OPEN -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct monomial {
	int coef;		//koeficijent
	int exp;		//eksponent
	struct monomial* next;	//pokazivač na sljedeci clan
} Mono;

int strToPoly(Mono*, char*);
int addMonoToPoly(Mono*, Mono*);
int printMono(Mono*);
int printPoly(Mono*);
int deletePoly(Mono*);
int polySum(Mono*, Mono*, Mono*);
int polyProduct(Mono*, Mono*, Mono*);

int main()
{
	Mono poly1Head = {
		.coef = 0, 
		.exp = 0, 
		.next = NULL
	};

	Mono poly2Head = { 
		.coef = 0, 
		.exp = 0, 
		.next = NULL 
	};

	Mono sumHead = { 
		.coef = 0, 
		.exp = 0, 
		.next = NULL 
	};

	Mono productHead = { 
		.coef = 0, 
		.exp = 0, 
		.next = NULL 
	};


	int i = 0;
	int size = 0;
	char buffer[MAX_LINE] = { 0 };

	FILE* filePointer = NULL;
	filePointer = fopen("polynomials.txt", "r");

	if (filePointer == NULL) {
		printf("File could not be opened.\n");
		return FILE_ERROR_OPEN;
	}

	// Cita prvu liniju i konvertuje je u poly1
	fgets(buffer, MAX_LINE, filePointer);
	strToPoly(&poly1Head, buffer);

	// Cita drugu liniju i konvertuje je u poly2
	fgets(buffer, MAX_LINE, filePointer);
	strToPoly(&poly2Head, buffer);

	printf("Polynomial 1: ");
	printPoly(poly1Head.next);
	printf("Polynomial 2: ");
	printPoly(poly2Head.next);

	polySum(&sumHead, poly1Head.next, poly2Head.next);
	printf("Sum: ");
	printPoly(sumHead.next);

	polyProduct(&productHead, poly1Head.next, poly2Head.next);
	printf("Product: ");
	printPoly(productHead.next);

	
	fclose(filePointer);

	deletePoly(&poly1Head);
	deletePoly(&poly2Head);
	deletePoly(&sumHead);
	deletePoly(&productHead);
	
	return EXIT_SUCCESS;
}

// Funkcija dodaje monom u polinom. Ako postoji monom sa istim eksponentom, koeficijenti se zbrajaju.
int addMonoToPoly(Mono* head, Mono* mono)
{
	Mono* prev = head;
	Mono* temp = NULL; // Pokazivač za privremeno pohranjivanje čvora (za brisanje)

	// ax^0 = 0
	if (mono->coef == 0) {
		free(mono);
		return EXIT_SUCCESS;
	}

	// Trazi prvi monom sa eksponentom manjim ili jednakim od eksponenta novog monoma
	while (prev->next != NULL && prev->next->exp > mono->exp) 
		prev = prev->next;

	// ax^n + bx^n = (a+b)x^n
	if (prev->next != NULL && mono->exp == prev->next->exp)
	{
		prev->next->coef += mono->coef;
		free(mono);

		if (prev->next->coef == 0)
		{
			temp = prev->next;
			prev->next = prev->next->next;
			free(temp);
		}
	}
		
	// Ubacuje novi monom u listu 
	else {
		mono->next = prev->next;
		prev->next = mono;
	}

	return EXIT_SUCCESS;
}

/*Funkcija dobije jedan redak iz datoteke, npr. "3x^2 -5x^1 4x^0\n", i od njega napravi listu clanova.
Svaki clan izvuce iz teksta, stavi u novi cvor i preda addMonoToPoly da ga smjesti na pravo mjesto. */
int strToPoly(Mono* head, char* str)
{
	int size = 0;
	Mono* mono = NULL;

	while (strlen(str) > 0)
	{
		mono = (Mono*)malloc(sizeof(Mono));
		if (mono == NULL) return MALLOC_ERROR;

		/* sscanf vraca broj uspješno procitanih vrijednosti. Ocekujemo 2(coef i exp). %n se ne broji, pa je provjera != 2.
		Ako nije 2, tekst nije u ocekivanom obliku i vraca se SCANF_ERROR. */
		if (sscanf(str, " %dx^%d %n", &mono->coef, &mono->exp, &size) != 2) return SCANF_ERROR;

		addMonoToPoly(head, mono);
		str += size;
	}

	return EXIT_SUCCESS;
}

int printMono(Mono* mono)
{
	
	mono->coef > 0 ? printf("%d", mono->coef) : printf("- %d", -mono->coef);

	if (mono->exp == 1) printf("x ");
	else if (mono->exp != 0) printf("x^%d ", mono->exp);

	return EXIT_SUCCESS;
}

int printPoly(Mono* current)
{
	if (current != NULL) {
		printMono(current);
		current = current->next;
	}

	while (current != NULL)
	{
		if (current->coef > 0) printf("+ ");
		printMono(current);
		current = current->next;
	}

	printf("\n");

	return EXIT_SUCCESS;
}

int deletePoly(Mono* head)
{
	Mono* temp = NULL;

	while (head->next != NULL)
	{
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}

	return EXIT_SUCCESS;
}

int polySum(Mono* sum, Mono* poly1, Mono* poly2)
{
	Mono* temp = NULL;

	while (poly1 != NULL)
	{
		temp = (Mono*)malloc(sizeof(Mono));
		if (temp == NULL) return MALLOC_ERROR;

		temp->coef = poly1->coef;
		temp->exp = poly1->exp;

		addMonoToPoly(sum, temp);
		poly1 = poly1->next;
	}

	while (poly2 != NULL)
	{
		temp = (Mono*)malloc(sizeof(Mono));
		if (temp == NULL) return MALLOC_ERROR;

		temp->coef = poly2->coef;
		temp->exp = poly2->exp;

		addMonoToPoly(sum, temp);
		poly2 = poly2->next;
	}

	return EXIT_SUCCESS;
}

int polyProduct(Mono* product, Mono* poly1, Mono* poly2)
{
	Mono* temp = NULL;
	Mono* poly2first = poly2;

	while (poly1 != NULL)
	{
		poly2 = poly2first;
		while (poly2 != NULL)
		{
			temp = (Mono*)malloc(sizeof(Mono));
			if (temp == NULL) return MALLOC_ERROR;

			temp->coef = poly1->coef * poly2->coef;
			temp->exp = poly1->exp + poly2->exp;

			addMonoToPoly(product, temp);
			poly2 = poly2->next;
		}
		poly1 = poly1->next;
	}

	return EXIT_SUCCESS;
}