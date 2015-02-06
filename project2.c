// James Purkiss CS 370 Project 2
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<termios.h>


void main() 
{
  // Get the original termios configuration
  struct termios origConfig;
  tcgetattr(0, &origConfig);
  // Create a copy of the original configuration
  struct termios newConfig = origConfig;
  // adjust the new configuration
  newConfig.c_lflag &= ~(ICANON|ECHO);
  newConfig.c_cc[VMIN] = 10;
  newConfig.c_cc[VTIME] = 2;
  // Set the new configuration
  tcsetattr(0, TCSANOW, &newConfig);
  int *hist_count = 0;
  char *command[10];                          // Argument array    
  char *cmd_line = calloc(256, sizeof(char)); // Input buffer from shell prompt                   
  char *hist_array = (char *)calloc(256, sizeof(char));

  int stop = 0;                                   // Initialize stop loop variable to false (0)
  while(!stop) 
    {  
      prompt_input(command);

      if(strcmp(command[0], "exit") == 0)         // Check if user wishes to exit
	stop = 1; 
      else if(strcmp(command[0], "cd") == 0)      // Check if command to change directory is entered
	{
	  if(chdir(command[1]) != 0)
	    printf("\nThat directory does not exist!");
	  else
	    execute(command);
	} // End if
      else
	printf("\nThat command is not valid!");   // Print error if no valid command is entered
    
      free(command);
      free(cmd_line);
    } // End of while loop
  // Restore the original configuration
  tcsetattr(0, TCSANOW, &origConfig);
} // End of main


void prompt_input(char *command[], char *hist_array[], int *hist_count)
{
  /*
  char *arrow = (char*)calloc(2, sizeof(char));
  arrow[0] = 27;
  arrow[1] = 91;
  char *up = 65;
  char *down = 66;
  */
  char *prompt = "jprompt $>";
  char char_in;
  int buff_position = 0;

  int input = 1;
  while(input)
    {
      printf("\n[%d]", prompt);                   // Print prompt   
      char_in = getchar();
      if(char_in == 27)
	{
	  char_in = getchar();            // get second character of arrow key
	  char_in = getchar();            // get third character of arrow key
	  if(char_in == 65)
	    {
	      // up arrow history here
	    }
	  else if(char_in == 66)
	    {
	      // down arrow history here
	    }
	  else if(char_in == 67)
	    {
	      // right arrow key
	    }
	  else if(char_in == 68)
	    {
	      // left arrow key
	    }
	} // End if
      else if(char_in == "\n")
	{
	  // end input, save history (end in NULL), return command to main 
	}
      else if(char_in != NULL)
	{
	  // Store character in buffer
	}
    }
  parse(command, cmd_line);                   // Parse the newly read input      
}


void parse(char *command[], char *cmd_line) 
{
  int arg_cnt = 0;
  char *token = strtok(cmd_line, " ");
  
  while(token != NULL) 
    {
      command[arg_cnt] = token;
      token = strtok(cmd_line, " ");
      arg_cnt = arg_cnt + 1;
    } // End of while loop
  command[arg_cnt] = NULL;
} // End of parse function


void execute(char *command[]) 
{
  int pid, status;
  pid = fork();
  
  if(pid > 0) { // Parent process
    waitpid(pid, &status, WUNTRACED);
  }
  else if(pid == 0) { // Child process
    execvp(command[0], command);
    perror(NULL);
    exit(pid);
  } // End if
} // End of execute function
