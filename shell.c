#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

#define INPUT_STRING_SIZE 80

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"

   int cmd_quit(tok_t arg[]) {
      printf("Bye\n");
      exit(0);
      return 1;
   }

   int cmd_help(tok_t arg[]);
   char *currentDirectory;


/* Command Lookup table */
   typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
   typedef struct fun_desc {
      cmd_fun_t *fun;
      char *cmd;
      char *doc;
   } fun_desc_t;

   int cmd_changeDirectory(tok_t arg[]){
   
      if	(chdir(arg[0]) == -1) {
         printf("cd: %s: no such directory\n", arg[0]);
         return -1; 
      }
   
      return 0;
   }

   fun_desc_t cmd_table[] = {
   {cmd_help, "?", "show this help menu"},
   {cmd_quit, "q", "quit the command shell"},
   {cmd_changeDirectory, "cd", "Change directory"}
   };

   int cmd_help(tok_t arg[]) {
      int i;
      for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
         printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
      }
      return 1;
   }

   int lookup(char cmd[]) {
      int i;
      for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
         if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) 
            return i;
      }
      return -1;
   }

   void init_shell()
   {
   /* Check if we are running interactively */
      shell_terminal = STDIN_FILENO;
   
   /** Note that we cannot take control of the terminal if the shell
      is not interactive */
      shell_is_interactive = isatty(shell_terminal);
   
      if(shell_is_interactive){
      
      /* force into foreground */
         while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
            kill( - shell_pgid, SIGTTIN);
      
         shell_pgid = getpid();
      /* Put shell in its own process group */
         if(setpgid(shell_pgid, shell_pgid) < 0){
            perror("Couldn't put the shell in its own process group");
            exit(1);
         }
      
      /* Take control of the terminal */
         tcsetpgrp(shell_terminal, shell_pgid);
         tcgetattr(shell_terminal, &shell_tmodes);
      }
      /** YOUR CODE HERE */
      
      else{
         printf("Shell not Interactive");
         exit(EXIT_FAILURE);
      }
   }

/**
 * Add a process to our process list
 */
   void add_process(process* p)
   {
   /** YOUR CODE HERE */
   }

/**
 * Creates a process given the inputString from stdin
 */
   process* create_process(char* inputString)
   {
   /** YOUR CODE HERE */
   
   
      return NULL;
   }

   int shell (int argc, char *argv[]) {
      char *s = malloc(INPUT_STRING_SIZE+1);			/* user input string */
      tok_t *t;		/* tokens parsed from input */
      int lineNum = 0;
      int fundex = -1;
      pid_t pid = getpid();		/* get current processes PID */
      pid_t ppid = getppid();	/* get parents PID */
      pid_t cpid, tcpid, cpgid;
      int *status=NULL;
   
      init_shell();
   
      printf("%s running as PID %d under %d\n",argv[0],pid,ppid);
   
      lineNum=0;
      fprintf(stdout, "%s: ", getcwd(currentDirectory, 1024));
      while ((s = freadln(stdin))){
         t = getToks(s); /* break the line into tokens */
         fundex = lookup(t[0]); /* Is first token a shell literal */
         if(fundex >= 0) cmd_table[fundex].fun(&t[1]);
         else {
         // fork and execute ;
            char *envp;
            pid_t child_pid = fork();
         
            if (child_pid == 0 ){
               envp = getenv("PATH");
               execvp(*t,t);
               exit(0);
            }
            else if(child_pid < 0){
               printf("Child: Failed");
               exit(1);
            }
            else
               while(child_pid !=	wait(&status));
         
         
         
         /*	if (child_pid == 0){  
         
         if ((execve(*t, t, getenv("PATH"))) == -1){
         printf("Command not found\n");
         }
         
         }
         */
         
         }
         fprintf(stdout, "%s: ", getcwd(currentDirectory, 1024));
      // printf("%s\n", getenv("PATH"));
      }
   
   
   
      free(status);
      status = NULL;
      return 0;
   }
