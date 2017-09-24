#include "mytoken.h"
#include <unistd.h>

int length(char *str);
int compare(char *str1, char *str2);
char ** findPATH(char **list);




 


int main(int argc, char **argv, char **envp){
  

  char buffer[512];
  int run = 1;

  while(run){
     
    write(1, "$ ", 2);
    read(0, buffer, 512);
    char **vecToken = mytoc(buffer,' ');
    if(compare(vecToken[0], "exit") == 1){
      run = 0;
    }

    

    




    
      
    //prints all the tokens inside of the array
    int i = 0; 
    while(vecToken[i] != '\0'){
    int len = length(vecToken[i]);
    write(1, vecToken[i], len);
    write(1, "\n", 1);
    i++;
      
    }

    
    

  }



  char **path = findPATH(envp);
  printf("RETURNED\n");
  printf("%s\n",path[0]);
  
    

    
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

char **findPATH(char** list){

  
  int index = 0;
  char **keys;

  while(list[index] != '\0'){
    keys = mytoc(list[index],'=');
    printf("%s\n", keys[0]);
    if(compare(keys[0], "PATH")){
      printf("%s\n",keys[0]);
	break;

      }
    index++;
   }
    printf("%s\n",keys[1]);
    char **path = mytoc(keys[1], ':');
    return path;
       

}