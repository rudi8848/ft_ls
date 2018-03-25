#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct list
{
    char* data;
    struct list *next;
};

struct list * insert( struct list *node, char* data )
{
    //printf("\n%s-------------------> %p %s\n",__FUNCTION__, &node, data);
    struct list *tmp = malloc( sizeof( struct list ) );

    if ( tmp != NULL )
    {
        tmp->data = strdup(data);

        if ( node != NULL )
        {
            tmp->next = node->next;
            node->next = tmp;
        }
        else
        {
            tmp->next = NULL;
        }
    }
 //printf("\n%s-------------------> %p %s %p \n",__FUNCTION__, &node, data, &node->next);
    return tmp;
}

void display( struct list *node )
{
    for ( ; node != NULL; node = node->next ) printf("\n%s\n", node->data);//printf( "%s ", node->data );
}

struct list * clear( struct list *node )
{
    while ( node != NULL )
    {
        struct list *tmp = node;
        node = node->next;
        free(tmp->data);
        free( tmp );
    }

    return node;
}

struct list * sort( struct list *root )
{
    struct list *new_root = NULL;

    while ( root != NULL )
    {
        struct list *node = root;
        root = root->next;

        if ( new_root == NULL || (strcmp(node->data, new_root->data)< 0) )
        {
            node->next = new_root;
            new_root = node;
        }
        else
        {
            struct list *current = new_root;
            while ( current->next != NULL && !( (strcmp(node->data, current->next->data) < 0) ) )
            {                   
                  current = current->next;
            }                

            node->next = current->next;
            current->next = node;
        }
    }

    return new_root;
}



int main(int argc, char **argv)
{
   
    struct list *root = NULL;
    struct list **tmp = &root;

    //printf("\n%s--------------> %p %p\n",__FUNCTION__, &tmp, &root );
    for ( size_t i = 1; i < argc; i++ )
    {
        *tmp = insert( *tmp, argv[i] );
        tmp = &( *tmp )->next;
    }

    display( root );
    printf( "\n" );

    root = sort( root );
//printf("\n %s ---------------> %p %p %p\n", __FUNCTION__, &root, &root->next, &root->next->next );
    display( root );
    printf( "\n" );

    root = clear( root );
    system("leaks a.out");
}   