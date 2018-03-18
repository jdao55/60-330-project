#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "lruStack.h"

//returns idex to 1st free 256 block of memory
int find_memory_space(int mem_status[]);

//loads memory from backing store updates page table
int load_memory(int page_num,lru_stack* ptablem,int mem_status[],FILE** backingstore);//TODO add entries to page_table

int check_tlb(int page_num, lru_stack *tlb);//TODO push found entries to top
int check_table(int page_num, lru_stack* page_table);//TODO push found etries to top

void print_output(int addr,int frame, int offset, int memory[][256]);
void print_output_stats(int tlb_hit, int page_table_miss);

int main()
{
    FILE * backingstore=fopen("backingstore.txt","r");
    FILE * address_file=fopen("addesses.txt", "r");
    int physical_memory[128][256];
    int physical_memory_status[128];//whether spot in memory is empty
    size_t i, page_fault_count=0, tlb_hit_count=0;
    int address, offset,page_num, frame;

    lru_stack *tlb_table=construct_lru(16);
    lru_stack *page_table=construct_lru(128);
    
    for(i=0;i<1000;i++)
    {
        fscanf(address_file,"%d\n", &address);
        offset=address & 0xFF;
        page_num=address>>8;

        if((frame=check_tlb(page_num, tlb_table)))
        {
            print_output(address, frame, offset,physical_memory);
            tlb_hit_count++;
        }
        else if((address =check_table(page_num, page_table)))
        {
            print_output(address, frame, offset,physical_memory);
            push(tlb_table, construct_node(page_num, frame, NULL, NULL));
        }
        else
        {
            load_memory(page_num, page_table, physical_memory_status,&backingstore);
            //update tlb
            push(tlb_table, page_table->front);
            page_fault_count++;
        }
    }
    print_output_stats(tlb_hit_count, page_fault_count);
}
