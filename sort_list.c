#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct		s_flist
{
	char			*name;
	char			*path;
	char			*mode;
	short			nlink;
	char			*user;
	char			*group;
	long			size;
	char			*day;
	char			*month;
	char			*time;
	struct s_flist	*next;
}					t_flist;

void		ft_print_flist( t_flist *head)
{
	printf("-----------%s--------------\n",__FUNCTION__ );
	if (!head)
		{
			printf("NULL\n");
			return;
		}
	while (head->next)
	{
		printf("%-15s\n", head->name);
		head = head->next;
	}
	//printf("out of loop\n");
}

void		ft_push_fname(t_flist **head, char *name)
{
	printf("-----------%s-%s-------------\n",__FUNCTION__,name );
	t_flist *tmp;

	tmp = (t_flist*)malloc(sizeof(t_flist));
	if (!tmp)
		exit(1);
	tmp->name = strdup(name);
	tmp->next = (*head);
	(*head) = tmp;	
}

int			ft_flist_count(t_flist *head)
{
	int			i;

	i = 0;
	if (!head)
		return (0);
	while (head->next)
	{
		head = head->next;
		i++;
	}
	return (i);
}

void	insert(t_flist *head, unsigned n, char *name)
{
	unsigned i = 0;
	t_flist *tmp = NULL;
	while (i < n && head->next)
	{
		head = head->next;
		i++;
	}
	tmp = (t_flist*)malloc(sizeof(t_flist));
	tmp->name = name;
	if (head->next)
		tmp->next = head->next;
	else
		tmp->next = NULL;
	head->next = tmp;
}
/*
t_flist *sort(t_flist *head)			// функция возвращает заголовок нового списка
 {
 	printf("-----------%s--------------\n",__FUNCTION__ );
 	t_flist *cur, *out, *p , *prev;
 out = NULL;
 while (head !=NULL)
	  {
	  	ft_print_flist(out);
	  cur = head; head = head->next;
	  for ( p = out, prev = NULL; p != NULL && strcmp(cur->name, p->name) >= 0; prev = p, p = p->next);
	  if (prev == NULL)
		  {
		  	cur->next = out;
		  	out = cur;
		  }
	 else
	 	{
	 		cur->next = p;
	 		prev->next = cur;
		   }
		//printf("%s\n", cur->name);
	  
}
 return out;
}*/

//------------------------------------------------------------
t_flist *sort( t_flist *root )
{
	printf("-----------%s--------------\n",__FUNCTION__ );
    t_flist *a = NULL;

    while ( root != NULL )
    {
        t_flist *b;

        if(root->next)
        	b = root;
        else
        	return a;
        root = root->next;

        if ( a == NULL || (strcmp(b->name, a->name) < 0) )
        {
            b->next = a;
            a = b;
        }
        else
        {
            t_flist *c;
            c = a;
            while ( c->next != NULL && !(strcmp(b->name, c->next->name) < 0) )
            {                   
                  c = c->next;
            }                

            b->next = c->next;
            c->next = b;
        }
    }
    return a;
}
//------------------------------------------------------------


int 		main(int argc, char **argv)
{
	printf("-----------%s--------------\n",__FUNCTION__ );
	t_flist *head;
	

	int i = 1;

	head = (t_flist*)malloc(sizeof(t_flist));
	if (!head)
	{
		perror(__FUNCTION__);
		exit(1);
	}
	head->next = NULL;
	while (i < argc)
	{
		ft_push_fname(&head, argv[i]);
		i++;
	}
	ft_print_flist(head);
		printf("-----------%s--------------\n",__FUNCTION__ );
	head = sort(head);
	ft_print_flist(head);

	return 0;
}