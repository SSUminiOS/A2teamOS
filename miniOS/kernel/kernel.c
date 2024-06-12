#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "system.h"
#include "memory_management.h"
#include "scheduling.h"
#include "file_system.h"

void print_minios(char* str);

int main() {
    print_minios("[MiniOS SSU] Hello, World!");

    char *input;
    int fs_mode = 0;
    init_partitions();
    disk_init();

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
            Process empty_process = create_default_process();
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
        else if (strcmp(input, "file system") == 0) {
            fs_mode = 1;
            disk_init();
            fs_format();
            fs_mount();
            printf("Entered file system mode. \nAvailable commands: create <name>, write <name>, read <name>\n");
        } 
        else if (fs_mode) {
            if (strcmp(input, "format") == 0) {
                fs_format();
                printf("File system formatted.\n");
            } else if (strcmp(input, "mount") == 0) {
                fs_mount(NULL);  // 메모리 디스크를 사용하기 때문에 경로는 필요 없음
                printf("File system mounted.\n");
            } else if (strncmp(input, "create ", 7) == 0) {
                fs_create(input + 7); //포인터 연산 7번째부터 읽음
                printf("File '%s' created.\n", input + 7);
            } else if (strncmp(input, "read ", 5) == 0) {
                char buf[BLOCK_SIZE];
                fs_read(input + 5, buf, BLOCK_SIZE, 0);
                printf("File content: %s\n", buf);
            } else if (strncmp(input, "write ", 6) == 0) {
                char buf[BLOCK_SIZE];
                printf("Enter content to write: ");
                fgets(buf, BLOCK_SIZE, stdin);
                buf[strcspn(buf, "\n")] = '\0';  // 개행 문자 제거
                fs_write(input + 6, buf, strlen(buf) + 1, 0);
                printf("Content written to file '%s'.\n", input + 6);
            } else if (strcmp(input, "close") == 0) {
                fs_unmount();
                fs_mode = 0;
                printf("Exit file system mode. \n");
            } else if (strcmp(input, "disk") == 0) { //테스트용
                disk_print(); 
            }
            else {
                printf("Unknown command: %s\n", input);  
            }
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
