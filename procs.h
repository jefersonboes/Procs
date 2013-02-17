#ifndef PROCS_H
#define PROCS_H

class Procs
{
public:
    Procs();
};

#include <dirent.h>

typedef struct {
    char key[64];
    char value[256];
} key_value;

typedef struct {
    key_value kv;
    void *next;
} status;

typedef struct {
    DIR *dir;    
} procs ;

typedef struct {
    int pid;
    char name[256];
    status *st;
    unsigned int st_size;
} proc ;

procs *openProcs();
void closeProcs(procs *p);
bool nextProc(procs *p, proc &pr);

#endif // PROCS_H
