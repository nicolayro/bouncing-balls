#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH    64
#define HEIGHT   32
#define C_COLOR  'C'
#define BG_COLOR '`'

#define FPS     30
#define GRAVITY 150.0f
#define MS      1000
#define DT      (1.0f/FPS)
#define BOUNCE  0.9f

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

char *screen;

typedef struct {
    float x;
    float y;
} Vec2;

typedef struct {
    int r;
    Vec2 pos;
    Vec2 vel;
} Circle;

void fill(char c)
{
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            screen[j * WIDTH + i] = c;
        }
    }
}

void add_circle(Circle *c)
{
    int begin_x = MAX(0, c->pos.x - c->r);
    int begin_y = MAX(0, c->pos.y - c->r);

    int end_x = MIN(WIDTH, c->pos.x + c->r);
    int end_y = MIN(HEIGHT, c->pos.y + c->r);

    for (int y = begin_y; y < end_y; y++) {
        for (int x = begin_x; x < end_x; x++) {
            float d_x = c->pos.x - (x + 0.5f);
            float d_y = c->pos.y - (y + 0.5f);
            if (d_x*d_x + d_y*d_y <= c->r*c->r) {
                screen[y * WIDTH + x] = C_COLOR;
            }
        }
    }
}

void move_circle(Circle *c)
{
    c->vel.x += DT * 0;
    c->pos.x += DT * c->vel.x;

    c->vel.y += DT * GRAVITY;
    c->pos.y += DT * c->vel.y;

    if (c->pos.x >= WIDTH - c->r) {
        c->pos.x = WIDTH - c->r;
        c->vel.x = -c->vel.x * BOUNCE;
    }

    if (c->pos.x <= 0 + c->r) {
        c->pos.x = 0 + c->r;
        c->vel.x = -c->vel.x * BOUNCE;
    }

    if (c->pos.y >= HEIGHT - c->r) {
        c->pos.y = HEIGHT - c->r;
        c->vel.y = -c->vel.y * BOUNCE;
    }

    if (c->pos.y <= 0 + c->r) {
        c->pos.y = 0 + c->r;
        c->vel.y = -c->vel.y * BOUNCE;
    }
}

void reset_cursor()
{
    printf("\x1b[%dA", HEIGHT);
    printf("\x1b[%dD", WIDTH);
}

void render()
{
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            printf("%c ", screen[j * WIDTH + i]);
        }
        printf("\n");
    }

    reset_cursor();
}


Circle circles[3] = {
    {
        .r   = 3,
        .pos = { 2.0f, 50.0f },
        .vel = { 50.0f, -120.0f }
    },
    {
        .r   = 5,
        .pos = { 20.0f, 0.0f },
        .vel = { 30.0f, 0.0f }
    },
    {
        .r   = 7,
        .pos = { 50.0f, 10.0f },
        .vel = { -10.0f, 0.0f }
    }
};

int main()
{
    screen = malloc(HEIGHT * WIDTH * sizeof(char));

    for (;;) {
        fill(BG_COLOR);

        for (int i = 0; i < 3; i++) {
            move_circle(&circles[i]);
            add_circle(&circles[i]);
        }
        render();

        usleep(FPS * MS);
    }

    return 0;
}
