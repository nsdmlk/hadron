#include "shell.h"
#include "memory.h"

static int shell_x = 0;
static int shell_y = 1;

static void shell_putchar(char c) {
    char* video = (char*) 0xB8000;
    if (c == '\n') {
        shell_x = 0;
        shell_y++;
        return;
    }

    if (shell_x >= 80) {
        shell_x = 0;
        shell_y++;
    }

    video[(shell_y * 80 + shell_x) * 2] = c;
    video[(shell_y * 80 + shell_x) * 2 + 1] = 0x0F;
    shell_x++;
}

static void shell_print(const char* msg) {
    while (*msg) shell_putchar(*msg++);
}

static int strcmp(const char* a, const char* b) {
    while (*a && *a == *b) { a++; b++; }
    return *a - *b;
}

static void* memcpy(void* dest, const void* src, int n) {
    char* d = (char*) dest;
    const char* s = (const char*) src;
    for (int i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

static void shell_parse(const char* input, Command* cmd) {
    cmd->count = 0;
    
    int i = 0;
    while (*input && cmd->count < MAX_TOKENS) {
        while (*input == ' ') input++;
        if (!*input) break;
        
        i = 0;
        if (*input == '"') {
            input++;
            while (*input && *input != '"' && i < MAX_TOKEN_LEN - 1) {
                cmd->tokens[cmd->count][i++] = *input++;
            }
            if (*input == '"') input++;
        } else {
            while (*input && *input != ' ' && i < MAX_TOKEN_LEN - 1) {
                cmd->tokens[cmd->count][i++] = *input++;
            }
        }
        cmd->tokens[cmd->count][i] = '\0';
        cmd->count++;
    }
}

void shell_init(void) {
    
}

int shell_execute(const char* input, int current_y) {
    Command cmd;
    shell_parse(input, &cmd);
    if (cmd.count == 0) return 0;
    
    shell_x = 0;
    int shell_y_start = current_y;
    shell_y = current_y;
    char* name = cmd.tokens[0];
    
    if (strcmp(name, "hadron") == 0) {
        shell_print("Hadron OS v0.1");
    } else if (strcmp(name, "info") == 0) {
        shell_print("Commands:\n    hadron - check OS version\n    display - display 'text'\n    clear - clear window\n    mem - create emory block\n    off - kill terminal");
    } else if (strcmp(name, "display") == 0) {
        shell_print(cmd.count > 1 ? cmd.tokens[1] : "null");
    } else if (strcmp(name, "clear") == 0) {
        char* v = (char*) 0xB8000;
        for (int j = 0; j < 25 * 80 * 2; j++) v[j] = 0;
        shell_y = 0;
    } else if (strcmp(name, "off") == 0) {
        shell_print("Shutting down...");
        asm volatile("hlt");
    } else if (strcmp(name, "mem") == 0) {
        char* test = (char*) malloc(100);
        char* msg = "malloc OK ";
        for (int i = 0; msg[i]; i++) test[i] = msg[i];
        shell_print(test);
        free(test);
    } else {
        shell_print("Unknown command, use info to check available commands");
    }
    return shell_y - shell_y_start + 1;
}