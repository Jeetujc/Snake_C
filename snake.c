#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
int score = 0, y, x, snake_size = 5, foodx, foody, snake_speed = 150;
int posx[100], posy[100];
char direction = 'r';
void start();
void display()
{
    attron(A_BOLD);
    getmaxyx(stdscr, y, x);
    mvprintw(0, 1, "Score = %d    Enter Number to play Backspace to exit", score);
    for (int i = 0; i < x; i++)
    {
        for (int j = 1; j < y; j++)
        {
            if ((i == 0 || i == x - 1) && j != 1)
                mvaddch(j, i, '|');
            if ((j == 1 || j == y - 1) && (i != 0) && i != x - 1)
                mvaddch(j, i, '_');
        }
    }
    attroff(A_BOLD);
}
void welcome()
{
    mvprintw(2, 1, "Welcome");
    mvprintw(3, 1, "press 1 for easy level");
    mvprintw(4, 1, "press 2 for hard level");

    refresh();
}

void snake()
{
    mvprintw(0, 1, "Score = %d    Enter Space to Pause                   ", score);
    int startx = 5, starty = 5;
    for (int i = 0; i < snake_size; i++)
    {
        posx[i] = startx - i;
        posy[i] = starty;
    }
}
int key()
{
    int k = getch();
    if (k == KEY_UP && direction != 'd')
        direction = 'u';
    if (k == KEY_DOWN && direction != 'u')
        direction = 'd';
    if (k == KEY_LEFT && direction != 'r')
        direction = 'l';
    if (k == KEY_RIGHT && direction != 'l')
        direction = 'r';
    if (k == ' ')
    {
        mvprintw(0, 1, "Score = %d    Enter Space to Play                   ", score);
        nodelay(stdscr, false);
        while (getch() != ' ')
            ;
        mvprintw(0, 1, "Score = %d    Enter Space to Pause                ", score);
        nodelay(stdscr, true);
    }
    return k;
}
void move_snake()
{
    mvaddch(posy[snake_size - 1], posx[snake_size - 1], ' ');

    for (int i = snake_size - 1; i > 0; i--)
    {
        posx[i] = posx[i - 1];
        posy[i] = posy[i - 1];
    }

    if (direction == 'r')
        posx[0]++;
    if (direction == 'l')
        posx[0]--;
    if (direction == 'u')
        posy[0]--;
    if (direction == 'd')
        posy[0]++;

    for (int i = 0; i < snake_size; i++)
    {
        mvprintw(posy[i], posx[i], ".");
    }

    refresh();
    napms(snake_speed);
}

void food()
{
    srand(time(NULL));
    foodx = 1 + rand() % (x - 2 - 1);
    foody = 2 + rand() % (y - 2 - 2);
    mvaddch(foody, foodx, '@');
    refresh();
}

void gameover()
{
    clear();
    const char *msg[] = {
        " ######      ###    ##     ## ########    #######  ##     ## ######## ########  ",
        "##    ##    ## ##   ###   ### ##         ##     ## ##     ## ##       ##     ## ",
        "##         ##   ##  #### #### ##         ##     ## ##     ## ##       ##     ## ",
        "##   #### ##     ## ## ### ## ######     ##     ## ##     ## ######   ########  ",
        "##    ##  ######### ##     ## ##         ##     ##  ##   ##  ##       ##   ##   ",
        "##    ##  ##     ## ##     ## ##         ##     ##   ## ##   ##       ##    ##  ",
        "  ######   ##     ## ##     ## ########    #######     ###    ######## ##     ##  "};
    int lines = sizeof(msg) / sizeof(msg[0]);

    for (int i = 0; i < lines; i++)
    {
        mvprintw(y / 2 - lines / 2 + i, (x - strlen(msg[i])) / 2, "%s", msg[i]);
    }

    mvprintw(y / 2 + lines / 2 + 1, (x - 20) / 2, "Your score: %d", score);
    mvprintw(y / 2 + lines / 2 + 3, (x - 40) / 2, "Press SPACE to restart or BACKSPACE to exit");
    refresh();

    int ch;
    nodelay(stdscr, false);
    while ((ch = getch()))
    {
        if (ch == ' ')
        {
            clear();
            start();
        }
        if (ch == KEY_BACKSPACE || ch == 127)
        {
            endwin();
            exit(0);
        }
    }
}

void collide_detection()
{
    if (posx[0] == 0 || posx[0] == x - 1 || posy[0] == 1 || posy[0] == y - 1)
        gameover();

    for (int i = 1; i < snake_size; i++)
    {
        if (posx[0] == posx[i] && posy[0] == posy[i])
        {
            gameover();
        }
    }

    if (posx[0] == foodx && posy[0] == foody)
    {
        score++;
        mvprintw(0, 9, "%d", score);
        food();
        snake_size++;
    }
}
void start()
{   
    snake_size=5;
    score=0;
    snake_speed=150;
    direction='r';
    welcome();
    display();
    int g = getch();
    if (g == '1' || g == '2')
    {
        nodelay(stdscr, true);
        if (g == '2')
            snake_speed = 40;
        clear();
        display();
        snake();
        food();
        while (key() != KEY_BACKSPACE)
        {

            move_snake();
            collide_detection();
        }
    }
}
int main()
{   
    
    initscr();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(FALSE);

    start();
    endwin();
    return 0;
}
