#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "fdfs_api.h"

int main(int argc, char* argv[])
{
    char file_id[1024] = {0};
    fdfs_upload_file("/etc/fdfs/client.conf", argv[1], file_id);
    printf("File ID: %s\n", file_id);

    return 0;
}