#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <dirent.h>

void list_files(const char *dirname)
{
    DIR *dir = opendir(dirname);

    if (dir == NULL)
    {
        printf("Error opening the directory!\n");
        return;
    }

    struct dirent *entry;
    entry = readdir(dir);

    while (entry != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, ".."))
        {
            printf("%s/%s\n", dirname, entry->d_name);
            if (entry->d_type == DT_DIR)
            {
                char path[512] = {0};
                strcat(path, dirname);
                if (strcmp(dirname, "/") != 0) {
                strcat(path, "/");
                }
                strcat(path, entry->d_name);
                list_files(path);
            }
        }
        entry = readdir(dir);
    }

    closedir(dir);
}