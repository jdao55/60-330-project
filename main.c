#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int physical_memory[128][256];
int tlb_talbe[16][2];
int physical_memory_status[128];//weather spot in memory is empty
