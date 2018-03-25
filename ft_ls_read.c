#include "ft_ls.h"

void			ft_read_link(struct stat buf, t_flist **head)
{
	int				len;
	char			*ref;
	int				ret;
	char			*prefix;

	len = buf.st_size;
	ref = (char*)ft_memalloc(sizeof(char) * (len + 1));
	ret = readlink((*head)->path, ref, len);
	if (ret < 0)
		perror("cannot read");
	ref[len] = '\0';
	prefix = " -> ";
	(*head)->ref = ft_strjoin(prefix, ref);
	free(ref);
}

void			ft_read_file(char *pth, t_opt opt, struct stat bf, t_flist **hd)
{
	ft_push_fname(hd, pth);
	(*hd)->mtime = bf.st_mtime;
	ft_set_color(bf, hd);
	(*hd)->ref = "";
	if (opt.l)
	{
		if (S_ISLNK(bf.st_mode))
			ft_read_link(bf, hd);
		ft_get_mode(bf, hd);
		ft_get_user_group(bf, hd);
		ft_get_time(bf, hd);
		ft_get_size(bf, hd);
		(*hd)->nlink = bf.st_nlink;
	}
}

void			ft_read_dir_info(char *pref, char *fnm, t_opt opt, t_flist **hd)
{
	struct stat		buf;
	char			*path;

	path = ft_strjoin(pref, fnm);
	lstat(path, &buf);
	ft_read_file(path, opt, buf, hd);
	free(path);
}

void			ft_read_dir(char *name, t_opt options, t_flist **head)
{
	struct dirent	*info;
	DIR				*dirp;
	char			*prefix;

	dirp = opendir(name);
	if (dirp == NULL)
	{
		perror("cannot open");
		return ;
	}
	prefix = ft_strjoin(name, "/");
	while ((info = readdir(dirp)))
	{
		if (info == NULL)
		{
			perror("cannot read");
			continue;
		}
		if (!options.a && ((*info).d_name[0] == '.'))
			continue;
		ft_read_dir_info(prefix, info->d_name, options, head);
	}
	free(prefix);
	closedir(dirp);
}

int			ft_read_args(char *name, t_opt options, t_flist **head)
{
	int				ret;
	struct stat		buf;

	ret = lstat(name, &buf);
	if (ret >= 0)
	{
		if (S_ISDIR(buf.st_mode))
			ft_read_dir(name, options, head);
		else
			ft_read_file(name, options, buf, head);
		return (1);
	}
	else
	{
		ft_printf("./ft_ls: %s: ", name);
		perror("cannot access");
	}
	return (0);
}