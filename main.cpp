#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "shelpers.h"
#include <readline/readline.h>


int main() {
        std::string line;
        while (true) {
            line = readline("myshell>");


            std::vector<std::string> args =tokenize(line);
            std::vector<Command> Commands = getCommands(args);
            if (Commands[0].execName != "cd") {
                for (auto &Command: Commands) {
                    pid_t pid = fork();

                    if (pid == -1) {
                        perror("Fork failed");
                        continue;
                    } else if (pid == 0) {
                        // Redirect I/O if necessary
                        if (Command.outputFd != 1) {
                            if (dup2(Command.outputFd, 1) == -1) {
                                perror("open output file/Dup2 error");
                                exit(EXIT_FAILURE);
                            }
                        }
                        if (Command.inputFd != 0) {
                            if (dup2(Command.inputFd, 0) == -1) {
                                perror("open input file/Dup2 error");
                                exit(EXIT_FAILURE);
                            }
                        }
                        // Child process
                        if (execvp(Command.execName.c_str(), const_cast<char *const *>(Command.argv.data())) == -1) {
                            // If execvp returns, it must have failed
                            std::cerr << "Unknown command or execvp failed with error: " << strerror(errno)
                                      << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        // Parent process
                        if (Command.inputFd != 0) {
                            if (close(Command.inputFd) == -1) {
                                std::cout << "close input is fail";
                            }
                        }
                        if (Command.outputFd != 1) {//check the output is open or not, !=1 means is open
                            // then we close
                            if (close(Command.outputFd) == -1) {//check if close successfully,if not cout error
                                std::cout << "close output is fail";
                            }
                        }

                        int status;
                        waitpid(pid, &status, 0);
                    }
                }
            }

        }

        return 0;
    }

