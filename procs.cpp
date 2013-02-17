#include "procs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Procs::Procs()
{
}

void trim(char *str)
{
    unsigned int i = 0, j, l = 0;

    while (i < strlen(str) && (str[i] == ' ' || str[i] == '\t' || str[i] == 10 || str[i] == 13))
        i++;

    j = strlen(str);

    while (j > i && (str[i] == ' ' || str[i] == '\t' || str[i] == 10 || str[i] == 13))
        j--;

    for (; i < j; i++) {
        str[i] = str[i];
        l++;
    }

    str[l] = 0;
}

void getKeyValue(FILE *f, key_value &kv)
{
    char str[512];
    char *key = kv.key;
    char *value = kv.value;

    fgets(str, 512, f);

    unsigned int i = 0;

    while (str[i] != ':' && i < strlen(str)) {
        *key++ = str[i];
        i++;
    }

    *key++ = 0;
    i++;
    i++;

    while (str[i] != 0x0a && i < strlen(str)) {
        *value++ = str[i];
        i++;
    }

    *value++ = 0;
}

procs *openProcs()
{
    DIR *dir;

    dir = opendir("/proc");

    if (!dir)
        return NULL;

    procs *p = (procs*) malloc(sizeof(procs));
    p->dir = dir;

    return p;
}

void closeProcs(procs *p)
{
    closedir(p->dir);
    free(p);
}

void openStatus(char *pid, proc &pr)
{
    char str[128];

    strcpy(str, "/proc/");
    strcat(str, pid);
    strcat(str, "/status");

    status *st = NULL;
    pr.st_size = 0;

    FILE *f = fopen(str, "r");
    if (f) {
        while (!feof(f)) {            
            status *nst = (status*) malloc(sizeof(status));
            key_value &kv = nst->kv;
            nst->next = NULL;
            if (!st) {
                st = nst;
                pr.st = st;
            }
            else {
                st->next = nst;
                st = nst;
            }

            getKeyValue(f, kv);

            pr.st_size++;
        }

        fclose(f);
    }    
}

bool nextProc(procs *p, proc &pr)
{
    dirent *ent = readdir(p->dir);

    if (!ent)
        return false;

    int pid = atoi(ent->d_name);

    if (pid < 0)
        return false;

    pr.pid = pid;

    openStatus(ent->d_name, pr);

    return true;
}
