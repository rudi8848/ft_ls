#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

define	struct s_options
{
	int a;
	int l;
	int R;
	int r;
	int t;
} t_options;

int	main(int argc, char **argv)
{
	//int i = 0;
	DIR *dirp;
	struct dirent *info;
	char *name;
	if (argc <= 2)
	{
		if (argc == 2)
			name = argv[1];
		else
			name = ".";
		dirp = opendir(name);
		if (!dirp)
		{
			printf("Cannot open directory\n");
			exit (1);
		}
		while ((info = readdir(dirp)))
			printf("->	%s\n", info->d_name);
		closedir(dirp);
	}
	else
		printf("Too much agrs\n");
	return 0;
}
