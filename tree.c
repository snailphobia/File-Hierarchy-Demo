#include "utils.h"

int8_t srcDir(File* file, clock_t clk) {
    if (!file) raise(SIGUSR1);

    Dir* pwd = file->root;
    return 0; // sa presupunem
}

int8_t insertFile(File* srcPos, File* file, clock_t clk) {
    if (!file) raise(SIGUSR1);
    checkTime(clk)

    if (file->root->fileR == NULL) { // trebuie pus un cap
        file->root->fileR = file;
        file->root->dirsize++;
        return 0;
    }   

    if (nameComp(srcPos->name, file->name) == 1) { // la stanga
        if (srcPos->l == NULL) {
            srcPos->l = file;
            file->orig = srcPos;
            file->root->filesize++;
            return 0;
        } else
            return insertFile(srcPos->l, file, clk); 
    }
    if (nameComp(srcPos->name, file->name) == -1) { // la dreapta
        if (srcPos->r == NULL) {
            srcPos->r = file;
            file->orig = srcPos;
            file->root->filesize++;
            return 0;
        } else
            return insertFile(srcPos->r, file, clk);
    }
    return 1; // failsafe??
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