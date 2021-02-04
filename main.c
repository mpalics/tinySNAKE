#include <stdlib.h>
#include <ncurses.h>

#define GAME_SPEED 5
#define STARTING_SNAKE_SIZE 5

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
    Point fruit;
    int next;
    int snake_size;
    int points;
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
        if(p.x == ptr_handler->fruit.x && p.x == ptr_handler->fruit.y) {ptr_handler->points++;}
        ptr_handler->pos[ptr_handler->next] = p;
        ptr_handler->next++;
        if(ptr_handler->next>ptr_handler->snake_size) {
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
    wprintw(stdscr, "tick: %3d, points: %2d", ptr_handler->ticks, ptr_handler->points);
}

int main(int argc, char *argv[]) {
    Point *poz = malloc(sizeof(Point));
    Handler *handler = malloc(sizeof(Handler));
    handler->lastPressed = '0';
    handler->ticks = 0;
    handler->next = 0;
    handler->points = 0;
    handler->snake_size = STARTING_SNAKE_SIZE;
    poz->x = 5;
    poz->y = 5;
    init();

    int fruit_x = (rand() % (40 - 2 + 1)) + 2;
    int fruit_y = (rand() % (20 - 2 + 1)) + 2;
    handler->fruit.x = fruit_x;
    handler->fruit.y = fruit_y;
    mvwaddch(stdscr, fruit_y, fruit_x, 'O');
    //main loop
    while (input_handling(poz, handler)) {
        debug(handler);
        event_handling(poz, handler);
        tick(handler);
    }
    return 0;
}