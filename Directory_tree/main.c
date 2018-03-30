#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <stdbool.h>

void printFile(const char* filename, int spaceCount) {
    for (int i = 0; i < spaceCount; i++){
        printf("-");
    }
    printf("--| %s\n", filename);
}

int myFtw(const char* path, int spaceCount) {
    struct stat st;
    struct dirent* dentry;
    DIR* dir;

    if (lstat(path, &st) < 0) {
        printf("lstat error on %s\n", path);
        return -1;
    }

    if (!S_ISDIR(st.st_mode)) {
        return 0;
    }

    if (!(dir = opendir(path))) {
        printf("Cannot open the directory \n");
        return -1;
    }

    // устанавливаем текущий каталог
    chdir(path);
    // читаем оглавление каталога по 1 файлу за раз
    while ((dentry = readdir(dir)) != NULL) {
        if (strcmp(dentry->d_name, ".") && strcmp(dentry->d_name, "..")) {
            printFile(dentry->d_name, spaceCount);
            myFtw(dentry->d_name, spaceCount+4);
        }
    }

    chdir("..");
    closedir(dir);

    return 0;
   
}

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        myFtw(".", 0);
    } else if (argc == 2) {
        if (myFtw(argv[1], 0)) {
            printf("Directory not found\n");
        }
    }
    return 0;
}


