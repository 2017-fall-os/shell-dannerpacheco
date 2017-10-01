#include "mytoken.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int length(char *str);
int compare(char *str1, char *str2);
char * stringCopy(char *str);
char ** findPATH(char **list);
char * stringConcat(char *str1, char *str2);
char * prevPath(char ** vect);
int numberOfTokens(char **vect);

int main(int argc, char **argv, char **envp){
  

  char **tenvp = findPATH(envp);
  char **path = mytoc(tenvp[1],':');
  
  char buffer[512];
  int run = 1;
  
  while(run){

    
    write(1, "[myShell] $ ", 11);
    read(0, buffer, 512);
    char **vecToken = mytoc(buffer,' ');
    int toWait = numberOfTokens(vecToken);
    int background = 0;
    if(compare(vecToken[toWait-1], "&") == 1){
	background = 1;
	vecToken[toWait-1] = "\0";
    }
       
    if(compare(vecToken[0], "exit") == 1){
      run = 0;
      break;
     
    }//if

    ///////////////////////////////////////////////////////////////////////
    
    if(compare(vecToken[0], "cd")){

      int absolute = 0;

      
	  if(compare(vecToken[1], "..") && vecToken[2] == '\0'){

	    char waka[1024];
	    getcwd(waka, sizeof(waka));
	    char **prev = mytoc(waka, '/');
	    char *newPath = prevPath(prev);
	    chdir(newPath);
	    //printf("New Path: %s\n", newPath);
	    

	  }//if ..
	  else if(vecToken[1] != ".."){
	    int pass = chdir(vecToken[1]);
	    if(pass == 0){
	      absolute = 1;
	    }

	  }

	  else if(vecToken[1] != '\0' && absolute == 0){
	    
	    char waka[1024];
	    getcwd(waka, sizeof(waka));
	    
	    char *newPath = stringConcat("/",vecToken[1]);
	    newPath = stringConcat(waka, newPath);
	    int runch = chdir(newPath);
	    if(runch != 0)
	      write(1, "Directory not found", 19); 

	  }//if not ..

    }//if cd

    ////////////////////////////////////////////////////////////////////////////////
    else{


      char ** pipeTokens = mytoc(buffer, '|');
      int pipeTok = numberOfTokens(pipeTokens);

      /*****************************************************************/
      if(pipeTok > 1){

	int *fileDescriptor;
	fileDescriptor = (int *) calloc(2, sizeof(int));
	pipe(fileDescriptor);

	int pid;
	int pid2;

	pipeTokens[0] = stringConcat(pipeTokens[0], " ");
	pipeTokens[1] = stringConcat(pipeTokens[1], " ");
	char **firstcmd = mytoc(pipeTokens[0], ' ');
	char **secondcmd = mytoc(pipeTokens[1], ' ');
	
	/*_____________FIRST CHILD PIPE________________*/
      pid = fork();
    
    //If child:
    if(pid == 0) {
                                                
      dup2(fileDescriptor[1],1);
      close(fileDescriptor[0]); 

      int ex = execve(firstcmd[0], firstcmd, envp);
      char *cmd = stringCopy(firstcmd[0]);
      if(ex == -1){
	int i = 0;
	while(path[i] != '\0'){
	  //Will look for the command in the PATH env
	  char *cmdPath = stringConcat(path[i],"/");
	  cmdPath = stringConcat(cmdPath, cmd);
	  firstcmd[0] = malloc(length(cmdPath));
	  firstcmd[0] = cmdPath;
	  ex = execve(cmdPath, firstcmd, envp);
	  i++;
	}
	write(1, "Command not found\n", 18);

      }


      exit(2);
    }

    //////////////////////PARENT////////////////////////////////
    else{
      
      int waitVal, waitStatus;
      waitpid(pid, &waitStatus, 0);
      close(fileDescriptor[1]);
      if(waitStatus != 0 && waitStatus != 512){

	printf("Terminated with %d\n", waitStatus);
	
      }//if wait


      pid2 = fork();
    
    //If child:
    if(pid2 == 0) {
      
      dup2(fileDescriptor[0],0);
      close(fileDescriptor[1]); 
      
      int ex = execve(secondcmd[0], secondcmd, envp);
      char *cmd = stringCopy(secondcmd[0]);
      if(ex == -1){
	int i = 0;
	while(path[i] != '\0'){
	  //Will look for the command in the PATH env
	  char *cmdPath = stringConcat(path[i],"/");
	  cmdPath = stringConcat(cmdPath, cmd);
	  secondcmd[0] = malloc(length(cmdPath));
	  secondcmd[0] = cmdPath;
	  ex = execve(cmdPath, secondcmd, envp);
	  i++;
	}
	write(1, "Command not found\n", 18);

      }


      exit(2);
    }

    //////////////////////PARENT////////////////////////////////
    else{
      
      int waitVal, waitStatus;
      int toWait = numberOfTokens(vecToken);
      if(compare(vecToken[toWait-1], "&") == 0){
      waitpid(pid2, &waitStatus, 0);
      dup2(0,fileDescriptor[0]);
      dup2(1,fileDescriptor[1]);
      
      if(waitStatus != 0 && waitStatus != 512){

	printf("Terminated with %d\n", waitStatus);
	
      }//if wait

      }
      
    }//else parent


    
    

      
    }//else parent
	

    
      }

      /*********************************************************/
      else if(pipeTok == 1){
    
      int pid;

      ////////////////////////////////////////CHILD///////////////////////////////
      pid = fork();
    
    //If child:
    if(pid == 0) {

      
      int ex = execve(vecToken[0], vecToken, envp);
      char *cmd = stringCopy(vecToken[0]);
      if(ex == -1){
	int i = 0;
	while(path[i] != '\0'){
	  //Will look for the command in the PATH env
	  char *cmdPath = stringConcat(path[i],"/");
	  cmdPath = stringConcat(cmdPath, cmd);
	  vecToken[0] = malloc(length(cmdPath));
	  vecToken[0] = cmdPath;
	  ex = execve(cmdPath, vecToken, envp);
	  i++;
	}
	write(1, "Command not found\n", 18);

      }


      exit(2);
    }

    //////////////////////PARENT////////////////////////////////
    else{

   
      
      int waitVal, waitStatus;
      int toWait = numberOfTokens(vecToken);
      if(!background){
	waitpid(pid, &waitStatus, 0);
	if(waitStatus != 0 && waitStatus != 512){

	printf("Terminated with %d\n", waitStatus);
	
      }//if wait
      }
    }//else parent
      }

    }//Else if it is not cd
    
  }//While that runs everything
    

    
  return 0;
  
}







//Returns the length of a string
int length(char *str){

  int count = 0;
  while(str[count] != '\0')
	count ++;
	
  return count;
}

//compares two strings
int compare(char *str1, char *str2){

  int count = 0;
  int len1 = length(str1);
  int len2 = length(str2);

  if(len1 == len2){
   
    for(int i =0; i < len1; i++){
      
      if(str1[i] != str2[i]){
	return 0;
    }

    }
    return 1;
  }
return 0;
  
}


//Returns the PATH string
char **findPATH(char** list){
  int index = 0;
  int run = 1;

  while(run){
    index++;
    char **keys = mytoc(list[index],'=');
    if(compare(keys[0], "PATH")){
      char *str = stringCopy(keys[1]);
      return keys;
    }
    
    
   }
  return 0;
       

}

char *stringCopy(char *str){

//copies string into vector 
      int j = 0;
      int lc = length(str);
      char * strcopy = malloc(lc+1);
      while(j < lc){
	strcopy[j] = str[j];
	j++;
       
      }
      //adds null terminator to string
      strcopy[lc+1] = '\0';

      return strcopy;

}


char* stringConcat(char *str1, char *str2){

  int len1 = length(str1);
  int len2 = length(str2);

  char *string = malloc(len1+len2+1);

  int i;
  int j = 0;
  for(i =0; i < (len1 + len2); i++){

    if(i < len1){
      string[i] = str1[i];
    }
    else{
      string[i] =str2[j];
      j++;
    }
  }

  string[len1 + len2] = '\0';
  return string;

}

 int numberOfTokens(char **vect){

   int count = 0;
   while(vect[count] != '\0'){

     count++;

   }

   return count;

 }

 char *prevPath(char ** vect){

   int numTok = numberOfTokens(vect);
   int lenghtPath = 0;
  
   for(int i = 0; i < numTok-1; i++){
     lenghtPath += length(vect[i]);
   }
   char *newPath  = (char *)malloc(lenghtPath);

   for(int i = 0; i < numTok-1; i++){
     newPath = stringConcat(newPath, "/");
     newPath = stringConcat(newPath, vect[i]);
     
   }


   return newPath;
 }
