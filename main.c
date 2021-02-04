#include <stdlib.h>
#include <ncurses.h>

#define GAME_SPEED 5

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
    int ticks;
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
    if(ptr_handler->ticks % GAME_SPEED == 0) {
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
            mvwaddch(stdscr, ptr_pos->y, ptr_pos->x, '#');    
        }
    }
}

void tick(Handler *ptr_handler) {
    ptr_handler->ticks++;
    if(ptr_handler->ticks > 100) {ptr_handler->ticks = 1;}
    wrefresh(stdscr);
    napms(15);
}

void init() {
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
}

int main(int argc, char *argv[]) {
    Point *poz = malloc(sizeof(Point));
    Handler *handler = malloc(sizeof(Handler));
    handler->lastPressed = '0';
    handler->ticks = 0;
    poz->x = 5;
    poz->y = 5;
    init();
    //main loop
    while (input_handling(poz, handler)) {
        wmove(stdscr, 0,0);
        wprintw(stdscr, "tick: %d", handler->ticks);
        event_handling(poz, handler);
        tick(handler);
    }
    return 0;
}