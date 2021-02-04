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
    Point pos[10];
    int next;
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
        }
        Point p;
        p.x = ptr_pos->x;
        p.y = ptr_pos->y;
        ptr_handler->pos[ptr_handler->next] = p;
        ptr_handler->next++;
        if(ptr_handler->next>9) {
            ptr_handler->next = 0;
        }
        mvwaddch(stdscr, ptr_handler->pos[ptr_handler->next].y, ptr_handler->pos[ptr_handler->next].x, ' ');
        
    }
    mvwaddch(stdscr, ptr_pos->y, ptr_pos->x, '#');
    
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

void debug(Handler *ptr_handler) {
    wmove(stdscr, 0,0);
    wprintw(stdscr, "tick: %d", ptr_handler->ticks);
}

int main(int argc, char *argv[]) {
    Point *poz = malloc(sizeof(Point));
    Handler *handler = malloc(sizeof(Handler));
    handler->lastPressed = '0';
    handler->ticks = 0;
    handler->next = 0;
    poz->x = 5;
    poz->y = 5;
    init();
    //main loop
    while (input_handling(poz, handler)) {
        debug(handler);
        event_handling(poz, handler);
        tick(handler);
    }
    return 0;
}