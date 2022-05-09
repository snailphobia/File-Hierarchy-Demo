#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#ifndef UTILS
#define UTILS

#define SIGUSR1 10
#define SIGUSR2 12
#define SIGUSR3 21
#define __S1__  256
#define timer(s) s = clock();
#define checkTime(s) if (clock() - s > CLOCKS_PER_SEC * 5) raise(SIGUSR3);

typedef struct _dir Dir;
typedef struct _file File;

typedef struct _file{
    char* name; // S1
    Dir* root; // nu se aloca niciodata memorie aici
               // dir root pentru fisierul curent
    File* orig; // nu se aloca niciodata memorie aici
                // parinte pentru fisierul curent
    File* l, * r; // initial NULL
} File;

typedef struct _dir{
    char* name; // S1
    Dir* orig; // nu se aloca niciodata memorie aici
    File* fileR; // initial NULL
    Dir* dirR; // initial NULL
    Dir* l, * r; // initial NULL
    int32_t dirsize, filesize; // marimile arborilor
} Dir;

typedef struct _fs{
    Dir* root;
    // aici teoretic ar mai intra date despre FS
} FILESYSTEM;

// utils
FILESYSTEM* crFS();
Dir* crDir(char* name, Dir* orig);
File* crFile(char* name, Dir* orig);
int8_t nameComp(const char* A, const char* B);
void parser();

// tree
int8_t srcDir(File* file, clock_t clk);
int8_t insertFile(File* srcPos, File* file, clock_t clk);
void printDir(Dir* dir, clock_t clk);
void printFile(File* file, clock_t clk);
#endif