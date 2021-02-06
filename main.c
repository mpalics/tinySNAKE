#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

#define GAME_SPEED 10
#define STARTING_SNAKE_SIZE 5
#define WIN_CONDITION 2

#define SNAKE_CHAR '#'
#define FRUIT_CHAR 'O'

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
    Point pos[80];
    Point fruit;
    int next;
    int snake_size;
    int points;
} Handler;

void makefruit(Handler *ptr_handler) {
    do {
        //(rand() % (10 - 2 + 1)) + 2;
        ptr_handler->fruit.y = (rand() % 8) + 2;
        ptr_handler->fruit.x = (rand() % 8) + 2;
    } while(mvwinch(stdscr, ptr_handler->fruit.y ,ptr_handler->fruit.x) == SNAKE_CHAR);
    
    mvwaddch(stdscr, ptr_handler->fruit.y, ptr_handler->fruit.x, FRUIT_CHAR);
}

bool IsDirectionalKey(char c) {
        return (c == RIGHT_KEY || c == LEFT_KEY || c == UP_KEY || c == DOWN_KEY) ? true : false;
}

bool input_handling(Point *ptr_pos, Handler *ptr_handler) {
    int ch = getch();
    //while( (ch = getch()) == ERR) {}
        if (ch != ERR && IsDirectionalKey(ch)) {ptr_handler->lastPressed = ch;}
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
        if(ptr_handler->next>ptr_handler->snake_size) {
            ptr_handler->next = 0;
        }
        mvwaddch(stdscr, ptr_handler->pos[ptr_handler->next].y, ptr_handler->pos[ptr_handler->next].x, ' ');
        
    }
    mvwaddch(stdscr, ptr_pos->y, ptr_pos->x, '#');
    if(ptr_pos->x == ptr_handler->fruit.x && ptr_pos->y == ptr_handler->fruit.y) 
        {
            ptr_handler->points++;
            ptr_handler->snake_size++; 
            makefruit(ptr_handler);
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

void debug(Handler *ptr_handler) {
    wmove(stdscr, 0,0);
    wprintw(stdscr, "tick: %3d, points: %2d, fruit: %2d,%2d, lastPressed: %c", 
    ptr_handler->ticks, ptr_handler->points, ptr_handler->fruit.x, ptr_handler->fruit.y, ptr_handler->lastPressed);
}

void checkwin(Handler *ptr_handler) {
    if (ptr_handler->points >= WIN_CONDITION)
    {
        nodelay(stdscr, FALSE);
        attron(A_BLINK);
        wprintw(stdscr, " YOU WIN!");
        getch();
        exit(0);
    } 
}

int main(int argc, char *argv[]) {
    time_t t;
    srand((unsigned) time(&t));
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
    makefruit(handler);
    //main loop
    while (input_handling(poz, handler)) {
        debug(handler);
        event_handling(poz, handler);
        tick(handler);
        checkwin(handler);
    }
    return 0;
}