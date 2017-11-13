#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include "mc2.h"

int main() {

  int option_len = 1;
  char *commands[9];
  int commandCounter = 0;
  int commandno = 3;
  char* getcwdBuf = malloc(128);
  clock_t start, end;
  //the two variables are used to caculate the running times of the commands.
  char *option = NULL;
  //Page Fault variable, currently not used
  long Page_Faults;
  //Page fault (reclaimed) variable, currently not used
  long Page_Faultr;
  struct pf_prev *prev = malloc(sizeof(struct pf_prev));
  prev->PF_prev = 0;
  prev->PFr_prev = 0;
  struct rusage* usage = malloc(sizeof(struct rusage));
  size_t len = 15;
  ssize_t read;
  char *line = NULL;
  int c;
  pid_t pid;
  
  while(1){
    printf("--- Mid-Day Commander, v1 ---\n");
    printf("G'day, Commander! What command would you like to run?\n");
    printf("	0. whoami    : Prints out the result of the whoamicommand\n");
    printf("	1. last      : Prints out the result of the last command\n");
    printf("	2. ls        : Prints out the result of a listing on a user-specified path\n");
    printCustomCommands(commands, commandCounter);
    printf("	a. add command : Adds a new command to the menu.\n");
    printf("	c. change directory : Changes process working directory\n");
    printf("	e. exit      : Leave Mid-Day Commander\n");
    printf("	p. pwd       : Prints working directory\n");
    printf("Option?: ");
    getline(&option, &option_len, stdin);
    printf("\n");
    
    if(option[0] == '3') {
      runcommand(commands, option, usage, prev);
    }

    if(option[0] == '4') {
      runcommand(commands, option, usage, prev);
    }

    if(option[0] == '5') {
      runcommand(commands, option, usage, prev);
    }  

    if(option[0] == '6') {
      runcommand(commands, option, usage, prev);
    }

    if(option[0] == '7') {
      runcommand(commands, option, usage, prev);
    }

    if(option[0] == '8') {
      runcommand(commands, option, usage, prev);
    }

    if(option[0] == '9') {
      runcommand(commands, option, usage, prev);
    }

    //~~~~ Who Am I ~~~~~
    if(option[0] == '0') {
      start = clock();
      int pid = fork();
      if(pid < 0){
	fprintf(stderr, "Fork failed\n");
	exit(1);
      }else if(pid == 0){ //child process
	char *myargs[2];
	myargs[0] = strdup("whoami");
	myargs[1] = NULL;
	printf("-- Who Am I? --\n");
	execvp(myargs[0], myargs);
      }else{
	int wc = wait(NULL);
	end = clock();
	printf("\n");
	int ru = getrusage(RUSAGE_CHILDREN, usage);
	stats(usage, start, end, prev);
      }
    }
    
    //~~~~ Last ~~~~
    if(option[0] == '1') {
      start = clock();
      int pid = fork();
      if(pid < 0){
	fprintf(stderr, "Fork failed\n");
	exit(1);
      }else if(pid == 0){ //child process
	char *myargs[5];
	myargs[0] = strdup("last");
	myargs[1] = strdup("-n");
	myargs[2] = strdup("1");
	myargs[3] = strdup("-i");
	myargs[4] = NULL;
	printf("-- Last Logins --\n");
	execvp(myargs[0], myargs);
      }else{ //parent process
	int wc = wait(NULL);
	end = clock();
	printf("\n");
	int ru = getrusage(RUSAGE_CHILDREN, usage);
	stats(usage, start, end, prev);
      }
    }

    //~~~~ ls ~~~~
    if(option[0] == '2') {
      
      char **ls_args = malloc(32*(sizeof(char*)));
      char *ls_path = malloc(128*(sizeof(char*)));
      ssize_t len = 0;
      char *buf;
      int i = 0;
      int j = 0;

      printf("-- Directory Listing --\n");
      printf("Arguments?: ");
      //scanf(" %[^\n]s", ls_args);
      read = getline(&line, &len, stdin);
      printf("line is a %s\n", line);
      line[strlen(line)-1] = '\0';
      buf = strtok(line, " ;\n");

      while(buf != NULL) {
	ls_args[i] = buf;
	printf("ls_args has %s\n", ls_args[i]);	
	buf = strtok(NULL, " ;\n");
	i++;
      }
      
      ls_args[i] = '\0';


      printf("Path?: ");
      read = getline(&ls_path, &len, stdin);
      /*read = getline(&line, &len, stdin);
	line[strlen(line)-1] = '\0';
	buf = strtok(line, " ;\n");

	while(buf != NULL) {
	ls_args[j] = buf;
	printf("ls_args has %s\n", ls_args[j]);	
	buf = strtok(NULL, " ;\n");
	j++;
	}
      
	ls_path[i] = '\0';
      */
      printf("\n");
      start = clock();
      int pid = fork();
      if(pid < 0){
	fprintf(stderr, "Fork failed\n");
	exit(1);
      }else if(pid == 0){ //child process
	char *myargs[32];
	myargs[0] = strdup("ls");
	//myargs[1] = strdup(ls_args);
	for(j; i > j; j++) {
	  myargs[j + 1] = strdup(ls_args[j]);
	}
	myargs[j + 1] = strdup(ls_path);
	printf("%s, %s\n", myargs[j - 2], myargs[j + 2]);
	myargs[j + 2] = '\0';
	execvp(myargs[0], myargs);
      }else{ //parent process
	int wc = wait(NULL);
	end = clock();
	printf("\n");
	int ru = getrusage(RUSAGE_CHILDREN, usage);
	stats(usage, start, end, prev);
      }
    }

    //~~~~ Add ~~~~
    if(option[0] == 'a') {
      printf("command to add?:");
      read = getline(&line, &len, stdin);
      line[strlen(line)-1] = '\0';
      commands[commandCounter] = strdup(line);
      printf("You added %s!\n\n", commands[commandCounter]);
      commandCounter++;
    }

    //~~~~ Change Directory ~~~~
    if(option[0] == 'c') {
      printf("New Directory?:");
      char *myargs[3];
      read = getline(&line, &len, stdin);
      line[strlen(line)-1] = '\0';
      chdir(line);
      printf("Directory changed to %s\n\n", line);
    }

    //~~~~ Exit ~~~~
    if(option[0] == 'e' || feof(stdin)) {
      printf("Logging you out Commander.\n");
      return 0;
    }

    //~~~~ pwd ~~~~
    if(option[0] == 'p') {
      char* gcwd;
      gcwd = getcwd(getcwdBuf, 128);
      printf("\n-- Current Directory --\n");
      printf("%s\n\n", getcwdBuf);
    }
  }
  
  return 0;
}

int printCustomCommands(char** commands, int commandCounter){
  if(commandCounter == 0){
    return 0;
  }
  for(int i = 0; i < commandCounter; i++){
    printf("\t%d. %s \t: User added command\n", i+3, commands[i]);
  }
  return 0;
}

int stats(struct rusage* usage, clock_t start, clock_t end, struct pf_prev* prev){
  double cpu_time_used;
  long Page_Faults;
  //Page fault (reclaimed) variable, currently not used
  long Page_Faultr;

  Page_Faults = usage->ru_majflt;
  Page_Faultr = usage->ru_minflt;
  cpu_time_used = (double)(end - start) / CLOCKS_PER_SEC;
  cpu_time_used *= 1000;
  // I decided the statistics would come at the end after any of the programs are run, but we have to be able to give an error if n (variable i used for scanf) is invalid!	
  printf("-- Statistics --\n");
  printf("Elapsed time: %f milliseconds\n", cpu_time_used);
  if(Page_Faults > prev->PF_prev){
    Page_Faults -= prev->PF_prev;
  }
  if(Page_Faultr > prev->PFr_prev){
    Page_Faultr -= prev->PFr_prev;
  }
  printf("Page Faults: %ld\n", Page_Faults);
  printf("Page Faults (reclaimed): %ld\n", Page_Faultr);
  printf("\n");
  prev->PF_prev += Page_Faults;
  prev->PFr_prev += Page_Faultr;
  return 0;
}

int runcommand(char **commands,  char* op, struct rusage* usage, struct pf_prev* prev) {
  //printf("%c\n", op[0]);
  int option = op[0] % 48;
  int background = 0;
  //printf("%d\n", option);
  char *myargs[15];
  clock_t start, end;
  time_t when;
  long Page_Faults;
  long Page_Faultr;
  //printf("%d\n", option - 3);
  printf("running %s\n", commands[option - 3]);
  char *command = commands[option - 3];
  /*if(command[strlen(command) - 1] == '&'){
    background = 1;
    command = command[strlen(command) - 2];
    }*/
  start = clock();
  int pid = fork();
  //if(background = 0) {
  if(pid < 0){
    fprintf(stderr, "Fork failed\n");
    exit(1);
  }else if(pid == 0){ //child process
    char *tCreate;
    int i = 0;
    tCreate = strtok(command, " \n");
    while(tCreate != NULL) {
      myargs[i] = tCreate;
      printf("myargs has %s\n", myargs[i]);	
      tCreate = strtok(NULL, " \n");
      i++;
    }
    myargs[i] = '\0';
    printf("-- %s --\n", myargs[0]);
    execvp(myargs[0], myargs);
  }else{
    pid_t wc = waitpid(pid, NULL, 0);
    printf("pid: %d\n", wc);
    end = clock();
    printf("\n");
    int ru = getrusage(RUSAGE_CHILDREN, usage);
    stats(usage, start, end, prev);
    return 0;
  }
  
  /* if(background == 1){
    if(pid < 0){
      fprintf(stderr, "Fork failed\n");
      exit(1);
    }else if(pid == 0){ //child process
      char *tCreate;
      int i = 0;
      tCreate = strtok(command, " \n");
      while(tCreate != NULL) {
	myargs[i] = tCreate;
	printf("myargs has %s\n", myargs[i]);	
	tCreate = strtok(NULL, " \n");
	i++;
      }
      myargs[i] = '\0';
      printf("-- %s --\n", myargs[0]);
      execvp(myargs[0], myargs);
    }else{
      int wc = wait3(0 ,WNOHANG, usage);
      
      end = clock();
      printf("\n");
      int ru = getrusage(RUSAGE_CHILDREN, usage);
      stats(usage, start, end, prev);
      return 0;
    }
    }*/
 
}
