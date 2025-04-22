#include "cli.h"
#include "ff.h"
#include "ffconf.h"
#include <string.h>
#include <stdio.h> 

static FATFS fs;
static FIL fil;
static DIR dir;
static FILINFO fno;

void mount() {
    FRESULT res = f_mount(&fs, "", 1);
    if (res != FR_OK) {
        printf("mount: failed\n");
    }
    return;
}

void pwd() {
    char buffer[128];
    FRESULT res= f_getcwd(buffer, sizeof(buffer));
    if (res != FR_OK) {
        printf("pwd: failed to retrieve path\n");
    } else {
        printf("%s\n", buffer);
    }
    return;
}

void rm(int argc, char *argv[]) {
    if (argc < 2) {
        printf("rm: missing operand\n");
        return;
    }

    for (int i = 1; i < argc; i++) {
        FRESULT res = f_unlink(argv[1]);
        if (res != FR_OK) {
            printf("rm: failed to remove %s\n", argv[1]);
        }
    }
    return;
}

void clear() {
    for (int i = 0; i < 50; i++) {
        printf("\n");
    }
    command_shell();
    return;
}

void append(int argc, char *argv[]) {
    if (argc < 2) {
        printf("append: missing operand\n");
        return;
    }

    FIL file;
    FRESULT res = f_open(&file, argv[1], (FA_WRITE|FA_OPEN_EXISTING|FA_OPEN_APPEND)); // FA_OPEN_APPEND: move pointer to the end of file

    if (res != FR_OK) {
        printf("append: failed to open file '%s'\n", argv[1]);
        return;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (strcmp(buffer, ".\n") == 0) {
            break;
        }
        UINT bytesWritten;
        res = f_write(&file, buffer, strlen(buffer), &bytesWritten);
        if (res != FR_OK || bytesWritten < strlen(buffer)) {
            printf("append: write error\n");
            break;
        }
    }

    f_close(&file);
    return;
}

void cat(int argc, char *argv[]) {
    if (argc < 2) {
        printf("cat: missing operand\n");
        return;
    }

    for (int i = 1; i < argc; i++) {
        FIL file;
        char buffer[128];
        UINT bytesRead;
        FRESULT res = f_open(&file, argv[i], FA_READ);

        if (res != FR_OK) {
            printf("cat: failed to open '%s'\n", argv[i]);
            continue;
        }

        while ((res = f_read(&file, buffer, sizeof(buffer)-1, &bytesRead)) == FR_OK && bytesRead > 0) {
            buffer[bytesRead] = '\0'; // need NULL to print as string
            printf("%s", buffer);
        }

        f_close(&file);
    }

    return;
}

void cd(int argc, char *argv[]) {
    FRESULT res;

    if (argc > 2) {
        printf("cd: too many arguements\n");
        return;
    }

    if (argc == 1) {
        res = f_chdir("/");
        if (res != FR_OK) {
            printf("cd: failed to change to root directory\n");
        }
        return;
    }

    res = f_chdir(argv[1]);
    if (res != FR_OK) {
        if (strcmp(argv[1], "..") == 0) {
            printf("cd: currently in the root directory\n", argv[1]);
        } else{
            printf("cd: failed to change directory to '%s'\n", argv[1]);
        }
    }
}

// void date(){
//     return;
// }

void input() {
    return;
}

void ls(int argc, char *argv[]) {
    const char *dirPath = (argc > 1) ? argv[1] : ".";
    DIR dir;
    FILINFO fno;

    FRESULT res = f_opendir(&dir, dirPath);
    if (res != FR_OK) {
        if (argc == 1) {
            printf("ls: failed to access current directory\n", dirPath);
        } else {
            printf("ls: failed to access '%s'\n", dirPath);
        }
        return;
    }

    while (1) {
        res = f_readdir(&dir, &fno);
        if (res != FR_OK || fno.fname[0] == 0) { // check end of directory
            break;
        }

        printf("%s\n", fno.fname);
    }

    f_closedir(&dir);
}

void mkdir(int argc, char *argv[]) {
    if (argc < 2) {
        printf("mkdir: missing operand\n");
        return;
    }

    for (int i = 1; i < argc; i++) {
        FRESULT res = f_mkdir(argv[i]);
        if (res != FR_OK) {
            printf("mkdir: failed to create %s\n", argv[i]);
            return;
        }
    }
    return;
}

void shout(){
    printf("Reached Shout\n");
}

struct commands_t cmds[] = {
        { "append", append }, //SD
        { "cat", cat }, //SD
        { "cd", cd }, //SD
        // { "date", date }, //fat time, can be faked to make it work
        { "input", input }, //SD
        { "ls", ls },//SD
        { "mkdir", mkdir }, //SD write
        { "mount", mount }, // calls SD card procedure
        { "pwd", pwd }, // SD retreive directory and drive
        { "rm", rm }, //SD write
        { "shout", shout }, // testing command
        { "clear", clear }, //terminal specific
};


void exec(int argc, char *argv[]) {
    for(int i=0; i<sizeof cmds/sizeof cmds[0]; i++)
        if (strcmp(cmds[i].cmd, argv[0]) == 0) {
            cmds[i].fn(argc, argv);
            return;
        }
    printf("%s: No such command.\n", argv[0]);
}

void parse_command(char *c) {
    char *argv[20];
    int argc=0;
    int skipspace=1;
    for(; *c; c++) {
        if (skipspace) {
            if (*c != ' ' && *c != '\t') {
                argv[argc++] = c;
                skipspace = 0;
            }
        } else {
            if (*c == ' ' || *c == '\t') {
                *c = '\0';
                skipspace=1;
            }
        }
    }
    if (argc > 0) {
        argv[argc] = "";
        exec(argc, argv);
    }
}

void command_shell(void) {
  char line[100];
  int len = strlen(line);
  printf("This is the AFT command shell.\n");
  for(;;) {
      printf("> ");
      fgets(line, 99, stdin);
      line[99] = '\0';
      len = strlen(line);
      if ((line[len-1]) == '\n'){
          line[len-1] = '\0';}
      parse_command(line);
  }
}

int main(int argc, char** argv) {
    command_shell();
    return 0;
}