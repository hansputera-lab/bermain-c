#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/request.h"

int main()
{
    int i = 0;
    char text[] = "Host: localhost:3000\nUser-Agent: sadasdasd/sadsadasd\n\nHelloW";
    struct request_field *fields[] = {};

    char *token;
    token = (char*)malloc(10);

    token = strtok(text, "\n");
    while(token != NULL) {
        struct request_field *field;

        field = (struct request_field*)malloc(sizeof(struct request_field));

        field->key = "";
        field->value = "";

        char *token2;
        token2 = (char*)malloc(sizeof(token));

        token2 = strtok(token, ":");
        while(token2 != NULL) {
            if (strlen(field->key) < 1) {
                strcpy(field->key, token2);
            } else strcpy(field->value, token2);

            token2 = strtok(NULL, ":");
        }

        fields[i] = field;

        free(token2);
        token = strtok(NULL, "\n");
        i++;
    }

    free(token);
    return 0;
}