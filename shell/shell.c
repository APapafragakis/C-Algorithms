#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SIZE 248
int f_descriptor;

enum Type {
  REDIRECTION,
  PIPES
};

char** Do()
{

    char **command;
    char **args;
    char *input;
    char *iterator;
	int i,j;
	enum Type type;
	
    char* removingToken = " \n=";
	iterator = (char *)malloc(sizeof(char) * SIZE);
	command=(char**)malloc(sizeof(char*) * SIZE);
    args=(char**)malloc(sizeof(char*) * SIZE);
	input =(char *)malloc(sizeof(char)*SIZE);
	fgets(input, SIZE, stdin);

	iterator = strtok(input, removingToken);
    i=j=0;

	while(iterator != NULL){
        int isRedirection = strcmp(iterator, "<") == 0 || strcmp(iterator, ">") == 0 || strcmp(iterator, ">>") == 0;
        int isPipes = strcmp(iterator, "|") == 0;

		if(isRedirection){
			type = REDIRECTION;
			break;
		}else if(isPipes){
			type = PIPES;
			break;
		}

		command[i] = iterator;
		iterator = strtok(NULL, removingToken);
		i++;	
	}

     

	if(type == REDIRECTION){
		while(iterator != NULL){
			args[j] = iterator;
			iterator = strtok(NULL, removingToken);
			j++;
		}
        
        command[i]=NULL;
	    args[j]=NULL;

        int hasInput = args[0]!=NULL && strcmp(args[0],"<")==0;
        int hasOutput = args[0]!=NULL && strcmp(args[0],">")==0;
        int hasOutputOverwrite = args[0]!=NULL && strcmp(args[0],">>")==0;

        if (hasInput)
        {
            char *filename = args[1];
            FILE *file = fopen(filename, "r");
            if (file == NULL)
            {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            f_descriptor = dup(fileno(stdin));	
            dup2(fileno(file),fileno(stdin));
            fclose(file);
        }


        if (hasOutput)
        {
            char *filename = args[1];
            FILE *file = fopen(filename, "w+");
            if (file == NULL)
            {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            f_descriptor = dup(fileno(stdout));
            dup2(fileno(file),fileno(stdout));
            fclose(file);
        }

        if (hasOutputOverwrite)
        {
            char *filename = args[1];
            FILE *file = fopen(filename, "a");
            if (file == NULL)
            {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            f_descriptor = dup(fileno(stdout));
            dup2(fileno(file),fileno(stdout));
            fclose(file);
        }


	}else if(type == PIPES){
		while(iterator != NULL){
			if(strcmp(iterator, "|") == 0){
				iterator = strtok(NULL, " \n=\"\'");
			}
            args[j] = iterator;
            iterator = strtok(NULL, " \n=\"\'");
            j++;
        }

        command[i]=NULL;
	    args[j]=NULL;

        int hasPipeCommand = args[0]!=NULL;
        
        if (hasPipeCommand)
        {
            pid_t p1, p2;
            int pipes[2];

            if (pipe(pipes) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            p1 = fork();

            if (p1 < 0)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else if (p1 == 0)
            {
                dup2(pipes[1], STDOUT_FILENO);
                close(pipes[1]);
                close(pipes[0]);
                execvp(command[0],command);
            }
            else
            {
                waitpid(p1, NULL, 0);
                p2 = fork();

                if (p2 < 0)
                {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                else if (p2 == 0)
                {
                    dup2(pipes[0], STDIN_FILENO);
                    close(pipes[0]);
                    close(pipes[1]);
                    execvp(args[0],args);
                }
                else
                {
                    close(pipes[0]);
                    close(pipes[1]);
                }
            }
        }
    }
	

	
    
    if (strcmp(command[0], "quit") == 0)
    {
        exit(EXIT_SUCCESS);
    }


    if (strcmp(command[0], "chdir") == 0 || strcmp(command[0], "cd") == 0)
    {
        if (command[1] != NULL)
        {
            if (chdir(command[1]) != 0)
            {
                perror("chdir");
            }
        }
        else
        {
            fprintf(stderr, "Usage: chdir <directory>\n");
        }
    }

    return command;
}

int main()
{
    char **args;
    int status;
    pid_t pid;

    f_descriptor = 0;

    while (1)
    {
        printf("csd5084-hy345sh@5084:$DIRECTORY $ ");
       
        args = Do();

        pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            execvp(args[0], args);
            exit(EXIT_FAILURE);
        } else {
            int status;
            waitpid(pid, &status, 0);
        }
    
        dup2(f_descriptor, fileno(stdout));
	    dup2(f_descriptor, fileno(stdin));

    }

    return 0;
}