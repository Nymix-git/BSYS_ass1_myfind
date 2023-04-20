#pragma once

int exists(const char *path);

int user_exists(const char *name);

void die(const char *message);

void process_args(int argc, char **argv, char *start_dir);