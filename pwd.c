#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#define MAX_DIR_DEPTH 1000
ino_t get_ino_byname(char *filename)
{
    struct stat file_stat;
    if (0 != stat(filename, &file_stat)) {
        perror("stat");
        exit(-1);
    }
    return file_stat.st_ino;
}

char* find_name_byino(ino_t ino)
{
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    char *filename = NULL;
    if (NULL == (dp = opendir("."))) {
        fprintf(stderr, "Can't open Current Diectory\n");
        exit(-1);
    } else {
        while (NULL != (dptr = readdir(dp))) {
            if (dptr->d_ino == ino) {
                filename = strdup(dptr->d_name);
                break;
            }
        }
        closedir(dp);
    }
    return filename;

}

int main(int argc, char *argv[]) {
    char *dir_stack[MAX_DIR_DEPTH]; 
    unsigned current_depth = 0;
    for (;;) {
        ino_t current_ino = get_ino_byname(".");
        ino_t parent_ino = get_ino_byname("..");
        if (current_ino == parent_ino)
            break;
        chdir("..");
        dir_stack[current_depth++] = find_name_byino(current_ino);
        if (current_depth >= MAX_DIR_DEPTH) {
            fprintf(stderr, "Direct tree is too deep.\n");
            exit(-1);
        }
    }
    int i = current_depth - 1;
    for (i = current_depth - 1;i >= 0;i--) {
        fprintf(stdout, "/%s",dir_stack[i]);
    }
    fprintf(stdout, "%s\n",current_depth == 0 ? "/" : "");
    return 0;
}
