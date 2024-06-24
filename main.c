#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_MSG 100

void user1_msg(int user1[2], int user2[2])
{
  char msg_in[MAX_MSG];
  char msg_out[MAX_MSG];

  close(user1[0]);
  close(user2[1]);

  while (1)
  {
    fgets(msg_out, sizeof(msg_out), stdin);
    write(user1[1], msg_out, strlen(msg_out) + 1);

    if (strncmp(msg_out, "exit", 4) == 0)
      break;
    
    printf("user1: %s\n", msg_out);
    
    read(user2[0], msg_in, MAX_MSG);

    if (strncmp(msg_in, "exit", 4) == 0)
      break;
  }
  
  close(user1[1]);
  close(user2[0]);
}

void user2_msg(int user1[2], int user2[2])
{
  char msg_in[MAX_MSG];
  char msg_out[MAX_MSG];
  
  close(user1[1]);
  close(user2[0]);

  while (1)
  {
    read(user1[0], msg_in, MAX_MSG);
    
    if (strncmp(msg_in, "exit", 4) == 0)
      break;
    
    fgets(msg_out, sizeof(msg_out), stdin);
    write(user2[1], msg_out, strlen(msg_out) + 1);
    
    if (strncmp(msg_out, "exit", 4) == 0)
      break;
    
    printf("user2: %s\n", msg_out);
  }
  
  close(user1[0]);
  close(user2[1]);
}

int main()
{
  int user1[2], user2[2];
  pid_t p_hijo;
  
  if (pipe(user1) == -1 || pipe(user2) == -1)
  {
    perror("Error al crear pipe");
    exit(EXIT_FAILURE);
  }

  p_hijo = fork();

  if (p_hijo == -1)
  {
    perror("Error al crear pipe");
    exit(EXIT_FAILURE);
  }

  else if(p_hijo == 0)
    user1_msg(user1, user2);

  else
  {
    user2_msg(user1, user2);
    wait(NULL);
  }
  
  return 0;
}