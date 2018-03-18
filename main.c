#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int physical_memory[128][256];
int tlb_table[16][2];
int physical_memory_status[128];//whether spot in memory is empty
