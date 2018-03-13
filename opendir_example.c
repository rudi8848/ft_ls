#include <dirent.h> // directory header
#include <stdio.h> // printf()
#include <stdlib.h> // exit()

#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include "libftprintf/includes/libft.h"
#include "libftprintf/includes/ft_printf.h"
#include <errno.h>

typedef struct      s_flist
{
    char            *name;
    char            *path;
    char            *mode;
    short           nlink;
    char            *user;
    char            *group;
    long            size;
    char            *day;
    char            *month;
    char            *time;
    struct s_flist  *next;
}                   t_flist;

void        ft_get_user_group(struct stat buf, t_flist **head);


void        ft_print_flist(t_flist *head)
{
    //printf("%s\n",__FUNCTION__ );
    //int total;

    //total = ft_flist_count(head);
    //ft_printf("total %d\n", total);
    while (head->next)
    {
      
            printf("%5s %3hi %5s %5s %5ld %4s %3s %-8s\n",
                head->mode, head->nlink, head->user, head->group,
                head->size, head->month, head->day,
                head->name);
       
        head = head->next;
    }
}

void        ft_push_fname(t_flist **head, char *name)
{
    //printf("%s\n",__FUNCTION__ );
    t_flist *tmp;

    tmp = (t_flist*)malloc(sizeof(t_flist));
    if (!tmp)
    {
        perror("ft_memalloc");
        exit(1);
    }
    tmp->name = strdup(name);
    tmp->next = (*head);
    (*head) = tmp;
}

void        ft_get_user_group(struct stat buf, t_flist **head)
{
    //printf("%s\n",__FUNCTION__ );
    struct passwd   *s_user;
    struct group    *s_group;

    s_user = getpwuid(buf.st_uid);
    (*head)->user = strdup(s_user->pw_name);
    s_group = getgrgid(buf.st_gid);
    (*head)->group = strdup(s_group->gr_name);

}

void        ft_get_size(struct stat buf, t_flist **file)
{
    //printf("%s\n",__FUNCTION__ );
    (*file)->size = buf.st_size;    
}

void        ft_get_time(struct stat buf, t_flist **file)
{
    ;
}

void        ft_get_mode(struct stat buf, t_flist **file)
{
    //printf("%s\n",__FUNCTION__ );
    (*file)->mode = (char*)malloc(10);
    if (!(*file)->mode)
    {
        perror("ft_strnew");
        exit(1);
    }
    //printf("\n-------%s %s mode: %hd---------\n", __FUNCTION__, (*file)->name, buf.st_mode);
    
    (*file)->mode[0] = S_IFDIR & buf.st_mode ? 'd' : '-';
    (*file)->mode[1] = S_IRUSR & buf.st_mode ? 'r' : '-';
    (*file)->mode[2] = S_IWUSR & buf.st_mode ? 'w' : '-';
    (*file)->mode[3] = S_IXUSR & buf.st_mode ? 'x' : '-';
    (*file)->mode[4] = S_IRGRP & buf.st_mode ? 'r' : '-';
    (*file)->mode[5] = S_IWGRP & buf.st_mode ? 'w' : '-';
    (*file)->mode[6] = S_IXGRP & buf.st_mode ? 'x' : '-';
    (*file)->mode[7] = S_IROTH & buf.st_mode ? 'r' : '-';
    (*file)->mode[8] = S_IWOTH & buf.st_mode ? 'w' : '-';
    (*file)->mode[9] = S_IXOTH & buf.st_mode ? 'x' : '-';
    //printf("\n-------%s %s mode: %s---------\n", __FUNCTION__, (*file)->name, (*file)->mode);
}

void        ft_get_links(struct stat buf, t_flist **file)
{
    //printf("%s\n",__FUNCTION__ );
    (*file)->nlink = buf.st_nlink;
}



void        ft_read_file(char *name, struct stat buf, t_flist **head)
{
    //printf("%s\n",__FUNCTION__ );
    ft_push_fname(head, name);
 
        ft_get_mode(buf, head);
        ft_get_user_group(buf, head);
        ft_get_time(buf, head);
        ft_get_size(buf, head);
        ft_get_links(buf, head);
    
}
char            *ft_strjoin(char const *s1, char const *s2)
{
    char        *dest;
    size_t      len;
    size_t      i;
    size_t      j;

    i = 0;
    j = 0;
    if (s1 && s2)
    {
        len = strlen(s1) + strlen(s2);
        dest = (char*)malloc(sizeof(char) * (len + 1));
        if (dest)
        {
            while (s1[i])
            {
                dest[i] = s1[i];
                i++;
            }
            while (s2[j])
                dest[i++] = s2[j++];
            dest[i++] = '\0';
            return (dest);
        }
    }
    return (NULL);
}

int main () // entry point of the program
{
    //printf("%s\n",__FUNCTION__ );
    t_flist *head = (t_flist*)malloc(sizeof(t_flist));
    struct stat buf;
    char *path;
    // first off, we need to create a pointer to a directory
    DIR *pdir = NULL; // remember, it's good practice to initialise a pointer to NULL!
    struct dirent *pent = NULL;

    // I used the current directory, since this is one which will apply to anyone reading
    // this tutorial~ If I said "C:\\" and you're on Linux, it may get a little confusing!
    pdir = opendir ("dir"); // "." will refer to the current directory
    if (pdir == NULL) // if pdir wasn't initialised correctly
    { // print an error message and exit the program
        printf ("\nERROR! pdir could not be initialised correctly");
        exit (3);
    } // end if

    while ((pent = readdir(pdir))) // while there is still something in the directory to list
    {
        if (pent == NULL) // if pent has not been initialised correctly
        { // print an error message, and exit the program
            printf ("ERROR! pent could not be initialised correctly");
            exit (3);
        }
        // otherwise, it was initialised correctly. let's print it on the console:
        stat(pent->d_name, &buf);
        path = ft_strjoin("dir/", pent->d_name);
        ft_read_file(path, buf, &head);
    }

    // finally, let's close the directory
    closedir (pdir);
    ft_print_flist(head);

    return 0; // everything went OK
}