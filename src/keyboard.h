#ifndef KEYBOARD_H
#define KEYBOARD_H

void keyboard_handler(unsigned char scancode);
char scancode_to_ascii(unsigned char scancode);
void keyboard_init(void);

#endif