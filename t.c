#include<stdio.h>
int arr[4][11];
void display()
{
    printf("\n\n");
    for(int i=0;i<20;i++)
	printf("-");
    printf("\n");
    printf("|\n");
    for(int i=0;i<11;i++)
    {
	if(arr[0][i]==0)
	{
	    printf("[A%d] ",i+1);
	}
	else
	{
	    printf("[A%d] ",i+1);
	}
    }
}
int main()
{
    display();
    return 0;
}
