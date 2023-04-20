#define _DEFAULT_SOURCE

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <fnmatch.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <sys/stat.h>

/**
 * @brief This function implements the functionality of the "find" command line utility. Before calling this function the arguments must be validated and the starting directory must be evaluated. This function calls itself recursively for each directory found.
 *
 * @param dirname Path of the starting directory
 * @param argc Argument counter, should be passed from main
 * @param argv Argument array, should be passed from main. It is important to validate the arguments first since this function does assume that all arguments are valid
 */
void list_files(char *dirname, int argc, char **argv)
{
    /* Open directory */
    DIR *dir = opendir(dirname);
    if (dir == NULL)
    {
        printf("Error: '%s': File or directory not found\n", dirname);
        exit(EXIT_FAILURE);
    }

    /* Get first entry in directory */
    struct dirent *entry;
    entry = readdir(dir);

    /* Check if entry != NULL and iterate over entries and recursively iterate through directories */
    while (entry != NULL)
    {
        /* Skip over .. */
        if (strcmp(entry->d_name, "..") != 0)
        {
            /* Create the pathname of current entry */
            char path[512] = {0};
            strcat(path, dirname);
            if (strcmp(dirname, "/") != 0 && strcmp(entry->d_name, ".") != 0)
            {
                strcat(path, "/");
            }
            if (strcmp(entry->d_name, ".") != 0)
            {
                strcat(path, entry->d_name);
            }

            /* Get the stats of the current entry */
            struct stat *entry_stats = malloc(sizeof(struct stat));

            if (entry_stats == NULL)
            {
                printf("Error: Failed to allocate memory for entry_stats\n");
                exit(EXIT_FAILURE);
            }
            lstat(path, entry_stats);

            /* This variable is set to 1 if an print or ls action is detected in the arguments. If it is 0 after all arguments are processed -print should be executed after all filters are applied */
            int print_or_ls_in_arguments = 0;
            int i = 1;
            if (argc > 1)
            {
                /* This variable is used to detect if the current entry does not meet the requirements of the tests */
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
                    else if (strcmp(argv[i], "-print") == 0 && strcmp(entry->d_name, ".") != 0)
                    {
                        print_or_ls_in_arguments = 1;
                        /* -print the current entry */
                        printf("%s/%s\n", dirname, entry->d_name);
                    }
                    else if (strcmp(argv[i], "-ls") == 0)
                    {
                        print_or_ls_in_arguments = 1;

                        /* -ls print the current entry */
                        struct passwd *user = getpwuid(entry_stats->st_uid);
                        struct group *group = getgrgid(entry_stats->st_gid);
                        struct passwd *pw = getpwuid(entry_stats->st_uid);

                        char date_string[20];
                        strftime(date_string, sizeof(date_string), "%b %d %H:%M", localtime(&entry_stats->st_mtime));

                        printf("%9ld %6lu ", entry_stats->st_ino, entry_stats->st_nlink);

                        // File type and permissions
                        switch (entry_stats->st_mode & S_IFMT)
                        {
                        case S_IFBLK:
                            printf("b");
                            break;
                        case S_IFCHR:
                            printf("c");
                            break;
                        case S_IFDIR:
                            printf("d");
                            break;
                        case S_IFIFO:
                            printf("p");
                            break;
                        case S_IFLNK:
                            printf("l");
                            break;
                        case S_IFREG:
                            printf("-");
                            break;
                        case S_IFSOCK:
                            printf("s");
                            break;
                        default:
                            printf("?");
                            break;
                        }
                        printf("%c%c%c%c%c%c%c%c%c  ",
                               (entry_stats->st_mode & S_IRUSR) ? 'r' : '-',
                               (entry_stats->st_mode & S_IWUSR) ? 'w' : '-',
                               (entry_stats->st_mode & S_IXUSR) ? 'x' : '-',
                               (entry_stats->st_mode & S_IRGRP) ? 'r' : '-',
                               (entry_stats->st_mode & S_IWGRP) ? 'w' : '-',
                               (entry_stats->st_mode & S_IXGRP) ? 'x' : '-',
                               (entry_stats->st_mode & S_IROTH) ? 'r' : '-',
                               (entry_stats->st_mode & S_IWOTH) ? 'w' : '-',
                               (entry_stats->st_mode & S_IXOTH) ? 'x' : '-');

                        // printf("%4lu ", entry_stats->st_uid);

                        // Get the number of hard links to the file
                        printf(" %lu ", (unsigned long)entry_stats->st_nlink);

                        // User name
                        if (user == NULL)
                        {
                            printf("%4lu ", entry_stats->st_uid);
                        }
                        else
                        {
                            printf("%-8s ", user->pw_name);
                        }

                        // Group name
                        if (group == NULL)
                        {
                            printf("%4lu ", entry_stats->st_gid);
                        }
                        else
                        {
                            printf("%-8s ", group->gr_name);
                        }

                        // File size
                        printf("%8lld ", entry_stats->st_size);

                        // Date and time
                        printf("%s ", date_string);

                        // File name
                        printf("%s\n", path);
                    }
                    i++;
                }
                if (print_or_ls_in_arguments == 0 && broken == 0 && strcmp(entry->d_name, ".") != 0)
                {
                    printf("%s/%s\n", dirname, entry->d_name);
                }
            }
            else
            {
                /* No tests or actions given, print the entry */
                if (strcmp(entry->d_name, ".") != 0)
                {
                    printf("%s/%s\n", dirname, entry->d_name);
                }
            }

            /* If entry is a directory recursive call to iterate over its content */
            if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0)
            {
                list_files(path, argc, argv);
            }
            free(entry_stats);
        }
        /* Next entry */
        entry = readdir(dir);
    }

    closedir(dir);
}