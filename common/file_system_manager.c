/**
 * Copyright 2025 INRIA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 *  @file   common/file_system_manager.c
 *  @author Tommy PRATS 
 */
#include <string.h>
#include <stdlib.h>
#include "common/file_system_manager.h"

uint8_t **fake_flash;


int ramfs_read(const struct lfs_config *cfg, lfs_block_t block,
               lfs_off_t off, void *buffer, lfs_size_t size) {
    if (block >= BLOCK_COUNT || off + size > BLOCK_SIZE) return LFS_ERR_IO;
    memcpy(buffer, &fake_flash[block][off], size);
    return 0;
}

int ramfs_prog(const struct lfs_config *cfg, lfs_block_t block,
               lfs_off_t off, const void *buffer, lfs_size_t size) {
    if (block >= BLOCK_COUNT || off + size > BLOCK_SIZE) return LFS_ERR_IO;
    for (lfs_size_t i = 0; i < size; i++) {
        uint8_t *target = &fake_flash[block][off + i];
        *target &= ((uint8_t*)buffer)[i];
    }
    return 0;
}

int ramfs_erase(const struct lfs_config *cfg, lfs_block_t block) {
    if (block >= BLOCK_COUNT) return LFS_ERR_IO;
    memset(fake_flash[block], 0xFF, BLOCK_SIZE);
    return 0;
}

int ramfs_sync(const struct lfs_config *cfg) {
    return 0;
}


struct lfs_config cfg = {
    .read  = ramfs_read,
    .prog  = ramfs_prog,
    .erase = ramfs_erase,
    .sync  = ramfs_sync,

    .read_size = READ_SIZE,
    .prog_size = PROG_SIZE,
    .block_size = BLOCK_SIZE,
    .block_count = BLOCK_COUNT,
    .cache_size = CACHE_SIZE,
    .lookahead_size = LOOKAHEAD_SIZE,
    .block_cycles = 500,
};

int current_fd = 3;
lfs_t lfs;
my_dir_ptr current_directory = NULL;


void init_file_structure(){
    fake_flash = malloc(BLOCK_COUNT * sizeof(uint8_t *));
    for (int i = 0; i < BLOCK_COUNT; i++){
        fake_flash[i] = malloc(sizeof(uint8_t) * BLOCK_SIZE);
    }
    int err = lfs_mount(&lfs, &cfg);

    if (err) {
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }
    file_system_mkdir("/", 0);
}

my_files_ptr search_file(int fd){
    for (my_files_ptr cur = current_directory->child_files; cur != NULL; cur=cur->next){
        if (cur->fd == fd){
            return cur;
        }
    }
    return NULL;
}

lfs_file_t* get_file_by_fd(int fd){
    my_files_ptr tmp = search_file(fd);
    if (tmp){
        return &(tmp->file);
    }
    return NULL;
}

int add_file(char * name, enum lfs_open_flags true_flag ){
    for (my_files_ptr cur = current_directory->child_files; cur != NULL; cur=cur->next){
        if (strcmp(cur->name, name) == 0){
            cur->is_open = true;
            lfs_file_open(&lfs, &(cur->file), name, true_flag);
            cur->fd = current_fd++;
            return cur->fd;
        }
    }
    my_files_ptr new_node = malloc(sizeof(my_files_t));
    new_node->name = strdup(name);
    new_node->next = current_directory->child_files;
    new_node->is_open = true;
    current_directory->child_files = new_node;
    lfs_file_open(&lfs, &(current_directory->child_files->file), name, true_flag);
    current_directory->child_files->fd=current_fd++;
    return current_directory->child_files->fd;
}

int file_system_mkdir(const char *name, unsigned int mode)
{
    my_dir_ptr new_dir = malloc(sizeof(my_dir_t));
    new_dir->name = strdup(name);
    new_dir->child_files = NULL;
    new_dir->child_dir = NULL;
    new_dir->next = NULL;
    new_dir->dir = malloc(sizeof(lfs_dir_t));
    new_dir->prev_directory = current_directory;
    int result = lfs_mkdir(&lfs, name);
    if (!current_directory){
        current_directory = new_dir;
    } else{
        new_dir->next = current_directory->child_dir;
        current_directory->child_dir = new_dir;
    }
    return result;
}

int file_system_chdir( const char* path )
{
    for (my_dir_ptr cur_dir = current_directory->child_dir; cur_dir != NULL; cur_dir = cur_dir->next){
        if ( strcmp(cur_dir->name, path) == 0){
            lfs_dir_close(&lfs, current_directory->dir);
            current_directory = cur_dir;
            return lfs_dir_open(&lfs, cur_dir->dir , path);
        }
    }
    return -1;
}

int file_system_readir(char *name, char ** result)
{
    if (strcmp(name, current_directory->name) == 0) {
        int i = 0;
        for (my_files_ptr cur_file = current_directory->child_files; cur_file != NULL; cur_file = cur_file->next){
            result[i] = strdup(cur_file->name);
            i++;
        }
        for (my_dir_ptr cur_dir = current_directory->child_dir; cur_dir != NULL; cur_dir = cur_dir->next){
            result[i] = strdup(cur_dir->name);
            i++;
        }
        result[i] = NULL;
    }

    return 0;
}

int remove_file(int fd){
    for (my_files_ptr cur = current_directory->child_files; cur != NULL; cur=cur->next){
        if (cur->fd == fd){
            lfs_file_close(&lfs, &(cur->file));
    //        prev->next = cur->next;
            int result = cur->fd;
            free(cur->name);
            free(cur);
            return result;
        }
    }
    return -1;
}


int close_file(int fd){
    for (my_files_ptr cur = current_directory->child_files; cur != NULL; cur=cur->next){
        if (cur->fd == fd){
            cur->is_open = false;
            lfs_file_close(&lfs, &(cur->file));
            return -1;
        }
    }
    return -1;
}
