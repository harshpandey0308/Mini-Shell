#ifndef SHARE_H
#define SHARE_H

#define SIZE 5
#define SLOT 4

typedef struct Shared_Mem{
    char buffer[SLOT][SIZE];
    unsigned int read_index;
    unsigned int write_index;
}Shared_Mem;

#endif