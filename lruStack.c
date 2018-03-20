#include "lruStack.h"

lru_stack* construct_lru(int cap)
{
     lru_stack* new_lru=(node *)malloc(sizeof(lru_stack));
     new_lru->capacity=cap;
     new_lru->node_count=0;
     new_lru->back=NULL;
     new_lru->front=NULL;
     return new_lru;
}

//pop bottom value off stack
void pop_bottom(lru_stack *table)
{
    //general case
    if(table->node_count>1)
    {
        node * end=table->back;
        table->back=end->back;
        table->back->next=NULL;
        free(end);
        table->node_count--;
    }
    //when there is 1 element
    else if(table->node_count==1)
    {
        node * end=table->back;
        table->front=NULL;
        table->back=NULL;
        free(end);
        table->node_count--;
        
    }
}

//push onto top of stack
void push(lru_stack* table, node* p)
{
    if(table->node_count>=table->capacity)
    {
        //remove least recently used node
        if(table->node_count==table->capacity)
            pop_bottom(table);
        //push onto stack
        p->back=NULL;
        p->next=table->front;
        table->front->back=p;
        table->front=p;
        //upadate node count
        table->node_count++;
    }
    else if(table->node_count==0)
    {
        p->back=NULL;
        p->next=NULL;
        table->front=p;
        table->back=p;
    }
}

//create new node struct
node* construct_node(int node_n, int frame_n, node* n, node*b)
{
    node* new_node=(node *)malloc(sizeof(node));
    new_node->node_number=node_n;
    new_node->frame_number=frame_n;
    new_node->next=n;
    new_node->back=b;
    return new_node;
}
//find noed in stack
node* find(lru_stack *table, int node_num)
{
    node * current=table->front;
    while(current)
    {
        if (current->node_number==node_num)
        {
            return current;
        }
        current=current->next;
    }
    return NULL;
}

//move node p in stack to top of stack
void move_to_top(lru_stack* table, node* p)
{
    if((table->front != p)
       &&table->back!=p)
    {
        p->back->next=p->next;
        p->next->back=p->back;
        push(table, p);
    }
    else if(table->back==p)
    {
        //update back of table to 2nd last element and push last element
        table->back=table->back->back;
        table->back->next=NULL;
        push(table, p);
    }
}

//delete the stack
void delete_table_contents(lru_stack* table)
{
    node * current= table->front;
    node* next;
    table->front=NULL;
    table->back=NULL;

    while(current)
    {
        next=current->next;
        free(current);
        current=next;
    }
    
}
