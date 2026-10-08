//Otvoriti datoteku i ucitati koliko ima redaka(studenata) u zasebnu varijablu. 
//Nakon toga alocirati memoriju za niz struktura studenata i ucitati sve podatke iz datoteke u niz struktura.
//Na kraju ispisati ime, prezime, apsolutni i relativni broj bodova za svakog studenta.

#include <stdio.h>
#include <stdlib.h>

//Postavljam compile-time constants
#define MAX_LENGTH 50
#define BUFFER 1000
#define MAX_POINTS 50
#define OPEN_ERROR -1
#define MALLOC_ERROR -2
#define SCANF_ERROR -3

typedef struct _student {
	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	double points;
} Student;

int readRows();

int main()
{
	int i = 0;
    int numberOfRows = 0;
	numberOfRows = readRows();

	if (numberOfRows > 0)
	{
		FILE* fp = NULL;
		fp = fopen("students.txt", "r");

        //Provjeravam je datoteka otvorena
		if (!fp) {
			printf("File not opened!\n");
			return OPEN_ERROR;
		}

		Student* stud = NULL;
		stud = (Student*)malloc(numberOfRows * sizeof(Student));

        //Provjeravam je li malloc uspio
		if (stud == NULL) {
			printf("Error with malloc!\n");
			return MALLOC_ERROR;
		}
        
		for (i = 0; i < numberOfRows; i++) {
			if (fscanf(fp, " %s %s %lf ", stud[i].name, stud[i].surname, &stud[i].points) != 3)
				return SCANF_ERROR;
		}

		for (i = 0; i < numberOfRows; i++) {
			printf("%s %s %.2lf %.2lf\n", stud[i].name, stud[i].surname, stud[i].points, stud[i].points / MAX_POINTS * 100);
		}

		fclose(fp);
		free(stud);
	}

	return 0;
}

int readRows()
{
	int counter = 0;
	char buffer[BUFFER] = { 0 };

	FILE* fp = NULL;
	fp = fopen("students.txt", "r");
	if (!fp) {
		printf("File not opened!\n");
		return OPEN_ERROR;
	}

    while (fgets(buffer, BUFFER, fp) != NULL) {
    counter++;
    
    }
	fclose(fp);

	return counter;
}