#include <dirent.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include "libftprintf/includes/libft.h"
#include "libftprintf/includes/ft_printf.h"
#include <errno.h>

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



     int 	stat(const char *file_name, struct stat *buf);
     int 	lstat(const char *file_name, struct stat *buf);
*/

typedef	struct s_opt
{
	int a;
	int l;
	int R;
	int r;
	int t;
} t_opt;

typedef enum
{
	TYPE,
	PERM,
	LINKS,
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

typedef struct s_flist
{
	char *name;
	char *path;
	char mode[11];
	short nlink;
	char *user;
	char *group;
	long size;
	char date;
	struct s_flist *next;
} t_flist;

void ft_read_args(char *name, t_opt *options, t_flist **head);
void ft_print_time(struct stat *buf);
void ft_read_file();
void ft_read_dir(DIR *dirp, t_opt *options, t_flist *head);
void ft_sort_flist(void)
{
	;
}
void ft_print_flist(t_flist *head)
{
	while (head->next)
	{
		ft_printf("%5p %5s %5s %5s %15s\n",head, head->mode, head->user, head->group, head->name);
		head = head->next;
	}
}

void ft_delete_flist(void)
{
	;
}


void	ft_push_fname(t_flist **head, char *name)
{
	t_flist *tmp;

	tmp = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!tmp)
	{
		perror("ft_memalloc");
		exit(1);
	}
	tmp->name = ft_strdup(name);
	tmp->next = (*head);
	(*head) = tmp;
}

void ft_print_time(struct stat *buf)
{
	char mtime[100];
	char **date;

	ft_strcpy(mtime, ctime(&buf->st_mtime));
	date = ft_strsplit(mtime, ' ');
	ft_putstr(date[T_MONTH]);
	ft_putchar(' ');
	ft_putstr(date[T_DAY]);
	ft_putchar(' ');
	write(1, date[T_TIME], 5);
	ft_putchar(' ');
}

void get_mode(struct stat buf, t_flist *file)
{
	file->mode[0] = S_IFDIR & buf.st_mode ? 'd' : '-';
	file->mode[1] = S_IRUSR & buf.st_mode ? 'r' : '-';
	file->mode[2] = S_IWUSR & buf.st_mode ? 'w' : '-';
	file->mode[3] = S_IXUSR & buf.st_mode ? 'x' : '-';
	file->mode[4] = S_IRGRP & buf.st_mode ? 'r' : '-';
	file->mode[5] = S_IWGRP & buf.st_mode ? 'w' : '-';
	file->mode[6] = S_IXGRP & buf.st_mode ? 'x' : '-';
	file->mode[7] = S_IROTH & buf.st_mode ? 'r' : '-';
	file->mode[8] = S_IWOTH & buf.st_mode ? 'w' : '-';
	file->mode[9] = S_IXOTH & buf.st_mode ? 'x' : '-';
	//write(1, file->mode, 10);
	//ft_printf("%8s", file->mode);
}

void ft_read_link(struct stat buf, t_flist *head)
{
	;
}

void ft_read_file(struct stat buf, t_flist **head)
{
	//printf("links: %hd\t",buf.st_nlink);
	//printf("uid %d\t", buf.st_uid);
	//printf("gid %d\t", buf.st_gid);
	//printf("size %ld\t", buf.st_size);
	//ft_print_time(&buf);
	//printf("st_mode %d", buf.st_mode);
	//printf("%d\n");
	get_mode(buf, *head);
}

void ft_read_dir(DIR *dirp, t_opt *options, t_flist *head)
{
	struct dirent *info;
	struct stat buf;
	while ((info = readdir(dirp)))
	{
		if (ft_strequ(info->d_name, ".")||ft_strequ(info->d_name, ".."))
		 	{
		 		printf("->	%s\n", info->d_name);
		 		continue;
		 	}
		if (options->R)
		{
			stat(info->d_name, &buf);
			ft_read_file(buf, &head);

		}	//ft_read_args(info->d_name, options);
		else
			printf("->	%s\n", info->d_name);
	}
}

void	ft_get_user_group(struct stat buf, t_flist **head)
{
	struct passwd *s_user;
	struct group *s_group;

	s_user = getpwuid(buf.st_uid);
	(*head)->user = ft_strdup(s_user->pw_name);
	s_group = getgrgid(buf.st_gid);
	(*head)->group = ft_strdup(s_group->gr_name);

}

void ft_read_args(char *name, t_opt *options, t_flist **head)
{
	int ret;
	DIR *dirp;
	struct stat buf;

	ret = stat(name, &buf);
	if (ret >= 0)
	{
		ft_push_fname(head, name);
		ft_get_user_group(buf, head);
		if (S_ISREG(buf.st_mode))
			ft_read_file(buf, head);
		else if (S_ISDIR(buf.st_mode))
		{
			dirp = opendir(name);
			ft_read_dir(dirp, options, *head);
			closedir(dirp);
		}
		else if (S_ISLNK(buf.st_mode))
			ft_read_link(buf, *head);
	}
	else
		perror(strerror(ret));
//ft_printf("%s %8s %8s\n", __FUNCTION__, (*head)->user, (*head)->group);
}

int	main(int argc, char **argv)
{
	int i = 1;
	t_flist *head = NULL;
	t_opt *options;

	options = (t_opt*)ft_memalloc(sizeof(t_opt));
	head = (t_flist*)ft_memalloc(sizeof(t_flist));
	if (!options)
		perror("Error");
	
//printf("-------------------------------------------------------------\n");
	if (argc > 1)
	{
		while (i < argc)
		{
			if (*argv[i] == '-')
			{
				++argv[i];
				while (*argv[i] && ft_isalnum(*argv[i]))
					{
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
						argv[i]++;
					}
					i++;
			}
			if (!argv[i])
				ft_read_args(".", options,&head);

			else
				ft_read_args(argv[i], options, &head);
						i++;
		}
		
	}
	else
		ft_read_args(".", options, &head);
	ft_sort_flist();
//ft_printf("%s %8s %8s %8s\n", __FUNCTION__, head->user, head->group, head->name);
	ft_print_flist(head);
	ft_delete_flist();
//	printf("\n-------------------------------------------------------------\noptions {a - %d, l - %d, R - %d, r - %d, t - %d}\n", options->a, options->l, options->R, options->r, options->t);
	free(options);
	return 0;
}
