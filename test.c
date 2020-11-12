#include<stdio.h>
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define ANSI_COLOR_CYAN "\x1b[36m"
int arr[4][11];
void display()
{
    printf("\n\n");
    printf(ANSI_COLOR_CYAN"                      <버스 좌석표>                           \n"ANSI_COLOR_RESET);
    printf(" ");
    for(int i=0;i<58;i++)
	printf("-");
    printf("\n");
    printf("|");
    for(int i=0;i<58;i++)
	printf(" ");
    printf("|\n");
    printf("|");
    for(int i=0;i<11;i++)
    {
	if(arr[0][i]==0)
	{
	    printf(ANSI_COLOR_GREEN "[A%d] "ANSI_COLOR_RESET,i+1);
	   
	}
	else
	{
	    printf(ANSI_COLOR_RED "[A%d] "ANSI_COLOR_RESET,i+1);
	}
    }
    printf(" |\n");
    printf("|");
    for(int i=0;i<11;i++)
    {
	if(arr[1][i]==0)
	{
	    printf(ANSI_COLOR_GREEN"[B%d] "ANSI_COLOR_RESET,i+1);
	}
	else
	{
	    printf(ANSI_COLOR_RED"[B%d] "ANSI_COLOR_RESET,i+1);
	}
    }
    printf(" |\n");
    printf("|");
    for(int i=0;i<58;i++)
	printf(" ");
    printf("|\n");
    printf("|");
    for(int i=0;i<58;i++)
	printf(" ");
    printf("|\n");
    printf("|");
    for(int i=0;i<11;i++)
    {
	if(arr[2][i]==0)
	{
	    printf(ANSI_COLOR_GREEN"[C%d] "ANSI_COLOR_RESET,i+1);
	}
	else
	{
	    printf(ANSI_COLOR_RED"[C%d] "ANSI_COLOR_RESET,i+1);
	}
    }
    printf(" |");
    printf("\n");
    printf("|");
    for(int i=0;i<11;i++)
    {
	if(arr[3][i]==0)
	{
	    printf(ANSI_COLOR_GREEN"[D%d] "ANSI_COLOR_RESET,i+1);
	}
	else
   	    printf(ANSI_COLOR_RED"[D%d] "ANSI_COLOR_RESET,i+1);
	
    }
    printf(" |");
    printf("\n");
    printf("|");
    for(int i=0;i<58;i++)
	printf(" ");
    printf("|\n");
    for(int i=0;i<58;i++)
    	printf("-");
    printf("\n");
}
int main()
{
    arr[2][1]=1;
    arr[3][6]=1;
    display();
    return 0;
}

