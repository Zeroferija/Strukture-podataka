#include <stdio.h>
#include <stdlib.h>
#define OPEN_ERROR -1


typedef struct _Student {   // Structura za studenta
    char name[20];
    char lastName[20];
    int bodovi;
}stud;



void readRows( )

int main() {

FILE *fp;
fp = fopen("studenti.txt", "r"); // Otvaranje datoteke za citanje
if (fp == NULL) {
    printf("Greska pri otvaranju datoteke!\n");
    return OPEN_ERROR;
}







return 0;
}