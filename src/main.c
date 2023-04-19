#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 1024
#define MAX_USER_LENGTH 32
#define MAX_ACTIONS 10
#define MAX_TESTS 10

char* starting_point = ".";
char* actions[MAX_ACTIONS];
char* tests[MAX_TESTS];
char* test_args[MAX_TESTS];
int num_actions = 0;
int num_tests = 0;

// Print an error message and exit with failure status
void error(char* message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

// Check if a path exists and is a directory
int is_directory(char* path) {
    struct stat sb;
    if (stat(path, &sb) == -1) {
        if (errno == ENOENT) {
            return 0; // path doesn't exist
        } else {
            error("Failed to check if path is a directory");
        }
    }
    return S_ISDIR(sb.st_mode);
}

// Verify that a user exists with the given name or ID
void verify_user(char* user) {
    if (isdigit(user[0])) {
        // user is specified by ID
        struct passwd* pw = getpwuid(atoi(user));
        if (pw == NULL) {
            error("Invalid user ID");
        }
    } else {
        // user is specified by name
        struct passwd* pw = getpwnam(user);
        if (pw == NULL) {
            error("Invalid user name");
        }
    }
}

// Parse command line arguments and preprocess them for myfind
void preprocess_arguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (i == 1 && argv[i][0] != '-' && strchr(argv[i], '/') == NULL) {
            // first argument is the starting point
            starting_point = argv[i];
            if (!is_directory(starting_point)) {
                error("Starting point must be a directory");
            }
        } else if (argv[i][0] == '-' && argv[i][1] != '\0') {
            // action or test
            if (strcmp(argv[i], "-print") == 0 || strcmp(argv[i], "-ls") == 0) {
                // action
                if (num_actions >= MAX_ACTIONS) {
                    error("Too many actions");
                }
                actions[num_actions++] = argv[i];
            } else if (strcmp(argv[i], "-name") == 0 || strcmp(argv[i], "-type") == 0 || strcmp(argv[i], "-user") == 0) {
                // test
                if (i == argc - 1) {
                    error("Missing argument for test");
                }
                if (num_tests >= MAX_TESTS) {
                    error("Too many tests");
                }
                tests[num_tests] = argv[i];
                test_args[num_tests] = argv[++i];
                if (strcmp(tests[num_tests], "-user") == 0) {
                    verify_user(test_args[num_tests]);
                }
                num_tests++;
            } else {
                error("Invalid option");
            }
        } else {
            error("Invalid argument");
        }
    }
}

int main(int argc, char* argv[]) {
    preprocess_arguments(argc, argv);

    // Do something with the preprocessed arguments
    printf("Starting point: %s\n", starting_point);
    printf("Actions:\n");
    for (int i = 0; i < num_actions; i++) {
        printf("  %s\n", actions[i]);
    }
    printf("Tests:\n");
    for (int i = 0; i < num_tests; i++) {
        printf("  %s %s\n", tests[i], test_args[i]);
    }

    return 0;
}
