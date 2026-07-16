#include "keyboard.h"
#include "shell.h"

#define BUFFER_SIZE 256
static char buffer[BUFFER_SIZE];
static int buf_pos = 0;
static int cursor_x = 0;
static int cursor_y = 0;
static int shift_pressed = 0;

static char shift_table[128] = {
    0,   0,   '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
    0,   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, 0, 0, ' ', 0
};

static char normal_table[128] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
    0,   'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0
};

char scancode_to_ascii(unsigned char scancode) {
    if (scancode < 128)
        if (shift_pressed == 0) {
            return normal_table[scancode];
        } else {
            return shift_table[scancode];
        }
    return 0;
}

static void scroll_screen() {
    char* video = (char*) 0xB8000;
    for (int y = 0; y < 24; y++) {
        for (int x = 0; x < 80; x++) {
            video[(y * 80 + x) * 2] = video[((y + 1) * 80 + x) * 2];
            video[(y * 80 + x) * 2 + 1] = video[((y + 1) * 80 + x) * 2 + 1];
        }
    }
    for (int x = 0; x < 80; x++) {
        video[(24 * 80 + x) * 2] = ' ';
        video[(24 * 80 + x) * 2 + 1] = 0x0F;
    }
    cursor_y = 24;
}

static void putchar_at(char c, int x, int y) {
    char* video = (char*) 0xB8000;
    video[(y * 80 + x) * 2] = c;
    video[(y * 80 + x) * 2 + 1] = 0x0F;
}

void keyboard_init(void) {
    char* prompt = "hadron@shell$ |  ";
    for (int i = 0; prompt[i]; i++) {
        putchar_at(prompt[i], cursor_x++, 0);
    }
}

void keyboard_handler(unsigned char scancode) {

    if (scancode == 0x2A || scancode == 0x36) {  // Left or Right Shift pressed
        shift_pressed = 1;
        return;
    }
    if (scancode == 0xAA || scancode == 0xB6) {  // Left or Right Shift released
        shift_pressed = 0;
        return;
    }
    if (scancode & 0x80) return;

    char c = scancode_to_ascii(scancode);
    
    if (scancode == 0x0E) { // backspace
        if (buf_pos > 0) {
            buf_pos--;
            if (cursor_x > 0) {
                cursor_x--;
            } else if (cursor_y > 0) {
                cursor_y--;
                cursor_x = 79;
            }
            putchar_at(' ', cursor_x, cursor_y);
        }
    } else if (c == '\n') {  // Enter
        buffer[buf_pos] = '\0';
        int line_used = shell_execute(buffer, cursor_y + 1);      
        buf_pos = 0;
        
        for (int j = 0; j < buf_pos; j++) buffer[j] = '\0';
        buf_pos = 0;
    
        cursor_x = 0;
        cursor_y = cursor_y + line_used + 1;
        if (cursor_y >= 25) scroll_screen();
        char* prompt = "hadron@shell$ |  ";
        for (int i = 0; prompt[i]; i++) {
            putchar_at(prompt[i], cursor_x++, cursor_y);
        }
    } else if (c) {  // regular symbols
        if (buf_pos < BUFFER_SIZE - 1) {
            buffer[buf_pos++] = c;
            putchar_at(c, cursor_x, cursor_y);
            cursor_x++;
            if (cursor_x >= 80) {
                cursor_x = 0;
                cursor_y++;
                if (cursor_y >= 25) scroll_screen();
            }
        }
    }
    putchar_at('_', cursor_x, cursor_y);
}
