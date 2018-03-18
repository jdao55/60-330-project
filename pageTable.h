#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H
#include <stdlib.h>
typedef struct page page;
struct page
{
    int page_number;
    int frame_number;
    page *next;
    page *back;
};
typedef struct
{
    page * front;
    page * back;
    const int capacity;
    int page_count;
}page_table;
page* construct_page(int page_n, int frame_n, page* n, page *b);
void pop_bottom(page_table *table);
void push(page_table *table, page* p);
page* find(page_table* table,int page_num);
void move_to_top(page_table* table, page* p);
void delete_table_contents(page_table* table);
#endif
