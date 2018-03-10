#include <stdio.h>
#include <string.h>
//#include <syscalls.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <sys/dir.h>
//#include <dirent.h>

#ifndef DIRSIZ
#define DIRSIZ
#endif


#define MAX_PATH 1024
#define NAME_MAX 14


typedef struct {
	long ino;
	char name[NAME_MAX + 1];
} Dirent;


typedef struct
{
	int fd;
	Dirent d;
} DIR;

struct direct
{
	ino_t d_ino;
	char d_name[DIRSIZ];
};


void 	fsize(char*);
void	dirwalk(char *, void (*fcn)(char *));


DIR *opendir(char *dirname)
{
	int fd;
	struct stat sbuf;
	DIR *dp;

	if ((fd = open(dirname, O_RDONLY, 0)) == -1)
		|| (fstat(fd, &stbuf) == -1)
		|| (stbuf.st_mode &S_IFMT) != S_IFDIR
		|| (dp = (DIR * )malloc(sizeof(DIR)) == NULL)
		return NULL;
		dp->fd = fd;
		return dp;
}

void	closedir(DIR *dp)
{
	if (dp)
	{
		close(dp->fd);
		free(dp);
	}
}

Dirent 	*readdir(DIR *dp)
{
	struct direct dirbuf;
	static Dirent d;

	while (read(dp->fd, (char*) &dirbuf, sizeof(dirbuf)) == sizeof (dirbuf))
	{
		if (dirbuf.d_ino == 0)
			continue;
		d.ino = dirbuf.d_ino;
		strncpy(d.name, dirbuf.d_name, DIRSIZ);
		d.name[DIRSIZ] = '\0';
		return &d;
	}
	return NULL;
}

void	fsize(char *name)
{
	struct stat stbuf;

	if (stat(name, &stbuf) == -1)
	{
		printf("fsize: can't access %s\n", name);
		return;
	}
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
		dirwalk(name, fsize);
	printf("%8ld %s\n", stbuf.st_size, name);
}

void	dirwalk(char *dir, void (*fcn)(char*))
{
	char name[MAX_PATH];
	Dirent *dp;
	DIR *dfd;

	if ((dfd = opendir(dir)) == NULL)
	{
		printf("dirwalk: can't open %s\n", dir);
		return;
	}
	while ((dp = readdir(dfd)) != NULL)
	{
		if (strcmp(dp->name, ".") == 0 || strcmp(dp->name, "..") == 0)
			continue;
		if (strlen(dir) + strlen(dp->name) + 2 > sizeof(name))
			printf("dirwalk: name %s %s too long\n", dir, dp->name);
		else
		{
			printf("name, %s/%s\n", dir, dp->name);
			(*fcn)(name);
		}
	}
	closedir(dfd);
}

int		main(int argc, char **argv)
{
	if (argc == 1)
		fsize(".");
	else
		while(--argc > 0)
			fsize(*++argv);
	return 0;
}