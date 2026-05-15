#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <time.h>

enum direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct node
{
    int x;
    int y;
    struct node *next;
    struct node *prev;
    enum direction dir;
};

struct snake
{
    uint8_t length;
    struct node *head;
};

struct food
{
    uint8_t x;
    uint8_t y;
};

uint8_t map[20][20];
struct food *f = NULL;

struct food *food_gen();
void game_over();

struct snake *snake_create()
{
    struct snake *s = (struct snake *)malloc(sizeof(struct snake));
    s->length = 1;
    s->head = (struct node *)malloc(sizeof(struct node));
    s->head->next = s->head;
    s->head->prev = s->head;
    srand((uint8_t)time(NULL));
    s->head->x = rand() % 5 + 5;
    s->head->y = rand() % 5 + 5;
    return s;
}

void snake_eat(struct snake *s)
{
    if (s == NULL)
        return ;
    if (s->head->x == f->x && s->head->y == f->y)
    {
        struct node *n = (struct node *)malloc(sizeof(struct node));
        if (s->length == 1)
        {
            n->next = s->head;
            n->prev = s->head;
            s->head->next = n;
            s->head->prev = n;
        }
        else
        {
            n->prev = s->head->prev;
            n->next = s->head;
            s->head->prev->next = n;
            s->head->prev = n;
        }
        switch (n->prev->dir)
        {
            case UP:n->x = n->prev->x;n->y = n->prev->y + 1;break;
            case DOWN:n->x = n->prev->x;n->y = n->prev->y - 1;break;
            case LEFT:n->x = n->prev->x + 1;n->y = n->prev->y;break;
            case RIGHT:n->x = n->prev->x - 1;n->y = n->prev->y;break;
        }
        s->length++;
        f = food_gen();
    }
}

void snake_move(struct snake *s)
{
    switch (s->head->dir)
    {
        case UP:s->head->prev->x = s->head->x;s->head->prev->y = s->head->y - 1;break;
        case DOWN:s->head->prev->x = s->head->x;s->head->prev->y = s->head->y + 1;break;
        case LEFT:s->head->prev->x = s->head->x - 1;s->head->prev->y = s->head->y;break;
        case RIGHT:s->head->prev->x = s->head->x + 1;s->head->prev->y = s->head->y;break;
    }
    s->head = s->head->prev;
    if (s->head->x >= 20 || s->head->x <= -1 || s->head->y >= 20 || s->head->y <= -1)
        game_over();
    struct node *cur = s->head->next;
    for (uint8_t i = 0; i < s->length - 1; i++)
    {
        if (s->head->x == cur->x && s->head->y == cur->y)
            game_over();
        cur = cur->next;
    }
    s->head->dir = s->head->prev->dir;
    cur = s->head->next;
    for (uint8_t i = 0; i < s->length - 2; i++)
    {
        cur->next->dir = cur->dir;
        cur = cur->next;
    }
}

void map_draw(struct snake *s)
{
    struct node *cur = s->head;
    memset(map, 0, sizeof(map));
    for (uint8_t i = 0; i < s->length; i++)
    {
        map[cur->y][cur->x] = 1;
        cur = cur->next;
    }
    map[f->y][f->x] = 2;
    for (uint8_t i = 0; i < 20; i++)
    {
        for (uint8_t j = 0; j < 20; j++)
        {
            if (map[i][j] == 1)
                printf("#  ", map[i][j]);
            else if (map[i][j] == 2)
                printf("@  ", map[i][j]);
            else
                printf(".  ", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void snake_ctrl(struct snake *s)
{
    char temp;
    scanf(" %c", &temp);
    switch (temp)
    {
    case 'w':s->head->dir = UP;break;
    case 's':s->head->dir = DOWN;break;
    case 'a':s->head->dir = LEFT;break;
    case 'd':s->head->dir = RIGHT;break;
    case 'W':s->head->dir = UP;break;
    case 'S':s->head->dir = DOWN;break;
    case 'A':s->head->dir = LEFT;break;
    case 'D':s->head->dir = RIGHT;break;
    }
    snake_move(s);
    snake_eat(s);
    map_draw(s);
}

struct food *food_gen(struct snake *s)
{
    struct food *food = f;
    uint8_t regen = 0;
    do
    {
        regen = 0;
        food->x = rand() % 20;
        food->y = rand() % 20;
        struct node *cur = s->head;
        for (uint8_t i = 0; i < s->length; i++)
        {
            if (food->x == cur->x && food->y == cur->y)
            regen = 1;
            cur = cur->next;
        }
    }while(regen);
    return food;
}

void game_over()
{
    printf("Game Over\n");
    exit(0);
}

void print_snake(struct snake *s)
{
    if (s == NULL)
        return ;
    struct node *cur = s->head;
    for (uint8_t i = 0; i < s->length; i++)
    {
        printf("%d, %d\n", cur->x, cur->y);
        cur = cur->next;
    }
}

int main()
{
    struct snake *s = snake_create();
    f = (struct food *)malloc(sizeof(struct food ));
    f = food_gen(s);
    snake_eat(s);
    while (1)
    {
        snake_ctrl(s);
    }
    return 0;
}
