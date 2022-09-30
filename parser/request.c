#include <string.h>
#include <stdio.h>
#include "request.h"

void parse_request_buf(
    struct request_field *fields[],
    char *buf[512]
) {
    char *token;

    token = strtok(*buf, "\n");
    while(token != NULL)
    {
        printf("%s\n", token);
        token = strtok(NULL, "\n");
    }
}