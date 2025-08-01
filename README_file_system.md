## Auteur
**Tommy PRATS**  
[tommy.prats@etu.univ-grenoble-alpes.fr](mailto:tommy.prats@etu.univ-grenoble-alpes.fr)

# File System Manager

System calls related to file operations are implemented in `syscall.c` and use `lfs.h` via `file_system_manager.c`.  
We support file creation and manipulation.

## Functionality

The following functionalities are implemented and appear to work correctly:

- `open`  
- `read`  
- `write`  
- `lseek`  
- `stat`  

Documentation is available in `file_system_manager.h`.

## Not Fully Functional Yet

Initial work has been done on directory-related features, but they are not yet fully functional:

- `mkdir` (create a directory)  
- `chdir` (change directory) – **WIP**, currently not working  
- `readir` (list files in a directory)  

For some of these functions, you need to declare it in your apps because they are provided by dirent.h and not by stdlib

```c
    int readir(char *name, char ** result);
```

These functions currently only support **relative paths** to the current directory.  
Further work is needed to support **absolute paths** as well.

By default, the root of the file system is named `/` to follow the Linux standard.
