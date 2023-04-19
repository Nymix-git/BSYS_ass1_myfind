#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

void list_files(const char *dirname, const int argc, const char *argv[])
{
    /* Open directory */
    DIR *dir = opendir(dirname);
    if (dir == NULL)
    {
        printf("myfind: '%s': File or directory not found\n", dirname);
        exit(EXIT_FAILURE);
    }

    /* Get first entry in directory */
    struct dirent *entry;
    entry = readdir(dir);

    /* Check if entry != NULL and iterate over entries and recursively iterate through directories */
    while (entry != NULL)
    {
        /* Skip over . and .. */
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            /* Create the pathname of current entry */
            char path[512] = {0};
            strcat(path, dirname);
            if (strcmp(dirname, "/") != 0)
            {
                strcat(path, "/");
            }
            strcat(path, entry->d_name);

            struct stat *entry_stats = malloc(sizeof(struct stat));
            if (entry_stats == NULL)
            {
                printf("Error: failed to allocate memory for entry_stats.\n");
                exit(EXIT_FAILURE);
            }
            lstat(path, entry_stats);

            /* This variable is set to 1 if an print or ls action is detected in the arguments. If it is 0 after all arguments are processed -print should be executed after all filters are applied */
            int print_or_ls_in_arguments = 0;
            int i = 1;
            if (argc > 1)
            {
                int broken = 0;
                /* At least one argument given */
                /* Check if a starting path was given */
                if (argv[1][0] != '-')
                {
                    i = 1;
                }
                while (i < argc)
                {
                    /* Determine if test or action is passed */
                    if (strcmp(argv[i], "-name") == 0)
                    {
                        i++;
                        if (fnmatch(argv[i], entry->d_name, 0) != 0)
                        {
                            /* Continue with next entry */
                            broken = 1;
                            break;
                        }
                    }
                    else if (strcmp(argv[i], "-type") == 0)
                    {
                        i++;

                        /* Get the type of entry */
                        int entry_type = entry_stats->st_mode & S_IFMT;

                        /* Translate the input character to inode file type number */
                        int type;
                        switch (argv[i][0])
                        {
                        case 'b':
                            type = S_IFBLK;
                            break;
                        case 'c':
                            type = S_IFCHR;
                            break;
                        case 'd':
                            type = S_IFDIR;
                            break;
                        case 'p':
                            type = S_IFIFO;
                            break;
                        case 'f':
                            type = S_IFREG;
                            break;
                        case 'l':
                            type = S_IFLNK;
                            break;
                        case 's':
                            type = S_IFSOCK;
                            break;
                        default:
                            type = 0;
                            break;
                        }

                        if (entry_type != type)
                        {
                            /* Continue with next entry */
                            broken = 1;
                            break;
                        }
                    }
                    else if (strcmp(argv[i], "-user") == 0)
                    {
                        i++;

                        /* Translate the input username */
                        struct passwd *pw = getpwnam(argv[i]);

                        if (pw->pw_uid != entry_stats->st_uid)
                        {
                            /* Continue with next entry */
                            broken = 1;
                            break;
                        }
                    }
                    else if (strcmp(argv[i], "-print") == 0)
                    {
                        print_or_ls_in_arguments = 1;
                        /* -print the current entry */
                        printf("%s/%s\n", dirname, entry->d_name);
                    }
                    else if (strcmp(argv[i], "-ls") == 0)
                    {
                        print_or_ls_in_arguments = 1;
                        // TODO: Change this to -ls print
                        /* -ls print the current entry */
                        printf("-ls:\t%s/%s\n", dirname, entry->d_name);
                    }
                    i++;
                }
                if (print_or_ls_in_arguments == 0 && broken == 0)
                {
                    printf("%s/%s\n", dirname, entry->d_name);
                }
                
            }
            else
            {
                /* No tests or actions given, print the entry */
                printf("%s/%s\n", dirname, entry->d_name);
            }

            /* If entry is a directory recursive call to iterate over its content */
            if (entry->d_type == DT_DIR)
            {
                list_files(path, argc, argv);
            }
        }
        /* Next entry */
        entry = readdir(dir);
    }

    closedir(dir);
}