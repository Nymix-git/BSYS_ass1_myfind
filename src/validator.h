//Code geschrieben von Jakob Gridl und Samuel Bißmann
//Teamarbeit über Github

//Validator.c und -ls-Funktion von Samuel Bißmann
//Iterator und Main.c von Jakob Gridl 

#pragma once

int exists(const char *path);

int user_exists(const char *name);

void die(const char *message);

void process_args(int argc, char **argv, char *start_dir);