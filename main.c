#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void header()
{
    printf("--------------\n");
    printf("1 - Add\n");
    printf("2 - Display\n");
    printf("3 - Delete\n");

    printf("0 - Exit\n");
    printf("--------------\n");
}


void add() {
   char a;
   printf("Not implemented please press enter");
   getchar();
   scanf("%c", &a );
}


void display() {

char a;
   printf("Not implemented please press enter");
   getchar();
   scanf("%c", &a );
}


void delete()
{
 char a;
   printf("Not implemented please press enter");
   getchar();
   scanf("%c", &a );
}


void menu(int op)
{
    switch(op)
    {
    case 1:
        add();
        break;
    case 2:
        display();
        break;
    case 3:
        delete();
        break;
    default:
        printf("EXIT!\n");
    }
}


int main()
{
    int option;

    do{
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        menu(option);
    } while(option > 0 && option <= 3);

    return 0;
}

