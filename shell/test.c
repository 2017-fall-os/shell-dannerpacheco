#include "mytoken.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int length(char *str);
int compare(char *str1, char *str2);
char * stringCopy(char *str);
char ** findPATH(char **list);
char * stringConcat(char *str1, char *str2);

int main(int argc, char **argv, char **envp){
  

  char **tenvp = findPATH(envp);
  char **path = mytoc(tenvp[1],':');
  char buffer[512];
  int run = 1;
  
  while(run){

    
    write(1, "[myShell] $ ", 11);
    read(0, buffer, 512);
    char **vecToken = mytoc(buffer,' ');
    if(compare(vecToken[0], "exit") == 1){
      run = 0;
      break;
     
    }  
    int pid;
    pid = fork();
    
    //If child:
    if(pid == 0) {

      //Tries to execute with absolute path
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
    //If parent:
    else{
      
      int waitVal, waitStatus;
      waitpid(pid, &waitStatus, 0);
      if(waitStatus != 0 && waitStatus != 512){

	printf("Terminated with %d\n", waitStatus);
	
      }
    }
    
  }
    
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
