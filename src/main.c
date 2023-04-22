

//Code geschrieben von Jakob Gridl und Samuel Bißmann
//Teamarbeit über Github

//Validator.c und -ls-Funktion von Samuel Bißmann
//Iterator und Main.c von Jakob Gridl 

#include "iterator.h"
#include "validator.h"

#define MAX_ARGS 255

int main(int argc, char *argv[])
{
    /* Buffer for starting directory */
    char start_dir[MAX_ARGS];

    /* Process the starting path and validate the arguments */
    process_args(argc, argv, start_dir);

    /* Calling the utility that implements the find-like functionality */
    list_files(start_dir, argc, argv);

    return 0;
}
