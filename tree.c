#include "utils.h"

int8_t srcDir(File* file, clock_t clk) {
    if (!file) raise(SIGUSR1);

    File* crtFile = file->root->fileR;
    while (crtFile != NULL) {
        checkTime(clk)
        if (nameComp(crtFile->name, file->name) == 0) {
            printf("File %s already exists!\n", file->name);
            return 1; // exista
        }
        if (nameComp(crtFile->name, file->name) == 1)
            crtFile = crtFile->l; // prea mare
        else
        if (nameComp(crtFile->name, file->name) == -1)
            crtFile = crtFile->r; // prea mic
    }

    Dir* crtDir = file->root->dirR;
    while (crtDir != NULL) {
        checkTime(clk)
        if (nameComp(crtDir->name, file->name) == 0) {
            printf("Directory %s already exists!\n", file->name);
            return 1; // exista
        }
        if (nameComp(crtDir->name, file->name) == 1)
            crtDir = crtDir->l; // prea mare
        else
        if (nameComp(crtDir->name, file->name) == -1)
            crtDir = crtDir->r; // prea mic
    }
    return 0; 
}

int8_t srcDirDir(Dir* dir, Dir* pwd, clock_t clk) {
    if (!dir) raise(SIGUSR1);

    File* crtFile = pwd->fileR;
    while (crtFile != NULL) {
        checkTime(clk)
        if (nameComp(crtFile->name, dir->name) == 0) {
            printf("File %s already exists!\n", dir->name);
            return 1; // exista
        }
        if (nameComp(crtFile->name, dir->name) == 1)
            crtFile = crtFile->l; // prea mare
        else
        if (nameComp(crtFile->name, dir->name) == -1)
            crtFile = crtFile->r; // prea mic
    }

    Dir* crtDir = pwd->dirR;
    while (crtDir != NULL) {
        checkTime(clk)
        if (nameComp(crtDir->name, dir->name) == 0) {
            printf("Directory %s already exists!\n", dir->name);
            return 1; // exista
        }
        if (nameComp(crtDir->name, dir->name) == 1)
            crtDir = crtDir->l; // prea mare
        else
        if (nameComp(crtDir->name, dir->name) == -1)
            crtDir = crtDir->r; // prea mic
    }
    return 0; 
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

int8_t insertDir(Dir* srcPos, Dir* dir, Dir* pwd, clock_t clk) {
    if (!dir) raise(SIGUSR1);
    checkTime(clk)

    dir->root = pwd;
    if (pwd->dirR == NULL) { // trebuie pus un cap
        pwd->dirR = dir;
        pwd->dirsize++;
        return 0;
    }   

    if (nameComp(srcPos->name, dir->name) == 1) { // la stanga
        if (srcPos->l == NULL) {
            srcPos->l = dir;
            dir->orig = srcPos;
            pwd->filesize++;
            return 0;
        } else
            return insertDir(srcPos->l, dir, pwd, clk); 
    }
    if (nameComp(srcPos->name, dir->name) == -1) { // la dreapta
        if (srcPos->r == NULL) {
            srcPos->r = dir;
            dir->orig = srcPos;
            pwd->filesize++;
            return 0;
        } else
            return insertDir(srcPos->r, dir, pwd, clk);
    }
    return 1; // failsafe??
}

void rmFile(const char* name, Dir* pwd, clock_t clk) {
    if (!pwd) return;
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
            File* prev = iter->orig;
            int8_t side = 0;
            if (prev != NULL) {
                if (prev->l)
                    side = 1 * (nameComp(prev->l->name, name) == 0);
                if (prev->r && !side)   
                    side = 2 * (nameComp(prev->r->name, name) == 0);
            }
            if (!iter->l && !iter->r) {
                if (prev) {
                    if (side == 1)
                        prev->l = NULL;
                    else
                    if (side == 2)
                        prev->r = NULL;
                    pwd->filesize--;
                    freeFile(iter);
                    return;
                }
            }
            if (iter->l) { // ia cea mai mare frunza mai mica decat iter
                File* repl = iter->l;
                while (repl->r) repl = repl->r;
                if (repl->orig != iter) {
                    iter->l->orig = repl;
                    repl->orig->r = NULL;
                    repl->l = iter->l;
                }
                
                    // muta legaturile pentru frunza ca sa fie inlocuita
                repl->orig = prev;
                repl->r = iter->r;
                
                if (prev) {
                    if (side == 1)
                        prev->l = repl;
                    else 
                    if (side == 2)
                        prev->r = repl;
                } else {
                    pwd->fileR = repl;
                }
                freeFile(iter);
            } else { // ia primul nod de la dreapta
                File* repl = iter->r;
                if (prev) {
                    if (side == 1)
                        prev->l = repl;
                    else
                    if (side == 2)
                        prev->r = repl;
                } else {
                    pwd->fileR = repl;
                }
                if (repl)
                    repl->orig = prev;
                
                freeFile(iter);
            }
            pwd->filesize--;
            return;
        }
    }
    printf ("File %s doesn't exist!\n", name);
    return;
}

void rmDir(const char* name, Dir* pwd, clock_t clk) {
    if (!pwd) return;
    // mai intai sterge fisierele
    while (pwd->fileR)
        rmFile(pwd->fileR->name, pwd, clk);
    
    timer(clk)
    while (pwd->dirR != NULL) {
        Dir* iter = pwd->dirR;
        checkTime(clk)
        if (iter->l) { // ia cea mai mare frunza mai mica decat iter
            Dir* repl = iter->l;
            while (repl->r) repl = repl->r;
            if (repl->orig != iter) {
                iter->l->orig = repl;
                repl->orig->r = NULL;
                repl->l = iter->l;
            }
            
            // muta legaturile pentru frunza ca sa fie inlocuita
            repl->orig = NULL;
            repl->r = iter->r;
            pwd->dirR = repl;
            
            rmDir(name, iter, clk);
            // if (iter->name)
            //     free(iter->name);
            // if (iter)
            // free(iter);
            continue;
        } else { // ia primul nod de la dreapta
            Dir* repl = iter->r;
            pwd->dirR = repl;
            if (repl)
                repl->orig = NULL;
            rmDir(name, iter, clk);
            // if (iter->name)
            //     free(iter->name);
            // if (iter)
            //     free(iter);
        }
        pwd->dirsize--;
    }
}