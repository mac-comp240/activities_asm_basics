

#include <stdio.h>

void swap(long *xp, long *yp);

int main() {
    long x = 25, y = 100;

    long *px = &x;
    long *py = &y;
    
    printf("Before swap: %li %li\n", x, y);
    swap(px, py);
    printf("After swap: %li %li\n", x, y);
    return 0;
}

// TODO: for the last part of the activity, you might copy the definition of
// swap here
void swap(long *xp, long *yp)
{
  long t0 = *xp;
  long t1 = *yp;
  *xp = t1;
  *yp = t0;
}
