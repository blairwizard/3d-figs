#include <stdio.h>
#include <math.h>
#include <string.h>
#include<unistd.h>

#define WIDTH 80
#define HEIGHT 30

int main(void)
{
    double R = 2.0;
    double r = 1.0;
    double pi = acos(-1.0);

    double K = 20.0;
    double K2 = 5.0;

    double A = 0.0;
    double B = 0.0;

    const char *brightness = ".,-~:;=!*#$@";

    while (1)
    {
        char screen[HEIGHT][WIDTH];
        double zbuf[HEIGHT][WIDTH];
        for (int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                screen[i][j] = ' ';
                zbuf[i][j] = 0.0;
            }
        }


        for (double theta = 0; theta < 2 * pi; theta += 0.07)
        {
            for (double phi = 0; phi < 2 * pi; phi += 0.02)
            {
                double ct = cos(theta);
                double st = sin(theta);
                double cp = cos(phi);
                double sp = sin(phi);

                double x = (R + r * ct) * cp;
                double y = (R + r * ct) * sp;
                double z = r * st;

                double nx = ct*cp;
                double ny = ct*sp;
                double nz = st;

                double y1 = y*cos(A) - z*sin(A);
                double z1 = y*sin(A) + z*cos(A);

                double x1 = x*cos(B) - y1*sin(B);
                double y2 = x*sin(B) + y1*cos(B);

                double ny1 = ny*cos(A) - nz*sin(A);
                double nz1 = ny*sin(A) + nz*cos(A);
                
                double nx1 = nx*cos(B) - ny1*sin(B);
                double ny2 = nx*sin(B) + ny1*cos(B);

                double lum = ny2-nz1;
                int index = lum<=0?0:(int)(8 * lum );

                double denominator = 1/(K2 + z1);

                int screenX = (int)(WIDTH / 2 + K * x1 * denominator);
                int screenY = (int)(HEIGHT / 2 - K * y2 * denominator);

                if (screenX >= 0 && screenX < WIDTH &&
                    screenY >= 0 && screenY < HEIGHT && denominator>zbuf[screenY][screenX])
                {
                    zbuf[screenY][screenX]=denominator;
                    screen[screenY][screenX] = brightness[index];
                }
	     }
        }
        printf("\x1b[H");
        for(int i = 0; i < HEIGHT; i++)
        {
            for (int j = 0; j < WIDTH; j++)
            {
                putchar(screen[i][j]);
            }
            putchar('\n');
        }
        usleep(30000);
        A += 0.05;
        B += 0.05;


    }

    return 0;
}

