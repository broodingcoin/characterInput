#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

struct pf_prev {
  long PF_prev;
  long PFr_prev;
};

int printCustomCommands(char** commands, int commandCounter);
int stats(struct rusage* usage, clock_t start, clock_t end, struct pf_prev* totals);
int runcommand(char **commands, char* op, struct rusage* usage, struct pf_prev* totals);
