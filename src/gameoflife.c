#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define for_x for (int x = 0; x < w; x++)
#define for_y for (int y = 0; y < h; y++)
#define for_xy for_x for_y
void show(int **univ, int w, int h)
{
    printf("\033[H");
    for_y
    {
        for_x printf(univ[y][x] ? "\033[07m  \033[m" : "  ");
        printf("\033[E");
    }
    fflush(stdout);
}

void evolve(int **univ, int w, int h)
{

    int **new = malloc(h * sizeof(*new));
    for_x new[x] = malloc(w * sizeof(*new[x]));
    for_y for_x new[y][x] = univ[y][x];

    #pragma omp parallel for collapse(2)
    for_y for_x
    {
        int n = 0;
        for (int y1 = y - 1; y1 <= y + 1; y1++)
            for (int x1 = x - 1; x1 <= x + 1; x1++)
                if (univ[(y1 + h) % h][(x1 + w) % w])
                    n++;

        if (univ[y][x])
            n--;
        new[y][x] = (n == 3 || (n == 2 && univ[y][x]));
    }
    for_y for_x univ[y][x] = new[y][x];
}

void game(int w, int h, int i)
{
    srand(666);

    int **univ = malloc(h * sizeof(*univ));
    for_y univ[y] = malloc(w * sizeof(*univ[y]));

    for_xy univ[y][x] = rand() < RAND_MAX / 10 ? 1 : 0;
    
    for (int j=0; j < i; j++)
        evolve(univ, w, h);

    FILE *fp;
    fp = fopen("output.txt", "w+");
    for_y
    {
        for_x
        {
            char str[12];
            sprintf(str, "%d", univ[y][x]);
            fputs(str, fp);
            fputs(" ", fp);
        }
        fputs("\n", fp);
    }
    fclose(fp);

    for_y free(univ[y]);
	free(univ);
}

int main(int c, char **v)
{
    printf("Threads available: %d\n", omp_get_max_threads());
    int w = 0, h = 0, i = 0;
    if (c > 1)
        w = atoi(v[1]);
    if (c > 2)
        h = atoi(v[2]);
    if (c > 3)
        i = atoi(v[3]);
    if (c > 4 && atoi(v[4]) >= 1 && atoi(v[4]) <= omp_get_max_threads())
	    omp_set_num_threads(atoi(v[4]));
    if (w <= 0)
        w = 1000;
    if (h <= 0)
        h = 1000;
    if (i <= 0)
        i = 100;
    game(w, h, i);
}
