#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "libftprintf/includes/libft.h"

/*
 struct stat { // when _DARWIN_FEATURE_64_BIT_INODE is NOT defined //
         dev_t    st_dev;    // device inode resides on //
         ino_t    st_ino;    // inode's number //
         mode_t   st_mode;   // inode protection mode //
         nlink_t  st_nlink;  // number of hard links to the file //
         uid_t    st_uid;    // user-id of owner //
         gid_t    st_gid;    // group-id of owner //
         dev_t    st_rdev;   // device type, for special file inode //
         struct timespec st_atimespec;  // time of last access //
         struct timespec st_mtimespec;  // time of last data modification //
         struct timespec st_ctimespec;  // time of last file status change //
         off_t    st_size;   // file size, in bytes //
         quad_t   st_blocks; // blocks allocated for file //
         u_long   st_blksize;// optimal file sys I/O ops blocksize //
         u_long   st_flags;  // user defined flags for file //
         u_long   st_gen;    // file generation number //
     };



     int 		fstat(int fildes, struct stat *buf);
     int 		lstat(const char *restrict path, struct stat *restrict buf);
*/

typedef	struct s_options
{
	int a;
	int l;
	int R;
	int r;
	int t;
} t_options;

typedef enum
{
	TYPE,
	PERM,
	AMOUNT,
	USER,
	GROUP,
	SIZE,
	MONTH,
	DAY,
	TIME,
	NAME,
	FIELDS
} e_fields;

typedef enum
{
	T_DOW,
	T_MONTH,
	T_DAY,
	T_TIME,
	T_YEAR,
	T_END
} e_time;

void ft_print_time(struct stat *buf)
{
	//printf("%s\n",__FUNCTION__ );
	char mtime[100];
	char **date;

	ft_strcpy(mtime, ctime(&buf->st_mtime));
	date = ft_strsplit(mtime, ' ');
	//ft_putstr(mtime);
	/*while (*date)
	{
		ft_putstr(*date);
		ft_putchar('\n');
		date++;
	}*/
	ft_putstr(date[T_MONTH]);
	ft_putchar(' ');
	ft_putstr(date[T_DAY]);
	ft_putchar(' ');
	ft_putstr(date[T_TIME]);
	ft_putchar('\n');
}

void ft_read_file(char *name, t_options *options)
{
	/*DIR *dirp;
	struct dirent *info;
	
	dirp = opendir(name);
	if (!dirp)
	{
		fd = open()
	}
	while ((info = readdir(dirp)))
		printf("->	%s\n", info->d_name);
	closedir(dirp);*/
	struct stat buf;
	stat(name, &buf);
	if (options->l)
		ft_print_time(&buf);
	printf("read: %s\n", name);
}

int	main(int argc, char **argv)
{
	int i = 1;

	t_options *options;

	options = (t_options*)ft_memalloc(sizeof(t_options));

	if (argc > 1)
	{
		while (i < argc)
		{
			if (*argv[i] == '-')
			{
				//printf("--------->'%c'\n", *argv[i]);
				++argv[i];
				while (*argv[i] && ft_isalnum(*argv[i]))
					{
						//printf("--------->'%c'\n", *argv[i]);
						if (*argv[i] == 'a')
							options->a = 1;
						else if (*argv[i] == 'l')
							options->l = 1;
						else if (*argv[i] == 'R')
							options->R = 1;
						else if (*argv[i] == 'r')
							options->r = 1;
						else if (*argv[i] == 't')
							options->t = 1;
						else
						{
							printf("illegal option '%c'\n", *argv[i]);
							return(1);
						}
						//printf("first circle done\n");
						argv[i]++;
					}
					i++;
			}
			if (!argv[i])
				ft_read_file(".", options);
			else
				ft_read_file(argv[i], options);
			i++;
		}
		
	}
	else
		ft_read_file(".", options);
		
	printf("\n\n				options {a - %d, l - %d, R - %d, r - %d, t - %d}\n", options->a, options->l, options->R, options->r, options->t);
	free(options);
	return 0;
}
