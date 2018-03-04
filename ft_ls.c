#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	int res = 0;
	DIR *folder;
	struct dirent *info;

	if (argc == 2)
	{
		folder = opendir(argv[1]);
		info = readdir(folder);
		res = closedir(folder);
	}
	else
		printf("Enter the filename\n");
	return res;
}
