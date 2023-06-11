/* Author: Chidubem Jean-Medard Udeh*/
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "linked_list.h"

Node* head = NULL;
// HELPER FUCNTIONS**********************************************************************************************************8

/*handles the error if cmd dne or file dne
Input: int flag (if flag==0 --> cmd dne; if flag==1--> file dne), char* user Input
Returns nothing
*/
void error_Handler(int flag, char *inputVal){
    char holder[strlen(inputVal)]; // can't tokenize initial string without causing problems
    memcpy(holder, inputVal, strlen(inputVal)+1);
    char *token = strtok(holder, " ");

    if(flag==0){
        printf("%s: command not found\n", token);
    }else if (flag==1){
        /* code */
    }
    
}

/* Code from:https://www.programiz.com/c-programming/examples/remove-characters-string
  Helper function that removes all characters except those from the alphabet
*/
char * removeSpecialCharacter(char *string){
   for (int i = 0, j; string[i] != '\0'; ++i) {
      while (!(string[i] >= 'a' && string[i] <= 'z') && !(string[i] >= 'A' && string[i] <= 'Z') && !(string[i] == '\0')) {
         for (j = i; string[j] != '\0'; ++j) {
            string[j] = string[j + 1];
         }
         string[j] = '\0';
      }
   }
   return string;
}

//Takes a string and turns it into a pid
pid_t pidaMaker(char * str_pid){
  int x = atoi(str_pid);
  pid_t temp= x;
  return temp;
}

/*Takes in filepath and a buffer for the information
scans through the file and puts relevant information into the buffer
*/
int fileReader(char* filePath, char fileContents[1000][100]){
    FILE *ptr= fopen(filePath, "r");
    if(ptr==NULL){
        printf("no such file");
        return 0;
    }
    char buff[1000];
    int i=0;
    while(fscanf(ptr, "%s\t",buff)==1){
        strncpy(fileContents[i], buff, strlen(buff));
        i++;
    }
    return i;
}
/*Same thing as fileReader, but different regex is used for fscanf
*/
int fileReader2(char* filePath, char fileContents[1000][1000]){
    FILE *ptr= fopen(filePath, "r");
    if(ptr==NULL){
        printf("no such file");
        return 0;
    }
    char buff2[1000];
    int i=0;
    while(fscanf(ptr, "%[^:]:\n",buff2)==1){
        strncpy(fileContents[i], buff2, strlen(buff2));
        i++;
    }
    return i;
}
/*Checks the status of each child process 
then prints out a statement according to the situation
*/
void processChecker() {
	pid_t pid;
	int	status;
	while (1==1) {
		pid = waitpid(-1, &status,  WNOHANG );
		if (pid > 0 && PifExist(head,pid)==1) {
			if (WIFSTOPPED(status)){
         printf("Process: %d was stopped\n", pid);

       }else if(WIFCONTINUED(status)){
         printf("Process: %d was continued\n", pid);

       }else if(WIFSIGNALED(status)){
         printf("Process: %d was signaled\n", pid);

       }else if(WIFEXITED(status)){
        head= deleteNode(head,pid);
         printf("Process: %d was exited\n", pid);
       }
		} else {
			break;
		}
	}
}
//*******************************************************************************************************************************

//Creates a new process and executes a file
void func_BG(char *cmd[50], char *args[50]){
  pid_t pid = fork();
  if(pid==-1){
    exit(1);
  }
	if (pid == 0) {    // child
    sleep(1);
    execvp(args[0], args);
		printf("Error! Failed to execute command: %s\n", cmd[1]);
		exit(2);
	} else if (pid > 0) {		// parent
    printf("Started background process %d\n", pid);

    char path[1000];
    char *temp= removeSpecialCharacter(args[0]);
    getcwd(path, 1000);
    strncat(path,"/", strlen("/")+1);
    strncat(path,temp, strlen(temp)+1);
    char path2[1000];
    strncpy(path2,path,strlen(path)+1);

		head= add_newNode(head, pid, path2);
    sleep(2);
	} else {
		printf("Error: failed to fork\n");
	}
}
    


//lists total background processes
void func_BGlist(char **cmd){
	int count = 0;
	Node *curr = head;

	while (curr != NULL) {
		printf("%d:\t %s\n", curr->pid, curr->path);
		curr = curr->next;
    count++;
	}
	printf("Total background jobs:\t%d\n", count);
}

// Terminates specified process
void func_BGkill(char * str_pid){
  if(str_pid!=NULL){
    pid_t pid=pidaMaker(str_pid); 
	  if(PifExist(head, pid)==0) {
		  printf("Error: invalid pid\n");
		  return;
	  }

	  int kill_val = kill(pid, SIGTERM);
	  if(!kill_val) {
      head= deleteNode(head, pid);
      int waitVal= waitpid(pid, NULL, WNOHANG);
      if(waitVal==0){
        printf("Successfully terminated process: %d\n", pid);
      }else{
        printf("failed to kill process: %d\n", pid);
      }
	  }else{
		  printf("Error: bgkill failed\n");
	  }
  }else{
    printf("Invalid Pid\n");
  }
  
}

// Stops a specified process
void func_BGstop(char * str_pid){
  if(str_pid!=NULL){
    pid_t pid=pidaMaker(str_pid); 
	  if(PifExist(head, pid)==0) {
		  printf("Error: invalid pid\n");
		  return;
	  }

	  int kill_val = kill(pid, SIGSTOP);
	  if(!kill_val) {
      sleep(1);
	  }else{
		  printf("Error: bgstop failed\n");
	  }
  }else{
    printf("Invalid Pid\n");
  }
	
}

//Restarts a specified process
void func_BGstart(char * str_pid){
  if(str_pid!=NULL){
    pid_t pid=pidaMaker(str_pid); 
	  if(PifExist(head, pid)==0) {
		  printf("Error: invalid pid\n");
		  return;
	  }

	  int kill_val = kill(pid, SIGCONT);
	  if(!kill_val) {
      sleep(1);
  	}else{
	  	printf("Error: bgstart failed\n");
	  }
  }else{
    printf("Invalid Pid\n");
  } 
}


//Displays relevant information of a specified process
void func_pstat(char * str_pid){
   if(str_pid!=NULL){  
      pid_t pid=pidaMaker(str_pid); 
 	    if (PifExist(head,pid)){
        char buffer[1000];
        char buffer2[1000];
        char readStat[1000][100];
        char readStatus[1000][1000];

        sprintf(buffer, "/proc/%d/stat",pid);
        sprintf(buffer2, "/proc/%d/status",pid);

        fileReader(buffer, readStat);
        int k= fileReader2(buffer2,readStatus);
  
        char *newComm= removeSpecialCharacter(readStat[1]);
        printf("\ncomm:%s\n",newComm);
        printf("state:%s\n", readStat[2]);
    
        long int ut= atoi(readStat[13])/sysconf(_SC_CLK_TCK);
        long int st= atoi(readStat[14])/sysconf(_SC_CLK_TCK);
        printf("utime: %lu\n",ut);
        printf("stime: %lu\n",st);
        printf("rss: %s\n", readStat[23]);

        char *token= strtok(readStatus[k-2],"\n");
        printf("voluntary_ctxt_switches: %s\n",token);
        printf("nonvoluntary_ctxt_switches: %s\n",readStatus[k-1]);
      }
    } else{
      printf("invalid Pid\n");
    }
}



int main(){
    char user_input_str[50];
    while (true) {
      printf("Pman: > ");
      fgets(user_input_str, 50, stdin);
      char * ptr = strtok(user_input_str, " \n");
      if(ptr == NULL){
        continue;
      }
      char * lst[50];
      int index = 0;
      lst[index] = ptr;
      index++;
      while(ptr != NULL){
        ptr = strtok(NULL, " \n");
        lst[index]=ptr;
        index++;
      }
      //this chunk of code puts relevant data into 
      //an array that execvp can use
      char *args[50]={};
      int ppp=0;
      for(int i=1; i<index; i++){
        args[ppp]=lst[i];
        ppp++;
      }

      processChecker();

      if (strcmp("bg",lst[0]) == 0){
        func_BG(lst, args);
      } else if (strcmp("bglist",lst[0]) == 0) {
        func_BGlist(lst);
      } else if (strcmp("bgkill",lst[0]) == 0) {
        func_BGkill(lst[1]);
      } else if (strcmp("bgstop",lst[0]) == 0) {
        func_BGstop(lst[1]);
      } else if (strcmp("bgstart",lst[0]) == 0) {
        func_BGstart(lst[1]);
      } else if (strcmp("pstat",lst[0]) == 0) {
        func_pstat(lst[1]);
      } else if (strcmp("q",lst[0]) == 0) {
        printf("Bye Bye \n");
        exit(0);
      } else {
        error_Handler(0,user_input_str);
      }
      processChecker();
    }

  return 0;
}
