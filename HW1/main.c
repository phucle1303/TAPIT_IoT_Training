#include <stdio.h>
#include <stdlib.h>

int main()
{
    char letter[100];
    int i=0,j,x;
    do
    {
        j=i;
        printf("Type a letter (type j to stop): ");
        scanf("%c", &letter[j]);
        getchar();
        i++;
    }while(letter[j]!='j');
    printf("\nLetter you inputted:\n");
    for (x=0;x<j;x++)
    {
        printf("%c", letter[x]);
    }
    printf("\n");
    return 0;
}
