#ifndef SHELL_H
#define SHELL_H

#define MAX_TOKENS 16
#define MAX_TOKEN_LEN 64

typedef struct {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int count;
} Command;

void shell_init(void);
void shell_execute(const char* input, int current_y);

#endif