struct request_field {
    char *key;
    char *value;
};

void parse_request_buff(struct request_field fields[], char *buf[512]);
