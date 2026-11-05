
#include "kernel/types.h"
#include "kernel/procstat.h"
#include "user/user.h"

#define NUM_CHILDREN 3
#define BASE_ITERATIONS 20000000


void cpu_work(int multiplier) {
  volatile uint64 result = 0;
  uint64 iterations = (uint64)BASE_ITERATIONS * multiplier;

  for (uint64 i = 0; i < iterations; i++) {
    result += (i & 7);
  }
}

int main(void) {
  int children = 0;

  // fork children with diff workloads
  for (int i = 0; i < NUM_CHILDREN; i++) {
    int pid = fork();

    if (pid < 0) {
      printf("fork failed\n");
      break;
    }

    if (pid == 0) {
      cpu_work(i + 1);
      exit(0);
    }

    children++;
  }

  printf("PID\tTurnaround\tResponse\tWaiting\tCPU\n");

  // metric collection from done children
  for (int i = 0; i < children; i++) {
    struct pstats stats;

    int pid = waitstat(&stats);

    if (pid < 0) {
      printf("waitstat failed\n");
      exit(1);
    }

    uint64 turnaround = stats.finished - stats.created;
    uint64 response = stats.firstRun - stats.created;

    printf("%lu\t%lu\t\t%lu\t\t%lu\t%lu\n",
           stats.pid,
           turnaround,
           response,
           stats.runnable,
           stats.running);
  }

  exit(children == NUM_CHILDREN ? 0 : 1);
}
