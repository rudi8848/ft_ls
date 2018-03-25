/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gvynogra <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/20 15:26:29 by gvynogra          #+#    #+#             */
/*   Updated: 2018/03/25 13:21:06 by gvynogra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <dirent.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/xattr.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <pwd.h>
# include <grp.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <time.h>
# include "libft/includes/libft.h"
# include "ft_printf.h"
# include <errno.h>

# define RESET		"\033[0m"
# define BOLD		"\033[1m"
# define RED 		"\033[0;31m"
# define GREEN 		"\033[0;32m"
# define YELLOW		"\033[0;33m"
# define BLUE		"\033[0;34m"
# define MAGENTA	"\033[0;35m"
# define CYAN		"\033[1;36m"
# define SIX_MONTH	15724800
# define FUTURE		3600

typedef	struct		s_opt
{
	int				a;
	int				l;
	int				rr;
	int				r;
	int				t;
	int				c1;
}					t_opt;

typedef struct		s_flist
{
	char			*name;
	char			*path;
	char			mode[12];
	short			nlink;
	char			*user;
	char			*group;
	long			size;
	long			blocks;
	char			*date;
	char			*color;
	char			*ref;
	time_t			mtime;
	struct s_flist	*next;
}					t_flist;

typedef int			(*t_pf_compare)(int, int);

typedef enum
{
	SO_ASC,
	SO_DESC,
	SORT_ORDERS
}	t_sort_order;

int					ft_read_args(char *name, t_opt options, t_flist **head);
char				*cut_name(char *str);
void				ft_set_color(struct stat buf, t_flist **head);
void				ft_check_xattr(t_flist *file);
void				ft_get_time(struct stat buf, t_flist **head);
void				ft_get_size(struct stat buf, t_flist **file);
void				ft_push_fname(t_flist **head, char *path);
void				ft_get_mode(struct stat buf, t_flist **file);
void				ft_get_user_group(struct stat buf, t_flist **head);
int					ft_flist_counter(t_flist *head);
void				ft_read_dir(char *path, t_opt options, t_flist **head);
void				ft_print_flist(t_opt options, t_flist *head);
void				print_recursion(char *path, t_opt options);
void				ft_clean_flist(t_opt options, t_flist **file);
t_flist				*ft_sort_flist(t_opt options, t_flist *head);
t_flist				*ft_sort_by_name(t_flist *a, t_flist *h, t_pf_compare cmp);
t_flist				*ft_sort_by_mtime(t_flist *a, t_flist *h, t_pf_compare cmp);
t_flist				*ft_get_nth(t_flist *head, int n);
size_t				ft_maxlen(t_flist *head);
void				ft_delete_flist(t_opt options, t_flist **head);
long				ft_count_blocks(t_flist *head);
int					ft_cmp_ascending(int a, int b);
int					ft_cmp_descending(int a, int b);

#endif
