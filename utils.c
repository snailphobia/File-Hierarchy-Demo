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

Dir* crDir(const char* name, Dir* orig) {
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
    dir->root = NULL;
    dir->l = dir->r = NULL;
    return dir;
}

File* crFile(const char* name, Dir* root) {
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

void freeDir(Dir* pwd, clock_t clk) {
    checkTime(clk)
    if (pwd->l)
        freeDir(pwd->l, clk);
    if (pwd->r)
        freeDir(pwd->r, clk);
    if (pwd->dirsize)
        freeDir(pwd->dirR, clk);
    
    
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
        if (!strcmp(cmd, "find")) {
            char* args2 = calloc(__S1__, 1);
            sgn = scanf("%s%s", args, args2);
            if (sgn < 2) raise(SIGUSR2);

            if (!strcmp("-f", args)) {
                timer(clk)
                int8_t set = 0;
                findItemF(args2, fs->root, fs->root, clk, &set);
                if (!set)
                    printf("File %s not found!\n", args2);
            };
            if (!strcmp("-d", args)) {
                timer(clk)
                int8_t set = 0;
                findItemD(args2, fs->root, fs->root, clk, &set);
                if (!set)
                    printf("Director %s not found!\n", args2);
            };
            free(args2);
        }
        if (!strcmp(cmd, "mkdir")) {
            sgn = scanf("%s", args);
            if (sgn < 1) raise(SIGUSR2);

            timer(clk)
            Dir* dir = crDir(args, pwd);

            if (!srcDirDir(dir, pwd, clk))
                insertDir(pwd->dirR, dir, pwd, clk);
            else {
                // elibereaza directorul creat
                free(dir->name);
                free(dir);
            }
        }
        if (!strcmp(cmd, "ls")) {
            timer(clk)
            printDir(pwd->dirR, clk);
            printFile(pwd->fileR, clk);
            printf("\n");
        }
        if (!strcmp(cmd, "cd")) {
            sgn = scanf("%s", args);
            if (sgn < 1) raise(SIGUSR2);

            timer(clk)
            if (!strcmp(args, "..")) {
                if (pwd->root)
                    pwd = pwd->root;
            } else {
                Dir* it = pwd->dirR;
                int8_t foundFlag = 0;
                if (it)
                    while (it) {
                        checkTime(clk)

                        if (nameComp(it->name, args) == 0) {
                            pwd = it;
                            foundFlag = 1;
                            break;
                        }
                        if (nameComp(it->name, args) == 1)
                            it = it->l;
                        else
                        if (nameComp(it->name, args) == -1)
                            it = it->r;
                    }
                if (!foundFlag)
                    printf("Directory not found!\n");
            }
        }
        if (!strcmp(cmd, "pwd")) {
            timer(clk)
            printPath(fs->root, pwd, clk);
        }
        if (!strcmp(cmd, "rm")) {
            sgn = scanf("%s", args);
            if (sgn < 1) raise(SIGUSR2);

            timer(clk)
            rmFile(args, pwd, clk);
        }
        if (!strcmp(cmd, "rmdir")) {
            sgn = scanf("%s", args);
            if (sgn < 1) raise(SIGUSR2);

            timer(clk)
            Dir* iter = pwd->dirR, * prev = NULL;
            while (iter) {
                checkTime(clk)
                if (nameComp(iter->name, args) == 1) {
                    prev = iter;
                    iter = iter->l;
                }
                else
                if (nameComp(iter->name, args) == -1) {
                    prev = iter;
                    iter = iter->r;
                }
                else
                if (nameComp(iter->name, args) == 0) {
                    rmDir(args, iter, clk);
                    break;
                }
            }
            if (!iter) {
                printf("Directory %s doesn't exist!\n", args);
                continue;
            }
            // pana aici s-a sters continutul directorului
            // mai trebuie refacuta structura curenta a arborelui

            Dir* repl = iter, * rprev = prev;
            if (!iter->l && !iter->r) {
                if (prev == NULL)
                    pwd->dirR = NULL;
                pwd->dirsize--;
                if (prev) {
                    if (prev->r && iter) {
                        if (!nameComp(prev->r->name, iter->name))
                            prev->r = NULL;
                    }
                    if (prev->l && iter) {
                        if (!nameComp(prev->l->name, iter->name))
                            prev->l = NULL;
                    }
                }
                free(iter->name);
                free(iter);
                continue;
            }
            if (iter->l) {
                repl = iter->l; rprev = iter;
                while (repl->r) {
                    rprev = repl;
                    repl = repl->r;
                }
                if (pwd->dirR == iter)
                    pwd->dirR = repl;
                if (rprev == iter)
                    rprev->l = repl->l;
                else
                    rprev->r = repl->l;
                if (prev) {
                    if (prev->r && iter) {
                        if (prev->r && !nameComp(prev->r->name, iter->name))
                            prev->r = repl;
                    }
                    if (prev->l && iter) {
                        if (prev->l && !nameComp(prev->l->name, iter->name))
                            prev->l = repl;
                    }
                }
                repl->l = iter->l;
                repl->r = iter->r;
                if (prev == NULL)
                    pwd->dirR = repl;
                free(iter->name);
                free(iter);
            } else {
                repl = iter->r;
                rprev = iter;
                if (prev) {
                    if (prev->r && iter) {
                        if (prev->r && !nameComp(prev->r->name, iter->name))
                            prev->r = repl;
                    }
                    if (prev->l && iter) {
                        if (prev->l && !nameComp(prev->l->name, iter->name))
                            prev->l = repl;
                    }
                }
                if (prev == NULL)
                    pwd->dirR = repl;
                free(iter->name);
                free(iter);
            }
            pwd->dirsize--;
        }
    }

    // timer(clk)
    // rmDir("root", fs->root, clk);
    // if (fs->root->name)
    //     free(fs->root->name);
    // if (fs->root)
    //     free(fs->root);
    // if (fs)
    //   free(fs);
    free(cmd); free(args);
    return;
}

void printDir(Dir* dir, clock_t clk) {
    if (!dir) return;
    checkTime(clk)

    if (dir->l != NULL)
        printDir(dir->l, clk);
    printf("%s ", dir->name);
    if (dir->r != NULL)
        printDir(dir->r, clk);
    
    return;
}

void printFile(File* file, clock_t clk) {
    if (!file) return;
    checkTime(clk)

    if (file->l != NULL)
        printFile(file->l, clk);
    printf("%s ", file->name);
    if (file->r != NULL)
        printFile(file->r, clk);
    
    return;
}

void printPath(Dir* root, Dir* pwd, clock_t clk) {
    if (!pwd || !root) raise(SIGUSR1);

    Dir** path = calloc(__S1__, sizeof(Dir*)); uint8_t N = 0;
    Dir* it = pwd;
    while (it != root) {
        checkTime(clk)
        path[N] = it;
        N++;
        if (N > 192) raise(SIGUSR1);
        it = it->root;
    } // cauta pathul de la coada la cap

    printf("/root");
    for (int i = N - 1; i >= 0; i--) {
        checkTime(clk)
        printf("/%s", path[i]->name);
    } // reconstituie pathul
    // i am speed
    printf("\n");

    free(path);
    return;
}

int8_t findItemF(const char* name, Dir* root, Dir* pwd, clock_t clk, int8_t* set) {
    // pentru arborele de fisiere
    if (!pwd) return 0;
    File* iter = pwd->fileR;
    while (iter != NULL) {
        checkTime(clk)
        if (nameComp(iter->name, name) == 1)
            iter = iter->l;
        else
        if (nameComp(iter->name, name) == -1)
            iter = iter->r;
        else
        if (nameComp(iter->name, name) == 0) {
            printf("File %s found!\n", name);
            printPath(root, pwd, clk);
            *set = 1;
            return 1;
        }
    }

    // pentru urmatorul director
    findItemF(name, root, pwd->l, clk, set);
    findItemF(name, root, pwd->r, clk, set);
    pwd = pwd->dirR;
    findItemF(name, root, pwd, clk, set);
    return 0;
}

int8_t findItemD(const char* name, Dir* root, Dir* pwd, clock_t clk, int8_t* set) {
    // pentru arborele de directoare
    // e cam la fel ca la fisiere, - nu mai cauta in arborele de fisiere
    if (!pwd) return 0;
    Dir* iter = pwd->dirR;

    // pentru urmatorul director
    while (iter != NULL) {
        checkTime(clk)
        if (nameComp(iter->name, name) == 1)
            iter = iter->l;
        else
        if (nameComp(iter->name, name) == -1)
            iter = iter->r;
        else
        if (nameComp(iter->name, name) == 0) {
            printf("Director %s found!\n", name);
            printPath(root, iter, clk);
            fflush(stdout);
            *set = 1;
            return 1;
        }
    }
    findItemD(name, root, pwd->l, clk, set);
    findItemD(name, root, pwd->r, clk, set);
    pwd = pwd->dirR;
    findItemD(name, root, pwd, clk, set);
    return 0;
}

void freeFile(File* file) { 
    // doar pentru fisiere deja extrase din arbore
    free(file->name);
    free(file);
}
