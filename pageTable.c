#include "pageTable.h"

//pop bottom value off stack
void pop_bottom(page_table *table)
{
    //general case
    if(table->page_count>1)
    {
        page * end=table->back;
        table->back=end->back;
        table->back->next=NULL;
        free(end);
        table->page_count--;
    }
    //when there is 1 element
    else if(table->page_count==1)
    {
        page * end=table->back;
        table->front=NULL;
        table->back=NULL;
        free(end);
        table->page_count--;
        
    }
}

//push onto top of stack
void push(page_table* table, page* p)
{
    if(table->page_count>0)
    {
        //remove least recently used page
        if(table->page_count==table->capacity)
            pop_bottom(table);
        //push onto stack
        p->back=NULL;
        p->next=table->front;
        table->front->back=p;
        table->front=p;
        //upadate page count
        table->page_count++;
    }
    else if(table->page_count==0)
    {
        p->back=NULL;
        p->next=NULL;
        table->front=p;
        table->back=p;
        table->page_count++;
    }
}

//create new page struct
page* construct_page(int page_n, int frame_n, page* n, page*b)
{
    page* new_page=(page *)malloc(sizeof(page));
    new_page->page_number=page_n;
    new_page->frame_number=frame_n;
    new_page->next=n;
    new_page->back=b;
    return new_page;
}
//find noed in stack
page* find(page_table *table, int page_num)
{
    page * current=table->front;
    while(current)
    {
        if (current->page_number==page_num)
        {
            return current;
        }
        current=current->next;
    }
    return NULL;
}

//move node p in stack to top of stack
void move_to_top(page_table* table, page* p)
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
void delete_table_contents(page_table* table)
{
    page * current= table->front;
    page* next;
    table->front=NULL;
    table->back=NULL;

    while(current)
    {
        next=current->next;
        free(current);
        current=next;
    }
    
}
