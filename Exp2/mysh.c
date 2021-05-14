#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN     0xff
#define MAX_ARGS    0x0f
#define MAX_HISTORY 0x7f

struct history{
    int index;
    char command[MAX_HISTORY];
};


int command_cnt = 0;
int should_run = 1; // global flag for shell to run
struct history history_list[MAX_HISTORY];
int history_head = 0;
int history_len = 0;


void create_history(char* command){
    command_cnt++;
    if (history_len < MAX_HISTORY)
        history_len++;
    else
        history_head = (history_head + 1) % MAX_HISTORY;
    history_list[(history_head + history_len) % MAX_HISTORY].index = command_cnt;
    strcpy(history_list[(history_head + history_len) % MAX_HISTORY].command, command);
}

void parse_args(char* raw_command, char* command, char* args[], int *arg_cnt){
    int cnt = 0;
    if (raw_command[strlen(raw_command) - 1] == '\n')
        raw_command[strlen(raw_command) - 1] = 0;
    static char temp_command[MAX_LEN];  // !! the value may change if not set to static
    memset(temp_command, 0, sizeof(temp_command));
    strcpy(temp_command, raw_command);
    args[cnt] = strtok(temp_command, " ");
    while (args[cnt]){
         args[++cnt] = strtok(NULL, " ");
    }
    *arg_cnt = cnt;
    if (args[0])
        strcpy(command, args[0]);
    else
        command = "";
}

int run_inner_command(char* command, char* args[]){
    if (strcmp(command, "exit") == 0) {
        should_run = 0;
        printf("Bye ~ \n");
        return 0;
    }
    if (strcmp(command, "history") == 0) {
        printf("history:\nindex\tcommand\n");
        for (int i = 0; i < (command_cnt < 10 ? command_cnt : 10); i++) {
            printf(
                    "%d\t%s\n",
                    history_list[(history_head + history_len - i) % MAX_HISTORY].index,
                    history_list[(history_head + history_len - i) % MAX_HISTORY].command
            );
        }
        return 0;
    }
    //printf("args:%s\n", args[0]);
    return 1;
}

void run_command(char* command, char* args[], int args_cnt){
    int background = 0;
    if (strcmp(args[args_cnt - 1], "&") == 0){
        args[args_cnt - 1] = NULL;
        background = 1;
    }
    if (run_inner_command(command, args)){
        pid_t pid = fork();
        if (pid < 0){
            printf("Unable to fork child process!\n");
        }
        else
            if (pid == 0){
                fflush(stdout);
                if (execvp(command, args) == -1)
                    printf("Error when executing command %s\n", command);
                exit(0);
            }
            else {
                if (!background)
                    waitpid(pid, NULL, 0);
                else {
                    printf("PID %d: Child process run background.\n", pid);
                    fflush(stdout);
                }
            }
    }
}


void history_feature(char* command){
    if (history_len == 0){
        printf("No commands in history.\n");
        return;
    }
    int cnt = 0;
    if (command[1] == '!'){
        cnt = history_len;
    }
    else{
        for (int i = 1; i < strlen(command); i++){
            if (command[i] >= '0' && command[i] <= '9'){
                cnt *= 10;
                cnt += command[i] - '0';
            }
            else{
                puts("Unknown argument!");
                return;
            }
        }
    }
    if (cnt > history_len){
        printf("No such command in history.");
        return;
    }
    char* current_command = history_list[(history_head + cnt) % MAX_HISTORY].command;
    printf("history_command > %s\n", current_command);
    char* args[MAX_ARGS] = { 0 };
    char command_temp[MAX_LEN] = { 0 };
    int arg_cnt = 0;
    parse_args(current_command, command_temp, args, &arg_cnt);
    run_command(command_temp, args, arg_cnt);
}

int main(){
    while (should_run){
        printf("my_sh > ");
        fflush(stdout);
        char raw_command[MAX_LEN] = { 0 };
        fgets(raw_command, MAX_LEN, stdin);
        char* args[MAX_ARGS] = { 0 };
        char command[MAX_LEN] = { 0 };
        int args_cnt = 0;
        parse_args(raw_command, command, args, &args_cnt);
        if (strlen(command) && command[0] != '!'){
            create_history(raw_command);
        }
        if (command[0] == '!' && strlen(command) > 1) {
            history_feature(command);
            continue;
        }
        if (args_cnt)
            run_command(command, args, args_cnt);
    }
    return 0;
}
