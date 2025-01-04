#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

// Function prototypes
void file_operations();
void directory_operations();
void keylogger_operations();
void print_menu();
void run_keylogger();

// Global flag for keylogger
int running = 1;

// Main function
int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_menu();
    } else if (argc > 1) {
        if (strcmp(argv[1], "-m") != 0) {
            fprintf(stderr, "Invalid command format. Use -m flag.\n");
            return 1;
        }

        int mode = atoi(argv[2]);
        switch (mode) {
            case 1:
                file_operations();
                break;
            case 2:
                directory_operations();
                break;
            case 3:
                keylogger_operations();
                break;
            default:
                printf("Invalid mode selected. Use 1, 2, or 3.\n");
        }
    }
    return 0;
}

// Print menu
void print_menu() {
    printf("=== SUPERCOMMAND MENU ===\n");
    printf("1. File Operations\n");
    printf("2. Directory Operations\n");
    printf("3. Keylogger Operations\n");
    printf("Select an option or use CLI mode.\n");
}

// File operations menu
void file_operations() {
    int option;
    printf("\n=== File Operations ===\n");
    printf("1. Create/Open File\n");
    printf("2. Change File Permissions\n");
    printf("3. Read File\n");
    printf("4. Write File\n");
    printf("5. Delete File\n");
    printf("Select an option: ");
    scanf("%d", &option);

    char filename[256];
    char data[1024];
    int fd, mode;

    switch (option) {
        case 1:
            printf("Enter filename to create/open: ");
            scanf("%s", filename);
            fd = open(filename, O_CREAT | O_RDWR, 0644);
            if (fd == -1) {
                perror("Error creating/opening file");
            } else {
                printf("File %s created/opened successfully.\n", filename);
                close(fd);
            }
            break;

        case 2:
            printf("Enter filename to change permissions: ");
            scanf("%s", filename);
            printf("Enter new permission mode (e.g., 0777): ");
            scanf("%o", &mode);
            if (chmod(filename, mode) == -1) {
                perror("Error changing file permissions");
            } else {
                printf("Permissions changed successfully for %s.\n", filename);
            }
            break;

        case 3:
            printf("Enter filename to read: ");
            scanf("%s", filename);
            fd = open(filename, O_RDONLY);
            if (fd == -1) {
                perror("Error reading file");
            } else {
                read(fd, data, sizeof(data));
                printf("File content: %s\n", data);
                close(fd);
            }
            break;

        case 4:
            printf("Enter filename to write to: ");
            scanf("%s", filename);
            fd = open(filename, O_WRONLY | O_APPEND);
            if (fd == -1) {
                perror("Error opening file for writing");
            } else {
                printf("Enter data to write: ");
                scanf(" %[^\n]", data);
                write(fd, data, strlen(data));
                printf("Data written successfully to %s.\n", filename);
                close(fd);
            }
            break;

        case 5:
            printf("Enter filename to delete: ");
            scanf("%s", filename);
            if (unlink(filename) == -1) {
                perror("Error deleting file");
            } else {
                printf("File %s deleted successfully.\n", filename);
            }
            break;

        default:
            printf("Invalid option.\n");
    }
}

// Directory operations menu
void directory_operations() {
    int option;
    printf("\n=== Directory Operations ===\n");
    printf("1. Create Directory\n");
    printf("2. Delete Directory\n");
    printf("3. Print Current Directory\n");
    printf("4. List Directory Contents\n");
    printf("Select an option: ");
    scanf("%d", &option);

    char dirname[256];
    DIR *dir;
    struct dirent *entry;

    switch (option) {
        case 1:
            printf("Enter directory name to create: ");
            scanf("%s", dirname);
            if (mkdir(dirname, 0755) == -1) {
                perror("Error creating directory");
            } else {
                printf("Directory %s created successfully.\n", dirname);
            }
            break;

        case 2:
            printf("Enter directory name to delete: ");
            scanf("%s", dirname);
            if (rmdir(dirname) == -1) {
                perror("Error deleting directory");
            } else {
                printf("Directory %s deleted successfully.\n", dirname);
            }
            break;

        case 3:
            if (getcwd(dirname, sizeof(dirname)) == NULL) {
                perror("Error getting current directory");
            } else {
                printf("Current Directory: %s\n", dirname);
            }
            break;

        case 4:
            if ((dir = opendir(".")) == NULL) {
                perror("Error opening directory");
            } else {
                printf("Directory contents:\n");
                while ((entry = readdir(dir)) != NULL) {
                    printf("%s\n", entry->d_name);
                }
                closedir(dir);
            }
            break;

        default:
            printf("Invalid option.\n");
    }
}

// Keylogger operations
void keylogger_operations() {
    printf("\n=== Keylogger Operations ===\n");
    printf("Starting keylogger in the background...\n");
    if (fork() == 0) {
        run_keylogger();
        exit(0);
    }
}

void run_keylogger() {
    FILE *keylog = fopen("keylog.txt", "a");
    if (!keylog) {
        perror("Error opening keylog.txt");
        exit(1);
    }

    // Add timestamp
    time_t now = time(NULL);
    fprintf(keylog, "Keylogger session started at %s\n", ctime(&now));
    fclose(keylog);

    // Simulate keylogger behavior (real implementation needs root/system calls)
    while (running) {
        sleep(1); // Simulate background running
    }
    printf("Keylogger stopped.\n");
}
