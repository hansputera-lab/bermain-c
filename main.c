#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "updates/webhook.h"

void printExampleUsage(const char* executableName)
{
    printf("USAGE   : %s <update-type> [port]\n", executableName);
    printf("POOLING : %s pool\n", executableName);
    printf("Webhook : %s webhook 3000\n", executableName);
    return;
}

// argc  = args total
// argv = args
int main(int argc, const char* argv[])
{
    if (argc > 3 || argc < 2) {
        printf("Please enter update-type, if you want use webhook update-type, you must fill port in 2nd args\n");
        printExampleUsage(argv[0]);
        exit(EXIT_FAILURE);
    }
    
    if (strcmp(argv[1], "pool") == 0) {
        printf("Pool Unsupported!\n");
        exit(EXIT_FAILURE);
    } else if (strcmp(argv[1], "webhook") == 0 && argc > 2) {
        int *port;
        
        *port = atoi(argv[2]);
        if (*port < 1 || *port < 1000) {
            printf("Couldn't parse the port or the port is lower than 1000!\n");
            exit(EXIT_FAILURE);
        } else {
            webhookUpdates(port);
        }
    } else {
        printExampleUsage(argv[0]);
    }
    
    return 0;
}