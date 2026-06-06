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
#include <readline/history.h>


int main() {
        while (true) {
            char* raw = readline("myshell> ");

            // Ctrl+D sends EOF: readline returns NULL
            if (raw == nullptr) {
                std::cout << std::endl;
                break;
            }

            std::string line(raw);
            free(raw);

            if (line.empty()) {
                continue;
            }

            add_history(line.c_str());

            if (line == "exit" || line == "quit") {
                break;
            }

            std::vector<std::string> args = tokenize(line);
            if (args.empty()) {
                continue;
            }

            std::vector<Command> Commands = getCommands(args);
            if (Commands.empty()) {
                continue;
            }

            if (Commands[0].execName == "cd") {
                continue;
            }

            // Fork all child processes first, then wait — avoids pipe deadlock
            std::vector<pid_t> pids;
            for (auto &cmd : Commands) {
                pid_t pid = fork();

                if (pid == -1) {
                    perror("fork failed");
                    break;
                } else if (pid == 0) {
                    if (cmd.outputFd != STDOUT_FILENO) {
                        if (dup2(cmd.outputFd, STDOUT_FILENO) == -1) {
                            perror("dup2 output");
                            exit(EXIT_FAILURE);
                        }
                    }
                    if (cmd.inputFd != STDIN_FILENO) {
                        if (dup2(cmd.inputFd, STDIN_FILENO) == -1) {
                            perror("dup2 input");
                            exit(EXIT_FAILURE);
                        }
                    }
                    // Close all pipe fds inherited from parent before exec
                    for (auto &other : Commands) {
                        if (other.inputFd != STDIN_FILENO)   close(other.inputFd);
                        if (other.outputFd != STDOUT_FILENO) close(other.outputFd);
                    }
                    if (execvp(cmd.execName.c_str(), const_cast<char *const *>(cmd.argv.data())) == -1) {
                        std::cerr << cmd.execName << ": " << strerror(errno) << std::endl;
                        exit(EXIT_FAILURE);
                    }
                } else {
                    pids.push_back(pid);
                    // Parent closes its copies of the pipe fds so children can detect EOF
                    if (cmd.inputFd != STDIN_FILENO)   close(cmd.inputFd);
                    if (cmd.outputFd != STDOUT_FILENO) close(cmd.outputFd);
                }
            }

            // Wait for all children
            for (pid_t pid : pids) {
                int status;
                waitpid(pid, &status, 0);
            }
        }

        return 0;
    }

