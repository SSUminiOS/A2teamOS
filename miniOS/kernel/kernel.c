#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "system.h"
#include "forkProgram.h"
#include "ipcProgram.h"
#include "calculate_pi.h"
#include "memory_management.h"
#include "round_robin.h"
#include "scheduling.h"

void print_minios(char* str);

int main() {
    print_minios("[MiniOS SSU] Hello, World!");

    char *input;
    init_partitions();

    while(1) {
        // readline을 사용하여 입력 받기
        input = readline("커맨드를 입력하세요(종료:exit) : ");

        if (strcmp(input,"exit") == 0) {
            break;
        }

        if (strcmp(input,"minisystem") == 0){
            minisystem();
        }
        else if (strcmp(input, "print memory") == 0) {
            print_memory_blocks();
        }
        else if (strcmp(input, "dyna alloc") == 0) {
            printf("Enter memory size to allocate: ");
            size_t size;
            scanf("%zu", &size);
            Process empty_process = createDefaultProcess();
            int mem_id = dyna_alloc(size, empty_process); 
            if (mem_id != -1) {
                printf("Memory allocated and ID is %d\n", mem_id);
            } else {
                printf("Memory allocation is failed!!\n");
            }
        }
        else if (strcmp(input, "dyna free") == 0) {
            printf("Enter memory ID to free: ");
            int mem_id;
            scanf("%d", &mem_id);
            dyna_free(mem_id);
        }
        else if (strcmp(input, "scheduling") == 0) {
            schedule();
        }
        else system(input);
    }

    // 메모리 해제
    free(input);
    print_minios("[MiniOS SSU] MiniOS Shutdown........");

    return(1);
}

void print_minios(char* str) {
        printf("%s\n",str);
}
