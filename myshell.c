#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 100

void execute_command(char *command) {
    char *args[MAX_ARGS];
    char *token = strtok(command, " ");
    int index = 0;


    while (token != NULL && index < MAX_ARGS - 1) {
        args[index++] = token;
        token = strtok(NULL, " ");
    }
   
    args[index] = NULL; // Null-terminate the argument array


    // Execute the command
    execvp(args[0], args);
    perror("Command not recognized"); // Error message for execvp failure
    exit(1); // Exit with an error code if exec fails
}

int main() {
    char command[MAX_CMD_LENGTH];

    while (1) {
        // Prompt the user for input
        printf("myshell> ");
       
        // Read the input
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("Error reading input");
            continue; // Handle error gracefully
        }

        // Remove the newline character, if present
        command[strcspn(command, "\n")] = 0;

        // Check for empty command
        if (strlen(command) == 0) {
            continue; // Skip empty commands
        }

        // Handle exit command
        if (strcmp(command, "exit") == 0) {
            break; // Exit the loop
        }

        // Check for pipe
        char *pipe_pos = strchr(command, '|');
        if (pipe_pos != NULL) {
            *pipe_pos = '\0'; // Split the command at the pipe
            char *first_cmd = command; // Command before the pipe
            char *second_cmd = pipe_pos + 1; // Command after the pipe

            int pipefd[2];
            if (pipe(pipefd) == -1) {
                perror("Pipe creation failed");
                continue;
            }

            // Fork the first child process
            pid_t pid1 = fork();
            if (pid1 < 0) {
                perror("Fork failed for first command");
                continue;
            }

            if (pid1 == 0) {
                // First child process
                close(pipefd[0]); // Close unused read end
                dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
                close(pipefd[1]); // Close original write end
                execute_command(first_cmd);
            }

            // Fork the second child process
            pid_t pid2 = fork();
            if (pid2 < 0) {
                perror("Fork failed for second command");
                continue;
            }

            if (pid2 == 0) {
                // Second child process
                close(pipefd[1]); // Close unused write end
                dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe
                close(pipefd[0]); // Close original read end
                execute_command(second_cmd);
            }

            // Close both ends of the pipe in the parent process
            close(pipefd[0]);
            close(pipefd[1]);

            // Wait for both child processes to finish
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
        } else {
            // Normal command execution without pipe
            pid_t pid = fork();
            if (pid < 0) {
                perror("Fork failed for command");
                continue;
            }

            if (pid == 0) {
                execute_command(command);
            } else {
                int status;
                waitpid(pid, &status, 0); // Wait for the child process to finish
                if (WIFEXITED(status)) {
                    printf("Child exited with status %d\n", WEXITSTATUS(status));
                }
            }
        }

        // Handle cd command
        if (strncmp(command, "cd ", 3) == 0) {
            char *dir = command + 3; // Get the directory part of the command
            if (chdir(dir) != 0) {
                perror("chdir failed"); // Print error if chdir fails
            }
            continue; // Skip the fork/exec for cd command
        }
    }

    return 0;
}