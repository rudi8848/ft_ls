#include <stdio.h>

int		main(void)
{
	int i = 10;
//	printf("%s\n", "\033[6n");
//	printf("%c", 07);	//BELL
//	printf("\a");	//BELL
/*	while (i < 10)
	{
	printf("%s%c", "\033[10;i]", 07);
	i++;
	}*/

printf("start \033[10Gend\n");
//printf("start \033[10;5Hend\n");
printf("start \033[10;5fend\n");

	return(0);
}
