#include "cstdio"
#include "cmath"

struct Coordinate
{
    Coordinate()
    {}
    Coordinate( double hour )
    {
        hAll = hour;
        h = (int)hour;
        m = (int)((hour - h) * 60);
        s = ((hour - h) * 60 - m) * 60;
    }
    Coordinate( int hour, int minute, double second )
    {
        h = hour;
        m = minute;
        s = second;
        hAll = h + m / 60.0 + s / 3600.0;
    }
    int h, m;
    double s;
    double hAll;
};

Coordinate crdtsYB[4];
Coordinate yourTime;

Coordinate calculate()
{
    Coordinate delta1_01( (crdtsYB[0].hAll + crdtsYB[1].hAll) / 2 );
    Coordinate delta1_12( (crdtsYB[1].hAll + crdtsYB[2].hAll) / 2 );
    Coordinate delta1_23( (crdtsYB[2].hAll + crdtsYB[3].hAll) / 2 );

    Coordinate delta2_0112( (delta1_01.hAll + delta1_12.hAll) / 2 );
    Coordinate delta2_1223( (delta1_12.hAll + delta1_23.hAll) / 2 );

    Coordinate delta3_01121223((delta2_0112.hAll + delta2_1223.hAll) / 2 );

    double n = yourTime.hAll / 24;
    double res = crdtsYB[1].hAll + n * delta1_12.hAll;
    res += n * (n - 1) /2 * delta3_01121223.hAll;

    return Coordinate( res );
}

int main()
{
    yourTime = Coordinate( 12, 0, 0.0 );
    crdtsYB[0] = Coordinate( 1, 1, 47.27 );
    crdtsYB[1] = Coordinate( 1, 5, 26.9 );
    crdtsYB[2] = Coordinate( 1, 9, 6.77 );
    crdtsYB[3] = Coordinate( 1, 12, 46.86 );

    Coordinate res = calculate();
    printf( "%d %d %.2f\n", res.h, res.m, res.s );

    return 0;
}
