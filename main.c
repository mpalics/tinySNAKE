#include <stdlib.h>
#include <ncurses.h>

#define RIGHT_KEY 'd'
#define LEFT_KEY 'a'
#define UP_KEY 'w'
#define DOWN_KEY 's'

#define RIGHT ptr_pos->x += 1
#define LEFT ptr_pos->x -= 1
#define UP ptr_pos->y -= 1
#define DOWN ptr_pos->y += 1

typedef struct {
  int x, y;
} Point;

typedef struct {
    char lastPressed;
} Handler;

bool input_handling(Point *ptr_pos, Handler *ptr_handler) {
    int ch = getch();
    //while( (ch = getch()) == ERR) {}
        if (ch != ERR) {ptr_handler->lastPressed = ch;}
        if (ch == 'x') {
            return false;
        }
    return true;
}

void event_handling(Point *ptr_pos, Handler *ptr_handler) {
    mvwaddch(stdscr, ptr_pos->y, ptr_pos->x, ' ');
    switch(ptr_handler->lastPressed) {
        case LEFT_KEY:
            LEFT;
            break;
        case RIGHT_KEY:
            RIGHT;
            break;
        case UP_KEY:
            UP;
            break;
        case DOWN_KEY:
            DOWN;
            break;
        default:
            break;
        mvwaddch(stdscr, ptr_pos->y, ptr_pos->x, ACS_BLOCK);
    }
}

int main(int argc, char *argv[]) {
    Point *poz = malloc(sizeof(Point));
    Handler *handler = malloc(sizeof(Handler));
    handler->lastPressed = '0';
    poz->x = 5;
    poz->y = 5;
    int ticks = 0;
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    //main loop
    while (input_handling(poz, handler)) {
        wmove(stdscr, 0,0);
        wprintw(stdscr, "tick: %d", ticks);
        ticks++;
        event_handling(poz, handler);
        wrefresh(stdscr);
        napms(100);
    }
    return 0;
}