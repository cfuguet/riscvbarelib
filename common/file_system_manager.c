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

// Fonctions de bas niveau
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
    // Rien à faire pour la RAM
    return 0;
}


// Configuration LittleFS
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

my_files_ptr list_files = NULL;

lfs_t lfs;

uint8_t **fake_flash;

void init_file_structure(){
    fake_flash = malloc(BLOCK_COUNT * sizeof(uint8_t *));
    for (int i = 0; i < BLOCK_COUNT; i++){
        fake_flash[i] = malloc(sizeof(uint8_t) * BLOCK_SIZE);
    }
    int err = lfs_mount(&lfs, &cfg);

    if (err) {
        // Si le système n'existe pas, formater
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }
}

my_files_ptr search_file(int fd){
    for (my_files_ptr cur = list_files; cur != NULL; cur=cur->next){
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
    if (! list_files){
        list_files = malloc(sizeof(my_files_t));
    }
    my_files_ptr new_node = malloc(sizeof(my_files_t));
    new_node->name = strdup(name);
    new_node->next = list_files;
    list_files = new_node;
    lfs_file_open(&lfs, &(new_node->file), name, true_flag);
    new_node->fd=current_fd++;
    return new_node->fd;
}

int remove_file(int fd){
    for (my_files_ptr cur = list_files, prev = cur; cur != NULL; cur=cur->next){
        if (cur->fd == fd){
            prev->next = cur->next;
            lfs_file_close(&lfs, &(cur->file));
            int result = cur->fd;
            free(cur);
            return result;
        }
        prev = cur;
    }
    return -1;
}
