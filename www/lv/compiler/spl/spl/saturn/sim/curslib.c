/*
 * curslib.c -- interface to curses library
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <curses.h>
#include <unistd.h>

#include "curslib.h"


static char inputLine[LINE_SIZE];
static char historyBuffer[HISTORY_SIZE][LINE_SIZE];
static int nextHistoryLine = 0;


int cGetChar(void) {
  int c;

  c = getch();
  if (c == ERR) {
    return -1;
  }
  switch (c) {
    case KEY_BACKSPACE:
      return 'H' & ~0x40;
    case KEY_LEFT:
      return 'B' & ~0x40;
    case KEY_RIGHT:
      return 'F' & ~0x40;
    case KEY_UP:
      return 'P' & ~0x40;
    case KEY_DOWN:
      return 'N' & ~0x40;
    default:
      return c;
  }
}


static void redrawInputLine(int y, int xStart, int xMax,
                            int xFinal, int size) {
  int x;
  int i;

  move(y, xStart);
  x = xStart;
  for (i = 0; i < size; i++) {
    addch(inputLine[i]);
    x++;
  }
  while (x < xMax - 1) {
    addch(' ');
    x++;
  }
  move(y, xFinal);
}


char *cGetLine(char *prompt) {
  int startx, starty;
  int maxx, maxy;
  int currentIndex, nextIndex;
  int insertMode;
  int currentHistoryLine;
  int running;
  int c;
  int i;

  cPrintf(prompt);
  getyx(stdscr, starty, startx);
  getmaxyx(stdscr, maxy, maxx);
  currentIndex = 0;
  nextIndex = 0;
  insertMode = 1;
  currentHistoryLine = nextHistoryLine;
  running = 1;
  while (running) {
    do {
      usleep(100);
      c = getch();
    } while (c == ERR) ;
    /* cPrintf("0x%02X\n", c); */
    switch (c) {
      case '\r':
      case '\n':
        /* return the current buffer to the caller */
        inputLine[nextIndex++] = '\n';
        running = 0;
        break;
      case 'H' & ~0x40:
      case 0x7F:
      case KEY_BACKSPACE:
        /* delete the character to the left of the cursor */
        if (currentIndex == 0) {
          beep();
        } else {
          currentIndex--;
          nextIndex--;
          for (i = currentIndex; i < nextIndex; i++) {
            inputLine[i] = inputLine[i + 1];
          }
          redrawInputLine(starty, startx, maxx,
                          startx + currentIndex, nextIndex);
        }
        break;
      case 'D' & ~0x40:
        /* delete the character under the cursor */
        if (nextIndex == 0) {
          /* ATTENTION: ^D on an empty line means EOF */
          running = 0;
          break;
        }
        if (currentIndex == nextIndex) {
          beep();
        } else {
          nextIndex--;
          for (i = currentIndex; i < nextIndex; i++) {
            inputLine[i] = inputLine[i + 1];
          }
          redrawInputLine(starty, startx, maxx,
                          startx + currentIndex, nextIndex);
        }
        break;
      case 'K' & ~0x40:
        /* kill from the cursor to the end of line */
        nextIndex = currentIndex;
        redrawInputLine(starty, startx, maxx,
                        startx + currentIndex, nextIndex);
        break;
      case 'U' & ~0x40:
        /* delete the entire line */
        currentIndex = 0;
        nextIndex = currentIndex;
        redrawInputLine(starty, startx, maxx,
                        startx + currentIndex, nextIndex);
        break;
      case 'O' & ~0x40:
        /* toggle overwrite/insert mode */
        insertMode ^= 1;
        break;
      case 'B' & ~0x40:
      case KEY_LEFT:
        /* move cursor backward */
        if (currentIndex == 0) {
          beep();
        } else {
          currentIndex--;
          move(starty, startx + currentIndex);
        }
        break;
      case 'F' & ~0x40:
      case KEY_RIGHT:
        /* move cursor forward */
        if (currentIndex == nextIndex) {
          beep();
        } else {
          currentIndex++;
          move(starty, startx + currentIndex);
        }
        break;
      case 'A' & ~0x40:
        /* move cursor to beginning of line */
        currentIndex = 0;
        move(starty, startx + currentIndex);
        break;
      case 'E' & ~0x40:
        /* move cursor to end of line */
        currentIndex = nextIndex;
        move(starty, startx + currentIndex);
        break;
      case 'P' & ~0x40:
      case KEY_UP:
        /* move to previous item on history list */
        if (currentHistoryLine == -1) {
          beep();
        } else {
          currentHistoryLine--;
        }
        if (currentHistoryLine == -1) {
          inputLine[0] = '\0';
        } else {
          strcpy(inputLine, historyBuffer[currentHistoryLine]);
        }
        nextIndex = strlen(inputLine);
        currentIndex = nextIndex;
        redrawInputLine(starty, startx, maxx,
                        startx + currentIndex, nextIndex);
        break;
      case 'N' & ~0x40:
      case KEY_DOWN:
        /* move to next item on history list */
        if (currentHistoryLine == nextHistoryLine) {
          beep();
        } else {
          currentHistoryLine++;
        }
        if (currentHistoryLine == nextHistoryLine) {
          inputLine[0] = '\0';
        } else {
          strcpy(inputLine, historyBuffer[currentHistoryLine]);
        }
        nextIndex = strlen(inputLine);
        currentIndex = nextIndex;
        redrawInputLine(starty, startx, maxx,
                        startx + currentIndex, nextIndex);
        break;
      default:
        /* add printable character to line */
        if (c == '\t') {
          /* turn tab into blank */
          c = ' ';
        }
        if (c < 0x20 || c > 0x7E) {
          /* ignore any other ctrl character */
          break;
        }
        if (insertMode || currentIndex == nextIndex) {
          /* shift rest of line right by one char */
          if (startx + nextIndex >= maxx - 1) {
            /* line is full */
            beep();
            break;
          }
          for (i = nextIndex; i > currentIndex; i--) {
            inputLine[i] = inputLine[i - 1];
          }
          nextIndex++;
        }
        inputLine[currentIndex++] = c;
        redrawInputLine(starty, startx, maxx,
                        startx + currentIndex, nextIndex);
        break;
    }
    refresh();
  }
  inputLine[nextIndex] = '\0';
  cPutChar('\r');
  cPutChar('\n');
  return inputLine;
}


void cAddHist(char *line) {
  char buffer[LINE_SIZE];
  char *p;
  int i;

  strcpy(buffer, line);
  p = strchr(buffer, '\n');
  if (p != NULL) {
    /* strip newline (and possibly rest of line) */
    *p = '\0';
  }
  if (buffer[0] == '\0') {
    /* do not add an empty line */
    return;
  }
  if (nextHistoryLine != 0 &&
      strcmp(historyBuffer[nextHistoryLine - 1], buffer) == 0) {
    /* do not add line if same as last one */
    return;
  }
  strcpy(historyBuffer[nextHistoryLine], buffer);
  nextHistoryLine++;
  if (nextHistoryLine == HISTORY_SIZE) {
    for (i = 1; i < HISTORY_SIZE; i++) {
      strcpy(historyBuffer[i - 1], historyBuffer[i]);
    }
    nextHistoryLine--;
  }
}


static void putCharWithoutRefresh(char c) {
  int x, y;
  int maxx, maxy;

  switch (c) {
    case 0x07:
      /* beep */
      beep();
      break;
    case 0x0D:
      /* carriage return */
      getyx(stdscr, y, x);
      move(y, 0);
      break;
    case 0x0A:
      /* line feed */
      getyx(stdscr, y, x);
      getmaxyx(stdscr, maxy, maxx);
      if (y == maxy - 1) {
        scroll(stdscr);
      } else {
        move(y + 1, x);
      }
      break;
    case 0x02:
      /* cursor left */
      getyx(stdscr, y, x);
      if (x == 0) {
        break;
      }
      move(y, x - 1);
      break;
    case 0x06:
      /* cursor right */
      getyx(stdscr, y, x);
      getmaxyx(stdscr, maxy, maxx);
      if (x == maxx - 1) {
        break;
      }
      move(y, x + 1);
      break;
    case 0x10:
      /* cursor up */
      getyx(stdscr, y, x);
      if (y == 0) {
        break;
      }
      move(y - 1, x);
      break;
    case 0x0E:
      /* cursor down */
      getyx(stdscr, y, x);
      getmaxyx(stdscr, maxy, maxx);
      if (y == maxy - 1) {
        break;
      }
      move(y + 1, x);
      break;
    case 0x08:
      /* backspace */
      getyx(stdscr, y, x);
      if (x == 0) {
        break;
      }
      move(y, x - 1);
      addch(' ');
      move(y, x - 1);
      break;
    case 0x09:
      /* horizontal tab */
      getyx(stdscr, y, x);
      getmaxyx(stdscr, maxy, maxx);
      do {
        addch(' ');
        x++;
      } while (x % 8 != 0 && x < maxx);
      break;
    default:
      /* add printable character to line */
      if (c < 0x20 || c > 0x7E) {
        /* ignore any other ctrl character */
        break;
      }
      addch(c);
      break;
  }
}


void cPrintf(char *format, ...) {
  va_list ap;
  char buffer[LINE_SIZE];
  char *p;

  va_start(ap, format);
  vsprintf(buffer, format, ap);
  va_end(ap);
  p = buffer;
  while (*p != '\0') {
    if (*p == '\n') {
      putCharWithoutRefresh('\r');
    }
    putCharWithoutRefresh(*p++);
  }
  refresh();
}


void cPutChar(char c) {
  putCharWithoutRefresh(c);
  refresh();
}


void cGotoXY(int x, int y) {
  /* upper left-hand corner is (0,0) */
  move(y, x);
  refresh();
}


void cClear(void) {
  clear();
  refresh();
}


void cHighlightOn(void) {
  attron(A_REVERSE);
}


void cHighlightOff(void) {
  attroff(A_REVERSE);
}


void cInit(void) {
  initscr();
  raw();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr, TRUE);
  scrollok(stdscr, TRUE);
  nodelay(stdscr, TRUE);
}


void cExit(void) {
  endwin();
}
