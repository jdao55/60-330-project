#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "lruStack.h"
#include <time.h>

//returns index to 1st free 256 block of memory
int find_memory_space(int mem_status[128]);

//loads memory from backing store updates page table
void load_memory(int page_num,lru_stack* ptablem,size_t* pr,int mem_status[],char phys_memory[][256]);

//check if node with page_num is in lru stack
//return frame# on success -1 otherwise
int check_lru(int page_num, lru_stack *tlb);//TODO push found entries to top

//prints output for single address to ofile
void print_output(int addr,int frame, int offset,char memory[][256], FILE ** ofile);

//prints excuetion results(tlb hit rate etc) to ofile
void print_output_stats(int num_addr,int tlb_hit, int page_table_miss, int stat,FILE ** ofile);

int main(int argc, char** argv)
{
    if(argc<2)
    {
        fprintf(stderr, "not enough args \n project <address file>");
		return 0;
    }
    FILE * address_file=fopen(argv[1], "r");
    FILE * out_file=fopen("ouput.txt", "w");

	if (address_file == NULL)
	{
		perror("Unable to open address file");
		return 0;
	}

	if (out_file == NULL)
	{
		perror("Unable to create output file");
		fclose(address_file);
		return 0;
	}

    //char array to simulate physical memory (2d for easier access)
    char physical_memory[128][256] = {0};
    //whether spot in memory is empty
    int physical_memory_status[128] = {0};
    size_t num_addr = 0,page_fault_count=0, tlb_hit_count=0, page_replacements=0;
    int address, offset,page_num, frame;

    lru_stack *tlb_table=construct_lru(16);
    lru_stack *page_table=construct_lru(128);

    //processs adresses from addresses.txt
    while(fscanf(address_file,"%d\n", &address)!=EOF)
    {
        offset = address & 0xFF;
        page_num = address>>8;
        num_addr++;
        //check tlb
        if((frame=check_lru(page_num, tlb_table))>=0)
        {
            node * page_node=find(page_table, page_num);
            move_to_top(page_table, page_node);
            print_output(address, frame, offset,physical_memory,&out_file);
            tlb_hit_count++;
        }
        
        //check page table
        else if((frame = check_lru(page_num, page_table))>=0)
        {
  
            print_output(address, frame, offset,physical_memory,&out_file);
            push(tlb_table, construct_node(page_num, frame, NULL, NULL));
        }
        //load from backing sore update page table and tlb
        else
        {
            load_memory(page_num, page_table, &page_replacements,physical_memory_status, physical_memory);
            //update tlb
            push(tlb_table,construct_node(page_table->front->page_number,
                                page_table->front->frame_number, NULL, NULL));
            page_fault_count++;
            print_output(address, page_table->front->frame_number, offset,physical_memory,&out_file);
            
        }
        
    }

	fclose(address_file);
	print_output_stats(num_addr,tlb_hit_count, page_fault_count,page_replacements,&out_file);

	fclose(out_file);
}

int find_memory_space(int mem_status[])
{
    for(int i=0;i<128;i++)
    {
        if (mem_status[i]==0)
            return i;
    }
    return -1;
}

void load_memory(int page_num,lru_stack* ptable, size_t *pr,int mem_status[], char phys_memory[][256])
{
    FILE * backing_store=fopen("BACKING_STORE.bin", "rb");
    char buffer[256];
    int mem_space;
    //seek to page location
    int err=fseek(backing_store, page_num*256, SEEK_SET);

    //load the page
    err=fread(buffer, 1, 256,backing_store);
    /*if(err<256)
    {
		
    }*/
    fclose(backing_store);
    
    //find free memspace if one exists
    if((mem_space = find_memory_space(mem_status)) < 0)
    {
        mem_space=ptable->back->frame_number;
        (*pr)++;
    }
        
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

void print_output(int addr,int frame, int offset, char memory[][256],FILE ** ofile)
{
    int phys_addr=(frame<<8)|offset;
    fprintf(*ofile,"Virtual address: %d Physical address: %d Value: %d\n", addr, phys_addr, memory[frame][offset]);
}

void print_output_stats(int num_addr, int tlb_hit, int page_table_miss, int stat,FILE ** ofile)
{
    int page_hit =1000-tlb_hit-page_table_miss;
    fprintf(*ofile,"Number of Translated Addresses=%d\n\
Page Faults = %d\n\
Page Fault Rate= %.3f\n\
TLB Hits = %d\n\
TLB git Rate = %.3f\n\
Page table Hits = %d\n\
Page table Hit rate= %.3f\n\
Page Relplacements = %d", num_addr,page_table_miss, page_table_miss/1000.0, tlb_hit, tlb_hit/1000.0, page_hit, page_hit/1000.00,stat);

    printf("Number of Translated Addresses=%d\n\
Page Faults = %d\n\
Page Fault Rate= %.3f\n\
TLB Hits = %d\n\
TLB Hit Rate = %.3f\n\
Page table Hits = %d\n\
Page table Hit rate= %.3f\n\
Page Replacements = %d\n", num_addr,page_table_miss, page_table_miss/1000.0, tlb_hit, tlb_hit/1000.0, page_hit, page_hit/1000.00,stat);
}
