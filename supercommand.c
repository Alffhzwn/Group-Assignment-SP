#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <unistd.h>

#include <fcntl.h>

#include <sys/types.h>

#include <sys/stat.h>

#include <dirent.h>

#include <time.h>

#include <termios.h>

#include <pthread.h>



// Function prototypes

void file_operations(int operation, char *arg1, char *arg2);

void directory_operations(int operation, char *arg1);

void *keylogger(void *arg);



// Global flag for keylogger thread

int running = 1;



int main(int argc, char *argv[]) {

    if (argc < 3) {

        fprintf(stderr, "Usage: ./supercommand -m <mode> <operation> [args]\n");

        exit(EXIT_FAILURE);

    }



    int mode = atoi(argv[2]);



    switch (mode) {

        case 1: // File operations

            if (argc < 5) {

                fprintf(stderr, "Usage: ./supercommand -m 1 <operation> <filename> [permission]\n");

                exit(EXIT_FAILURE);

            }

            file_operations(atoi(argv[3]), argv[4], argc == 6 ? argv[5] : NULL);

            break;



        case 2: // Directory operations

            if (argc < 4) {

                fprintf(stderr, "Usage: ./supercommand -m 2 <operation> <directory>\n");

                exit(EXIT_FAILURE);

            }

            directory_operations(atoi(argv[3]), argv[4]);

            break;



        case 3: // Keylogger

            pthread_t logger_thread;

            if (pthread_create(&logger_thread, NULL, keylogger, NULL) != 0) {

                perror("Failed to create keylogger thread");

                exit(EXIT_FAILURE);

            }

            printf("Keylogger started. Press Ctrl+C to stop.\n");

            pthread_join(logger_thread, NULL);

            break;



        default:

            fprintf(stderr, "Invalid mode selected.\n");

            exit(EXIT_FAILURE);

    }



    return 0;

}



void file_operations(int operation, char *arg1, char *arg2) {

    int fd;

    switch (operation) {

        case 1: // Create/Open file

            fd = open(arg1, O_CREAT | O_WRONLY, 0644);

            if (fd == -1) {

                perror("Error creating/opening file");

            } else {

                printf("File created/opened successfully: %s\n", arg1);

                close(fd);

            }

            break;



        case 2: // Change file permissions

            if (chmod(arg1, strtol(arg2, NULL, 8)) == -1) {

                perror("Error changing file permissions");

            } else {

                printf("File permissions changed successfully: %s\n", arg1);

            }

            break;



        case 3: // Read file

            fd = open(arg1, O_RDONLY);

            if (fd == -1) {

                perror("Error opening file for reading");

            } else {

                char buffer[1024];

                ssize_t bytes;

                while ((bytes = read(fd, buffer, sizeof(buffer) - 1)) > 0) {

                    buffer[bytes] = '\0';

                    printf("%s", buffer);

                }

                printf("\n");

                close(fd);

            }

            break;



        case 4: // Write to file

            fd = open(arg1, O_WRONLY | O_APPEND);

            if (fd == -1) {

                perror("Error opening file for writing");

            } else {

                printf("Enter text to write to file (Ctrl+D to end):\n");

                char buffer[1024];

                while (fgets(buffer, sizeof(buffer), stdin)) {

                    write(fd, buffer, strlen(buffer));

                }

                close(fd);

            }

            break;



        case 5: // Delete file

            if (unlink(arg1) == -1) {

                perror("Error deleting file");

            } else {

                printf("File deleted successfully: %s\n", arg1);

            }

            break;



        default:

            fprintf(stderr, "Invalid file operation.\n");

    }

}



void directory_operations(int operation, char *arg1) {

    switch (operation) {

        case 1: // Create directory

            if (mkdir(arg1, 0755) == -1) {

                perror("Error creating directory");

            } else {

                printf("Directory created successfully: %s\n", arg1);

            }

            break;



        case 2: // Remove directory

            if (rmdir(arg1) == -1) {

                perror("Error removing directory");

            } else {

                printf("Directory removed successfully: %s\n", arg1);

            }

            break;



        case 3: // Print current directory

            char cwd[1024];

            if (getcwd(cwd, sizeof(cwd)) == NULL) {

                perror("Error getting current directory");

            } else {

                printf("Current directory: %s\n", cwd);

            }

            break;



        case 4: // List directory contents

            DIR *dir;

            struct dirent *entry;

            if ((dir = opendir(arg1)) == NULL) {

                perror("Error opening directory");

            } else {

                printf("Contents of directory %s:\n", arg1);

                while ((entry = readdir(dir)) != NULL) {

                    printf("%s\n", entry->d_name);

                }

                closedir(dir);

            }

            break;



        default:

            fprintf(stderr, "Invalid directory operation.\n");

    }

}



void *keylogger(void *arg) {

    FILE *log_file = fopen("keylog.txt", "a");

    if (!log_file) {

        perror("Error opening keylog file");

        pthread_exit(NULL);

    }



    // Add timestamp

    time_t now = time(NULL);

    fprintf(log_file, "Keylogger session started: %s\n", ctime(&now));

    fflush(log_file);



    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newt);



    char c;

    while (running && read(STDIN_FILENO, &c, 1) == 1) {

        fprintf(log_file, "%c", c);

        fflush(log_file);

    }

    

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    fclose(log_file);

    pthread_exit(NULL);

}