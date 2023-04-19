#include "iterator.h"

int main(int argc, char *argv[])
{
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

        list_files(".");
    }
    return 0;
}
