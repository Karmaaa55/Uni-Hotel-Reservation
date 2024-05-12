#ifndef DATE_H
#define DATE_H
#include <iostream>
#include <iomanip>

using namespace std;

class Date
{
private :
    short unsigned int year, month, day;
public:
    Date() {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    friend istream& operator>>(istream& in, Date& x)
    {
        char dash, dash2;
        in >> x.year >> dash >> x.month >> dash2 >> x.day;
        return in;
    }

    friend ostream& operator<<(ostream& out, const Date& x)
    {
        out << x.year << "-" << setfill('0') << setw(2) << right << x.month << "-" << setfill('0') << setw(2) << right << x.day << setfill(' ');
        return out;
    }

    Date operator +(int);
};

Date Date::operator+(int n)
    {
        Date x = *this;
        x.day += n;
        while (x.day > daysInMonth[x.month - 1])
        {
            x.day -= daysInMonth[x.month - 1];
            x.month++;
            if (x.month > 12)
            {
                x.month -= 12;
                x.year++;
            }
        }
        return x;
    }


#endif // DATE_H
