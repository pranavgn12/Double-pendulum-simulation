#include "raylib.h"
#include <math.h>
#include <stdio.h>
#define pivot_x 400
#define pivot_y 200
#define rod_1 150
#define rod_2 150
#define m1 1
#define m2 2
#define g 10
#define w1 (g * m1)
#define w2 (g * m2)
#define pi M_PI
#define trail_size 2000
double v1,v2;
double a1,a2;
double acc1,acc2;

int trail_x[trail_size];
int trail_y[trail_size];
int n;
int solve(double a1, double b1, double c1,
          double a2, double b2, double c2,
          double *x, double *y);
struct point{
    double x;
    double y;
}p1,p2;


void init(){
    // p1.x = pivot_x+rod_1;
    // p1.y = pivot_y;
    // p2.x =p1.x + rod_2;
    // p2.y= p1.y;
    acc1 =0;
    acc2 =0;
    v1 = 0;
    v2 = 0;
    a1 = pi/2;
    a2 = pi/2;
    int n=0;
    for(int i=0; i<trail_size; i++)
    {
        trail_x[i] = 0;
        trail_y[i] = 0;
    }
}

void render(){
    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int i = 0; i < trail_size; i++) {

        int age = (n - i -1 + trail_size) % trail_size;

        float op = 255.0f * (1.0f - (float)age / trail_size);

        Color c = (Color){255, 150, 250, (unsigned char)op};

        DrawPixel(trail_x[i],     trail_y[i],     c);
        DrawPixel(trail_x[i] + 1, trail_y[i],     c);
        DrawPixel(trail_x[i] - 1, trail_y[i],     c);
        DrawPixel(trail_x[i] + 1, trail_y[i] + 1, c);
        DrawPixel(trail_x[i] + 1, trail_y[i] - 1, c);
        DrawPixel(trail_x[i] - 1, trail_y[i] - 1, c);
        DrawPixel(trail_x[i] - 1, trail_y[i] + 1, c);
        DrawPixel(trail_x[i],     trail_y[i] + 1, c);
        DrawPixel(trail_x[i],     trail_y[i] - 1, c);
    }

    DrawLineEx((Vector2) {pivot_x,pivot_y}, (Vector2) {p1.x,p1.y}, 4, GRAY);
    DrawLineEx((Vector2) {p1.x,p1.y}, (Vector2) {p2.x,p2.y}, 4, GRAY);
    DrawCircle(pivot_x, pivot_y, 5, BLACK);
    DrawCircle(p1.x, p1.y, 10*m1, (Color){245, 66, 66, 255});
    DrawCircle(p2.x, p2.y, 10*m2, (Color){66, 138, 245, 255});
    EndDrawing();

}

// double tan_q(double x, double y){
//     if(x > 0 && y >= 0)
//         return -1 * atanf(y/x);
//     else if(x == 0 && y > 0)
//         return -1 * pi / 2;
//     else if(x < 0 && y >= 0)
//         return -1 * (pi + atan(y/x));
//     else if(x < 0 && y <= 0)
//         return pi - atanf(y/x);
//     else if(x == 0 && y < 0)
//         return pi/2;
// }



void calculate_pos(){
    double dt = 0.01;
    for (int i = 0; i < 16; i++)
    {
    double c1 = m2 * rod_2 * v2 * v2 * sinf(a2-a1) - (m1+m2) * g * sin(a1);
    double d1 = (m1+m2) * rod_1;
    double b1 = m2 * rod_2 * cosf(a2-a1);

    double c2 = -rod_1 * v1 * v1 * sinf(a2-a1) - g* sinf(a2);
    double d2 = rod_1 * cosf(a2-a1);
    double b2 = rod_2;

    solve(d1, b1, c1, d2, b2, c2, &acc1, &acc2);

    v1+=acc1 * dt;
    v2+=acc2 * dt;

    a1+=v1 * dt;
    a2+=v2 * dt;

    p1.x = pivot_x + rod_1 * sinf(a1);
    p1.y = pivot_y + rod_1 * cosf(a1);
    p2.x = p1.x + rod_2 * sinf(a2);
    p2.y = p1.y + rod_2 * cosf(a2);

    trail_x[n] = p2.x;
    trail_y[n] = p2.y;

    n++;
    if(n == trail_size){
        n=0;
    }
    }
}



int solve(double A, double B, double C,
          double D, double E, double F,
          double *x, double *y)
{
    double determinant = A * E - D * B;

    // Avoid division by a value extremely close to zero
    if (fabs(determinant) < 1e-12)
        return 0;

    *x = (C * E - F * B) / determinant;
    *y = (A * F - D * C) / determinant;

    return 1;
}

int main(void)
{
    InitWindow(800, 800, "Double Pendulum Simulation");
    SetTargetFPS(60);
    init();


    while (!WindowShouldClose())
    {
        calculate_pos();
        render();
    }

    CloseWindow();

    return 0;
}
