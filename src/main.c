#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "lruStack.h"

//returns idex to 1st free 256 block of memory
int find_memory_space(int mem_status[128]);

//loads memory from backing store updates page table
void load_memory(int page_num,lru_stack* ptablem,int mem_status[],char phys_memory[][256]);

//check if node with page_num is in lru stack
//return frame# on success 0 otherwise
int check_lru(int page_num, lru_stack *tlb);//TODO push found entries to top

void print_output(int addr,int frame, int offset, char memory[][256]);
void print_output_stats(int tlb_hit, int page_table_miss);

int main()
{
    FILE * address_file=fopen("addresses.txt", "r");
    char physical_memory[128][256];
    int physical_memory_status[128];//whether spot in memory is empty
    size_t i, page_fault_count=0, tlb_hit_count=0;
    int address, offset,page_num, frame;

    lru_stack *tlb_table=construct_lru(16);
    lru_stack *page_table=construct_lru(128);
    int err;
    for(i=0;i<1000;i++)
    {
        err=fscanf(address_file,"%d\n", &address);
        if(err<=0) perror("Error with file IO");
        offset=address & 0xFF;
        page_num=address>>8;
        //check tlb
        if((frame=check_lru(page_num, tlb_table))<0)
        {
            print_output(address, frame, offset,physical_memory);
            tlb_hit_count++;
        }
        //check page table
        else if((frame =check_lru(page_num, page_table))<0)
        {
            print_output(address, frame, offset,physical_memory);
            push(tlb_table, construct_node(page_num, frame, NULL, NULL));
        }
        //load from backing sore update page table and tlb
        else
        {
            load_memory(page_num, page_table, physical_memory_status, physical_memory);
            //update tlb
            push(tlb_table, page_table->front);
            page_fault_count++;
            print_output(address, page_table->front->frame_number, offset,physical_memory);
        }
    }
    print_output_stats(tlb_hit_count, page_fault_count);
}

int find_memory_space(int mem_status[128])
{
    for(int i=0;i<128;i++)
    {
        if (mem_status[i]==0)
            return i;
    }
    return -1;
}

void load_memory(int page_num,lru_stack* ptable,int mem_status[],char phys_memory[][256])
{
    FILE * backing_store=fopen("BACKING_STORE.bin", "r");
    char buffer[256];
    int mem_space;
    int err;
    err=fseek(backing_store, page_num*256, SEEK_SET);
    err=fread(buffer,1, 256,backing_store);
    if(err<256)
        perror("Error with file IO");
    fclose(backing_store);
    
    //find free memspace if one exists
    if((mem_space = find_memory_space(mem_status)) < 0)
        mem_space=ptable->back->frame_number;
        
    //set space to occupied
    mem_status[mem_space]=1;
    //copy to phsy mem
    memcpy(phys_memory[mem_space], buffer, 256);
    //update page table

    node * new_node=construct_node(page_num, mem_space, NULL, NULL);
    push(ptable, new_node);
}

int check_lru(int page_num, lru_stack *tlb)
{
    node* frame;
    if((frame=find(tlb, page_num))!=NULL)
    {
        move_to_top(tlb, frame);
        return frame->frame_number;
    }
    else
        return -1;
}

void print_output(int addr,int frame, int offset, char memory[][256])
{
    int phys_addr=(frame<<8)|offset;
    printf("Virtual address: %d Physical address: %d Value: %d", addr, phys_addr, memory[frame][offset]);
}

void print_output_stats(int tlb_hit, int page_table_miss)
{
    printf("TLB HITS: %d page miss: %d",tlb_hit,page_table_miss);
}
