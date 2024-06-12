#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define BLOCK_SIZE 1024
#define NUM_BLOCKS 10
#define INODE_TABLE_SIZE 20

typedef struct {
    int file_size;
    int blocks;
    char name[10]; //파일 이름
} Inode;

void disk_init(); //초기화
int disk_read(int block, char *data); // 디스크 블록 읽기
int disk_write(int block, const char *data); //디스크 블록 쓰기
int disk_close();
void disk_print();

int fs_format(); 
int fs_mount();
int fs_create(const char *name);
int fs_read(const char *name, char *buf, size_t size, off_t offset);
int fs_write(const char *name, const char *buf, size_t size, off_t offset);
int fs_unmount();

#endif