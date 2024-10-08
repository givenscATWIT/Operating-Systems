# myshell
To compile and run your shell program, follow these steps.
1. Create the Source File:
  Ensure your shell source code is saved in a file named myshell.c.
2. Open a Terminal:
  Open a terminal window on your Unix-like system (Linux, macOS, etc.).
3. Navigate to the Directory:
  Use the cd command to navigate to the directory where myshell.c is located.
4. Compile the Shell:
  Use gcc (GNU Compiler Collection) to compile the source code. Run the following command:
    gcc myshell.c -o myshell
  This will generate an executable file named myshell.
5. Run the Shell:
  Execute the compiled shell by running:
    ./myshell

Basic Commands:  
  myshell> ls
  Expected Output: A list of files and directories in the current working directory.
  
  myshell> pwd
  Expected Output: The path of the current working directory, e.g., /home/user.

  myshell> date
  Expected Output: The current date and time, e.g., Tue Oct 08 14:23:01 UTC 2024.

  myshell> cat file.txt | wc -l
  Expected Output: The number of lines in file.txt, e.g., 5.

  myshell> ls | grep .c
  Expected Output: A list of .c files in the current directory, e.g.:

  myshell> cd ..
  Expected Output: No output, but the shell's current working directory changes to the parent directory.

  myshell> cd non_existent_directory
  Expected Output: An error message indicating that the directory cannot be changed, e.g., chdir failed: No such file or directory.

  myshell> exit
  Expected Output: The shell terminates, and you return to your terminal.
