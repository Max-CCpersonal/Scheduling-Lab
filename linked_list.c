#include <stdlib.h>
#include "linked_list.h"

// Creates and returns a new list
// If compare is NULL, list_insert just inserts at the head
list_t* list_create(compare_fn compare)
{
    /* IMPLEMENT THIS */
    list_t* new_list = (list_t*)malloc(sizeof(list_t));
    if (new_list == NULL){
        return NULL;
    }
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->count = 0;
    new_list->compare = compare;
    return new_list;
}

// Destroys a list
void list_destroy(list_t* list)
{
    /* IMPLEMENT THIS */
    list_node_t* curr = list->head;
    while(curr != NULL){
        list_node_t* next = curr->next;
        free(curr);
        curr = next;
    }
    free(list);

}

// Returns head of the list
list_node_t* list_head(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->head;
}

// Returns tail of the list
list_node_t* list_tail(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->tail;
}

// Returns next element in the list
list_node_t* list_next(list_node_t* node)
{
    /* IMPLEMENT THIS */
    return node->next;
}

// Returns prev element in the list
list_node_t* list_prev(list_node_t* node)
{
    /* IMPLEMENT THIS */
    return node->prev;
}

// Returns end of the list marker
list_node_t* list_end(list_t* list)
{
    /* IMPLEMENT THIS */
    return NULL;
}

// Returns data in the given list node
void* list_data(list_node_t* node)
{
    /* IMPLEMENT THIS */
    return node->data;
}

// Returns the number of elements in the list
size_t list_count(list_t* list)
{
    /* IMPLEMENT THIS */
    return list->count;
}

// Finds the first node in the list with the given data
// Returns NULL if data could not be found
list_node_t* list_find(list_t* list, void* data)
{
    /* IMPLEMENT THIS */
    for(list_node_t* curr = list->head; curr != NULL; curr = curr->next){
        if(curr->data == data){
            return curr;
        }
    }
    return NULL;
}

// Inserts a new node in the list with the given data
// Returns new node inserted
list_node_t* list_insert(list_t* list, void* data)
{
    /* IMPLEMENT THIS */
    if (list == NULL){
        return NULL;
    }
    //we need to create a node to carry the data and add to list
    list_node_t* add_node = (list_node_t*)malloc(sizeof(list_node_t));
    if (add_node == NULL){
        return NULL;
    }
    add_node->data = data;
    add_node->next = NULL;
    add_node->prev = NULL;
    //here we set it as a isolated node. insert method is seted by compare fn.

    //Case1: Compare == NULL, insert from the head.
    if (list->compare == NULL){
        if (list->head != NULL){
            list->head->prev = add_node;
            add_node->next = list->head;
            list->head = add_node;
        }
        else{
            list->head = add_node;
            list->tail = add_node;
        }
        list->count = list->count + 1;
    }
    //Case2: Compare != NULL, insert as order.
    else{
        list_node_t* curr = list->head;
        list_node_t* prev = NULL;
        while(curr!=NULL && list->compare(add_node->data,curr->data)>0){
            prev = curr;
            curr = curr->next;
        }

        add_node->next = curr;
        add_node->prev = prev;
        list->count = list->count + 1;

        //Update the head and tail.

        if (prev != NULL){
            prev->next = add_node;
            //insert to mid of list
        }
        else{
            list->head = add_node;
            //ihsert at the beginning of list
        }

        if (curr!=NULL){
            curr->prev = add_node;
        }
        else{
            list->tail = add_node;
        }

    }

    return add_node;
}

// Removes a node from the list and frees the node resources
void list_remove(list_t* list, list_node_t* node)
{
    /* IMPLEMENT THIS */
    if(list == NULL || node == NULL){
        return;
    }
    //node at the head of list
    if (node == list->head){
        list->head = node->next;
        if (list->head != NULL){
            list->head->prev = NULL;
        }
        else{
            list->tail = NULL;
        }
    }//node at the tail of list
    else if(node == list->tail){
        list->tail = node->prev;
        if(list->tail != NULL){
            list->tail->next = NULL;
        }
    }//node at the mid of list
    else{
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    free(node);
    list->count = list->count - 1;
    
}
