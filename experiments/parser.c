#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/request.h"

int main()
{
    int i = 0;
    char text[] = "Host: localhost:3000\nUser-Agent: sadasdasd/sadsadasd\n\nHelloW";
    struct request_field *fields[] = {};
    
    char *token = (char*)malloc(10);

    token = strtok(text, "\n");

    while(token != NULL) {
        struct request_field *field = (struct request_field *)malloc(sizeof(struct request_field));
        field->key = "";
        field->value = "";

        char *token2 = (char*)malloc(10);
        token2 = strtok(token, ":");

        while(token2 != NULL) {
            if (strlen(field->key) < 1) field->key = token2;
            else {
                if (strlen(field->value) > 0) {
                    // why i can't use 'strcat' two times here?!?!
                    strcat(field->value, token2);
                } else field->value = token2;

            }

            token2 = strtok(NULL, ":");
        }
        
        printf("%s valuenya %s\n", field->key, field->value);

        fields[i] = field;
        free(token2);
        token = strtok(NULL, "\n");
    }
    
    printf("total fields: %d", sizeof(fields)/sizeof(fields[0]));
    // printf("suki");
    // while(token != NULL) {
    //     // split ':'
    //     char *token2;
    //     struct request_field field;
    //     token2 = (char*)malloc(sizeof(token));

    //     token2 = strtok(token, ":");

    //     while(token2 != NULL) {
    //         if (strlen(field.key) < 1) {
    //             strcpy(field.key, token2);
    //         } else strcpy(field.value, token2);

    //         token2 = strtok(NULL, ":");
    //     }

    //     fields[i] = &field;        
    //     free(token2);

    //     token = strtok(NULL, "\n");
    // }

    // struct request_field *fields[] = {};
    // struct request_field field;

    // field.key = "Hello";
    // field.value = "holla";

    // fields[i] = &field;

    free(token);
    return 0;
}