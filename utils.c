#include "utils.h"

void memAbort() {
    fprintf(stderr, "Warning: failed to allocate memory /"
                    " ptr is NULL when not supposed to be\n"
                    "exiting . . .\n");
    exit(-1);
}

void readErr() {
    fprintf(stderr, "Warning: failed to read desired number of arguments\n");
    return;
}

void timeLimit() {
    fprintf(stderr, "Warning: TLE\n");
    exit(1);
}

FILESYSTEM* crFS() {
    FILESYSTEM* fs = calloc(1, sizeof(FILESYSTEM));
    if (!fs)
        raise(SIGUSR1);
    
    fs->root = crDir("root", NULL);
    return fs;
}

Dir* crDir(char* name, Dir* orig) {
    Dir* dir = calloc(1, sizeof(Dir));
    if (!dir)
        raise(SIGUSR1);
    
    dir->name = calloc(__S1__, 1);
    if (!dir->name)
        raise(SIGUSR1);
    snprintf(dir->name, __S1__, "%s", name);
    dir->orig = orig;
    dir->dirsize = dir->filesize = 0;
    dir->fileR = NULL; dir->dirR = NULL;
    dir->l = dir->r = NULL;
    return dir;
}

File* crFile(char* name, Dir* root) {
    File* file = calloc(1, sizeof(File));
    if (!file)
        raise(SIGUSR1);

    file->name = calloc(__S1__, 1);
    if (!file->name)
        raise(SIGUSR1);
    snprintf(file->name, __S1__, "%s", name);
    file->root = root;
    file->l = file->r = NULL;
    file->orig = NULL;
    return file;
}

int8_t nameComp(const char* A, const char* B) {
    if (strcmp(A, B) > 0) return 1;
    if (strcmp(A, B) < 0) return -1;
    return 0;
}

void parser() {
    char* cmd = calloc(__S1__, 1), * args = calloc(__S1__, 1);
    int8_t sgn = 0;
    FILESYSTEM* fs = crFS(); Dir* pwd = fs->root;
    clock_t clk = 0;

    while (strcmp(cmd, "quit")) {
        sgn = scanf("%s", cmd);
        if (sgn < 1) raise(SIGUSR2);

        if (!strcmp(cmd, "touch")) {
            sgn = scanf("%s", args);
            if (sgn < 1) raise(SIGUSR2);
            
            timer(clk)
            File* file = crFile(args, pwd);

            if (!srcDir(file, clk))
                insertFile(pwd->fileR, file, clk); // nu a gasit fisierul
            else {
                // elibereaza fisierul creat
                free(file->name);
                free(file);
            }

        }
        if (!strcmp(cmd, "find")); // TODO
        if (!strcmp(cmd, "mkdir")); // TODO
        if (!strcmp(cmd, "ls")) {
            timer(clk)
            printDir(pwd->dirR, clk);
            printFile(pwd->fileR, clk);
            printf("\n");
        }
        if (!strcmp(cmd, "cd")); // TODO
        if (!strcmp(cmd, "pwd")); // TODO
        if (!strcmp(cmd, "rm")); // TODO
        if (!strcmp(cmd, "rmdir")); // TODO
    }
    
    free(cmd); free(args);
    return;
}
