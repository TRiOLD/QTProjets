#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

string inToWord( int i )
{
    switch( i )
    {
    case 0:	return "";
    case 1:	return "One";
    case 2:	return "Two";
    case 3:	return "Three";
    case 4:	return "Four";
    case 5:	return "Five";
    case 6:	return "Six";
    case 7:	return "Seven";
    case 8:	return "Eight";
    case 9:	return "Nine";
    }
    return "";
}

string tnToWord( int i )
{
    switch( i )
    {
    case 0:	return "";
    case 2:	return "Twenty";
    case 3:	return "Thirty";
    case 4:	return "Fourty";
    case 5:	return "Fifty";
    case 6:	return "Sixty";
    case 7:	return "Seventy";
    case 8:	return "Eighty";
    case 9:	return "Ninety";
    }
    return "";
}

string teenToWord( int i )
{
    switch( i )
    {
    case 0:	return "";
    case 1:	return "Eleven";
    case 2:	return "Twelve";
    case 3:	return "Thirteen";
    case 4:	return "Fourteen";
    case 5:	return "Fifteen";
    case 6:	return "Sixteen";
    case 7:	return "Seventeen";
    case 8:	return "Eighteen";
    case 9:	return "Nineteen";
    }
    return "";
}

string rangToWord( int i )
{
    switch( i )
    {
    case 0:		return "";
    case 1:		return "Thousand";
    case 2:		return "Million";
    case 3:		return "Billion";
    }
    return "";
}

struct Thousand
{

    int hd;
    int tn;
    int in;
};

int main()
{
    cout << "Enter Integer: ";

    int value, temp;
    cin >> value;
    temp = value;

    int rang = 0;
    while( temp )
    {
        rang++;
        temp /= 10;
    }

    temp = value;
    int thdsRang = rang / 3;
    vector<Thousand> thds( thdsRang + 1 );
    for( int i = 0; i < thdsRang + 1; i++ )
    {
        thds[i].hd = temp / 100;
        thds[i].tn = temp / 10 - thds[i].hd * 10;
        thds[i].in = temp - thds[i].tn * 10 - thds[i].hd * 100;

        thds[i].hd %= 10;
        temp /= 1000;
    }

    for( int i = thdsRang; i >= 0; i-- )
    {
        if( thds[i].hd > 0 )
            cout << inToWord( thds[i].hd ) << " " << "Hundred" << " ";
        if( thds[i].tn > 1 )
            cout << tnToWord( thds[i].tn ) << " " << inToWord( thds[i].in ) << " ";
        if( thds[i].tn == 0 )
            cout << inToWord( thds[i].in ) << " ";
        if( thds[i].tn == 1 && thds[i].in != 0 )
            cout << teenToWord( thds[i].in ) << " ";
        if( thds[i].tn == 1 && thds[i].in == 0 )
            cout << "Ten" << " ";
        if( thds[i].hd || thds[i].tn || thds[i].in )
            cout << rangToWord( i ) << " ";

        if( value == 0 )
            cout << "Zero" << endl;
    }

    cout << endl;

    return 0;
}
