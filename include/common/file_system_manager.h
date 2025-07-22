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
 *  @file   include/common/file_system_manager.h
 *  @author Tommy PRATS 
 */
#ifndef __FILE_SYSTEM_MANAGER__
#define __FILE_SYSTEM_MANAGER__

#include "common/lfs.h"

extern lfs_t lfs;
typedef struct my_files {
    struct my_files * next;
    char * name;
    lfs_file_t file;
    int fd;
} my_files_t;

typedef my_files_t * my_files_ptr;

// Configuration of the virtual peripheric
#define BLOCK_SIZE     512   // Taille d'un bloc
#define BLOCK_COUNT    128   // Nombre total de blocs
#define READ_SIZE      16    // Taille minimale d'une lecture
#define PROG_SIZE      16    // Taille minimale d'une écriture
#define CACHE_SIZE     16
#define LOOKAHEAD_SIZE 16

/**
 * @brief Must need to call this function at the beginning to create the file system
 */
void init_file_structure();

/**
 * @brief Functions we need to provide for littlefs 
 */
int ramfs_read(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
/**
 * @brief Functions we need to provide for littlefs 
 */
int ramfs_prog(const struct lfs_config *cfg, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) ;
/**
 * @brief Functions we need to provide for littlefs 
 */
int ramfs_erase(const struct lfs_config *cfg, lfs_block_t block);
/**
 * @brief Functions we need to provide for littlefs 
 */
int ramfs_sync(const struct lfs_config *cfg);

/**
 * @brief find A file in the linked list
 *
 * @param fd file descriptor 
 * @return A pointer on internal structure with fd, name and file from lfs
 */
my_files_ptr search_file(int fd);

/**
 * @brief get structure file for lfs  
 *
 * @param fd file descriptor 
 * @return A pointer on a file from lfs 
 */
lfs_file_t* get_file_by_fd(int fd);

/**
 * @brief inser a new node in linked list with all open file 
 *
 * @param name name of the file
 * @param true_flag Mode for read, write, create
 * @return a file descriptor 
 */
int add_file(char * name, enum lfs_open_flags true_flag );

/**
 * @brief close a file in our list 
 *
 * @param fd file descriptor
 * @return file_descriptor
 */
int remove_file(int fd);

#endif //__FILE_SYSTEM_MANAGER__
