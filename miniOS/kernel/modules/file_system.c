#include "file_system.h"
#include <stdio.h>
#include <string.h>

Inode inodes[INODE_TABLE_SIZE];
static char disk[NUM_BLOCKS][BLOCK_SIZE];
static int closed = 0;

void disk_init() {
    memset(disk, 0, sizeof(disk)); //디스크 배열을 0으로 초기화
}

int disk_read(int block_idx, char *data) { 
    if(closed || block_idx < 0 || block_idx >= NUM_BLOCKS) return -1;
    memcpy(data, disk[block_idx], BLOCK_SIZE); //디스크에서 data에 복사
    return 0;
}

int disk_write(int block_idx, const char *data) {
    memcpy(disk[block_idx], data, BLOCK_SIZE); //데이터에서 disk[block_idx]에 복사
    return 0;
}

int disk_close() {
    closed = 1;
    return 0;
}

void disk_print() { //inodes나 disk 배열 출력으로 테스트용
    for (int i = NUM_BLOCKS - 1; i >= 0; i--) {
        printf("Block %d: %s %d\n", i, inodes[i].name, inodes[i].blocks);
    }
}


int fs_format() { //초기화
    memset(inodes, 0, sizeof(inodes)); //아이노드 테이블 초기화
    for(int i = 0; i < INODE_TABLE_SIZE; i++) {
        inodes[i].blocks = -1; //블록을 -1로 초기화
    }
    disk_write(0, (char *)&inodes); //아이노드 테이블을 디스크에 쓰기
    return 0;
}

int fs_mount() {
    disk_read(0, (char *)inodes); //아이노드 테이블 읽기
    return 0;
}

int fs_create(const char *name) { 
    for(int i = 0; i < INODE_TABLE_SIZE; i++) {
        if(inodes[i].file_size == 0) { //빈 아이노드 찾기
            inodes[i].file_size = 1; //기본 크기 설정
            strncpy(inodes[i].name, name, sizeof(inodes[i].name) - 1);
            inodes[i].name[sizeof(inodes[i].name) - 1] = '\0';
            //첫번째 빈 블록에 할당
            for(int j = 1; j < NUM_BLOCKS; j++) {
                if (disk[j][0] == 0) {
                    inodes[i].blocks = j;
                    disk[j][0] = 1; //디스크가 사용 중임을 나타내기 위해
                    break;
                }
            }
            disk_write(0, (char *)inodes); //디스크의 1번 인덱스에 아이노드 테이블 쓰기
            return 0;
        }
    }
    return -1;
}

int fs_read(const char *name, char *buf, size_t size, off_t offset) {
    for(int i = 0; i < INODE_TABLE_SIZE; i++) {
        if(strcmp(name, inodes[i].name) == 0 && inodes[i].file_size > 0) { //파일 찾기
            int block = inodes[i].blocks;
            disk_read(block, buf + offset); //데이터 읽기
            return size;
        }
    }
    return -1;
}

int fs_write(const char *name, const char *buf, size_t size, off_t offset) {
    for(int i = 0; i < INODE_TABLE_SIZE; i++) {
        if(strcmp(name, inodes[i].name) == 0 && inodes[i].file_size > 0) {
            int block = inodes[i].blocks;
            if (block == -1) {
                for (int j = 1; j < NUM_BLOCKS; j++) {
                    if (disk[j][0] == 0) {
                        inodes[i].blocks = j;
                        block = j;
                        break;
                    }
                }
            }
            disk_write(block, buf + offset); //데이터 쓰기
            inodes[i].file_size += size; //파일 크기 업데이트
            disk_write(0, (char *)inodes); //디스크의 1번 인덱스에 아이노드 테이블 쓰기
            return size;
        }
    }
    return -1;
}

int fs_unmount() { //슈퍼블록과 아이노드를 디스크에 저장하고 디스크 닫기
    disk_write(0, (char *)inodes);
    return disk_close();
}

