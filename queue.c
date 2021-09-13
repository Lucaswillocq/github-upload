/* 
 * Developed by R. E. Bryant, 2017
 * Extended to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    // allocate room for the queue
    queue_t *q =  malloc(sizeof(queue_t));

    /* What if malloc returned NULL? */
    
    if(q==NULL)
      return NULL;

//Initialize head,tail,and size
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */

//If queue isn't initialized, do nothing
    if(q==NULL)
    {
      return;
    }

//Point to head of queue
    list_ele_t *p;

//iterate through queue,freeing each element and it's value until at the end
    while(q->head!=NULL)
    {
      p = q->head;
      q->head = q->head->next;
      free(p->value);
      free(p);
    }

    //free queue
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    /* What should you do if the q is NULL? */
    if(q==NULL)
      {return false;}

    // variables to point to new head
    list_ele_t *newh;
    newh = malloc(sizeof(list_ele_t));
    if(newh==NULL)
      {return false;}


    //variables to point to values
    char *head_value = malloc((strlen(s)+1)*sizeof(char));

//if malloc of new element or string returns null

    if(head_value==NULL)
      {
        free(newh);
        return false;
      }
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    strcpy(head_value, s);

    newh->value = head_value;
    newh->next = q->head;

    if(q->head==NULL)
    {q->tail = newh;}

    q->head = newh;
    q->size++;

    return true;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */

    list_ele_t *newtail = malloc(sizeof(list_ele_t));

    if(q==NULL)
      {
        free(newtail);
        return false;
      }

    if(newtail==NULL)
      {return false;}

    char *value_tail = malloc((strlen(s)+1)*sizeof(char));
    if(value_tail==NULL)
    {
      free(newtail);
      return false;
    }

    strcpy(value_tail,s);
    newtail->value = value_tail;
    newtail->next = NULL;

    if (q->tail==NULL) {
        q->head = newtail;
        q->tail = newtail;
    }
    q->tail->next = newtail;
    q->tail = newtail;
    q->size++;
    return true;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* You need to fix up this code. */
  if(q==NULL || q->head == NULL)
    {return false;}

  list_ele_t *temp = q->head;

  if(sp!=NULL)
  {
    strncpy(sp,temp->value,bufsize-1);
  }

    q->head = q->head->next;
    free(temp->value);
    free(temp);
    q->size--;
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */

  if(q==NULL)
    {return 0;}

  return q->size;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* You need to write the code for this function */
 
 //if head of queue isnt initialized or queue doesn't exist, do nothing
  if (q == NULL || q->head == NULL)
        return;

  //variables for current,previous, and next node
    list_ele_t *p = q->head;
    list_ele_t *prev = NULL;
    list_ele_t *next = p->next;

  //switch em around
    while (p->next != NULL)
    {
        p->next = prev;
        prev = p;
        p = next;
        next = p->next;
    }
    p->next = prev;
    q->tail = q->head;
    q->head = p;

}

