#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFFSIZE 4096

void print_usage(){
    printf("Usage：cp src_file dest_file\n");
}

int main(int argc, char** argv){
    if (argc != 3){
        print_usage();
        return 1;
    }
    int src_fd;
    int dst_fd;
    if ((src_fd = open(argv[1], O_RDONLY)) == -1){
        printf("Error: Unable to open file %s\n", argv[1]);
        return 1;
    }
    if ((dst_fd = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1){
        printf("Error: Unable to write to file %s\n", argv[2]);
        close(src_fd);
        return 1;
    }
    int cnt = 0;
    char buffer[BUFFSIZE] = { 0 };
    while ((cnt = read(src_fd, buffer, BUFFSIZE)) > 0){
        if (write(dst_fd, buffer, cnt) != cnt){
            printf("Error: Problems occurred write to file %s\n", argv[2]);
            close(src_fd);
            close(dst_fd);
            return 1;
        }
    }
    close(src_fd);
    close(dst_fd);
    return 0;
}