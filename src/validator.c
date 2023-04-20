#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <dirent.h>

#define MAX_ARGS 255

/* Verify if a file or directory exists */
int exists(const char *path)
{
    struct stat buffer;
    return stat(path, &buffer) == 0;
}

/* Verify if a user exists */
int user_exists(const char *name)
{
    struct passwd *pw = getpwnam(name);
    return pw != NULL;
}

/* Print an error message and exit the program */
void die(const char *message)
{
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

/* Process the command-line arguments */
/* Process arguments passed to myfind(1) */
void process_args(int argc, char **argv, char *start_dir)
{
    /* Define valid test and action options */
    const char *valid_tests[] = {"-name", "-type", "-user"};
    const char *valid_actions[] = {"-print", "-ls"};

    /* Define valid file types for the -type test */
    const char valid_types[] = {'f', 'd', 'l', 's', 'p', 'c', 'b'};

    int i = 1, test_count = 0, action_count = 0;
    char *tests[MAX_ARGS];
    char *actions[MAX_ARGS];

    /* Determine the starting point */
    if (argc > 1 && argv[1][0] != '-')
    {
        strcpy(start_dir, argv[1]);
        i++;
    }
    else
    {
        strcpy(start_dir, ".");
    }

    /* Parse individual actions and tests */
    for (; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            /* Parse a test */
            if (strcmp(argv[i], "-name") == 0 ||
                strcmp(argv[i], "-type") == 0 ||
                strcmp(argv[i], "-user") == 0)
            {
                if (i + 1 >= argc)
                {
                    die("Missing argument for test");
                }
                tests[test_count++] = argv[i];
                tests[test_count++] = argv[++i];

                /* Verify the test argument is valid */
                if (strcmp(argv[i - 1], "-type") == 0)
                {
                    int j, valid = 0;
                    for (j = 0; j < sizeof(valid_types); j++)
                    {
                        if (valid_types[j] == argv[i][0])
                        {
                            valid = 1;
                            break;
                        }
                    }
                    if (!valid)
                    {
                        die("Invalid file type");
                    }
                }
                else if (strcmp(argv[i - 1], "-user") == 0)
                {
                    if (!user_exists(argv[i]))
                    {
                        die("User does not exist");
                    }
                }
                continue;
            }
            /* Parse an action */
            else if (strcmp(argv[i], "-print") == 0 ||
                     strcmp(argv[i], "-ls") == 0)
            {
                actions[action_count++] = argv[i];
                continue;
            }
            /* Invalid argument */
            else
            {
                die("Invalid argument");
            }
        }
        else
        {
            die("Invalid argument");
        }
    }
}
