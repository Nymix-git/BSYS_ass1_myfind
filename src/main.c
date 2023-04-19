#include "iterator.h"
#include "validator.c"

#define MAX_ARGS 255

int main(int argc, char *argv[])
{
    char start_dir[MAX_ARGS];

    process_args(argc, argv, start_dir);

    list_files(start_dir, argc, argv);

    if (argc > 1)
    {
        int filter_or_action = 0;

        // zuerst herausfinden, ob argv[1] ein Filter oder eine Aktion ist, sonst ist es der Startverzeichnis

        if (argv[1][0] != '-')
        {
            // Startverzeichnis = argv[1]
        }
        else
        {
            filter_or_action = 1;

            // Startverzeichnis = aktuelles Verzeichnis
        }

        // herausfinden, ob es Filter und/oder Aktionen gib

        if (filter_or_action == 1 || argc > 2)
        {
            // zusätzliche Filter und Aktionen anwenden
        }
    }
    else
    {
        // find in aktuellen verzeichnis, ohne spezifische Filter und Aktionen

        // list_files(".");
    }
    return 0;
}
