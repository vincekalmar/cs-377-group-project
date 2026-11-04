#ifndef PROCSTAT_H
#define PROCSTAT_H

#include "types.h"


struct pstats {
    uint64 pid;

    uint64 created;           // creation tick
    uint64 firstRun;          // first RUNNING tick
    uint64 finished;          // finish time

    uint64 runnable;    
    uint64 running;
};

int waitstat(struct pstats *);

#endif
