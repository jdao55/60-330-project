#ifndef LRU_STACK_H
#define LRU_STACK_H
#include <stdlib.h>
typedef struct node node;
struct node
{
    int page_number;
    int frame_number;
    node *next;
    node *back;
};
typedef struct
{
    node * front;
    node * back;
    int capacity;
    int node_count;
}lru_stack;

node* construct_node(int node_n, int frame_n, node* n, node *b);
lru_stack* construct_lru(int cap);
void pop_bottom(lru_stack *table);
void push(lru_stack *table, node* p);
node* find(lru_stack* table,int node_num);
void move_to_top(lru_stack* table, node* p);
void delete_table_contents(lru_stack* table);
#endif
