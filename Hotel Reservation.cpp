#include <iostream>
#include <iomanip>
#include <string>
#include "Date.h"

using namespace std;

enum struct Board {breakfast, half_board, all_inclusive, no_meals};

ostream& operator<<(ostream& out, Board& meal)
{
    switch (meal)
    {
    case Board::breakfast:
        out << "breakfast";
        break;
    case Board::half_board:
        out << "half board";
        break;
    case Board::all_inclusive:
        out << "all inclusive";
        break;
    default :
        out << "no meal";
    }
}

float get_price(Board meal)
{
    switch (meal)
    {
    case Board::breakfast:
        return 10;
        break;
    case Board::half_board:
        return 20;
        break;
    case Board::all_inclusive:
        return 50;
        break;
    default:
        return 0;
    }
}

class Accomodation
{
private:
    string location;
    Date arrival;
    int nights, singles, doubles;
    float priceNightSingle, priceNightDouble, priceOneDayParking;
    Board board;
    bool parking;
public:
    Accomodation (float priceSingle, float priceDouble, float priceParking): priceNightSingle(priceSingle),
        priceNightDouble(priceDouble), priceOneDayParking(priceParking)
    {
        ask_data();
    }
    Date get_arrival()
    {
        return arrival;
    }
    Date get_checkout()
    {
        return arrival+nights;
    }
    ~Accomodation()
    {
        cout << "destructor Accomodation " << location << " at " << arrival << " for " << get_checkout() << " guests done" << endl;
    }
    string get_location()
    {
        return location;
    }
    int get_guests()
    {
        return (2*doubles)+singles;
    }
    float get_price()
    {
        if (parking == false)
        {
            return nights*(singles*priceNightSingle + doubles*priceNightDouble + ::get_price(board)*get_guests());
        }
        else
        {
            return nights*(singles*priceNightSingle + doubles*priceNightDouble + ::get_price(board)*get_guests() + priceOneDayParking);
        }
    }
    void ask_data()
    {
        char temp, meal; //for the parking boolean, meal switch case
        cout << "location of accomodation: ";
        cin >> location;
        cout << "arrival on (yyyy-mm-dd): ";
        cin >> arrival;
        cout << "how many nights (maximum 4 weeks): ";
        cin >> nights;
        cout << "how many single bed rooms (" << priceNightSingle<< " EUR/night): ";
        cin >> singles;
        cout << "how many double bed rooms (" << priceNightDouble << " EUR/night): ";
        cin >> doubles;
        cout << "a all inclusive (50.00 EUR/day)" << endl;
        cout << "b breakfast     (10.00 EUR/day)" << endl;
        cout << "h half board    (20.00 EUR/day)" << endl;
        cout << "n no meals" << endl << "your choice: ";
        cin >> meal;
        switch (meal)
        {
        case 'a':
            board = Board::all_inclusive;
            break;
        case 'b':
            board = Board::breakfast;
            break;
        case 'h':
            board = Board::half_board;
            break;
        default :
            board = Board::no_meals;
        }
        cout << "with parking place (10.00 EUR/day; y(es) or n(o)): ";
        cin >> temp;
        if (temp == 'n')
        {
            parking = false;
        }
        else
        {
            parking = true;
        }
        cout << "price: " << get_price() << " EUR" << endl;
    }
    void print()
    {
        int width = 30;
        cout << setw(width) << left << "accomodation at:" << location << endl;
        cout << setw(width) << left << "number of guests:" << get_guests() << endl;
        cout << setw(width) << left << "number of nights:" << nights << endl;
        cout << setw(width) << left << "check-in date:" << get_arrival() << endl;
        cout << setw(width) << left << "check-out date:" << get_checkout() << endl;
        cout << setw(width) << left << "single bed room(s): " << singles << " (80.00 EUR/night)" << endl;
        cout << setw(width) << left << "double bed room(s): " << doubles << " (150.00 EUR/night)" << endl;
        cout << setw(width) << left << "board:" << board << '(' << ::get_price(board) << " EUR/day and person)" << endl;
        cout << setw(width) << left << "parking:";
        if (parking == true)
        {
            cout << "included (10.00 EUR/day)" << endl;
        }
        else
        {
            cout << "no parking place booked" << endl;
        }
        cout << setw(width) << left << "price accomodation:" << get_price() << "EUR" << endl;
    }
};

class Transport
{
public :
    virtual ~Transport()
    {
        cout << "destructor Transport done" << endl;
    }
    virtual bool includedTransfer()
    {
        return false;
    }
    virtual float get_price() = 0;
    virtual void print() = 0;
};

class SelfTravel : public Transport
{
public:
    SelfTravel() {}
    ~SelfTravel()
    {
        cout << "destructor SelfTravel done" << endl;
    }
    virtual float get_price()
    {
        return 0.00;
    }
    virtual void print()
    {
        cout << "self travel no transfer" << endl;
    }
};

class PublicTransport : public Transport
{
protected :
    Date departure;
    string code, from, to;
    float priceOneSeat;
    bool firstClass;
public :
    PublicTransport(Date departure, string code, string from, string to, double priceOneSeat, bool firstClass = false):
        departure(departure), code(code), from(from), to(to), priceOneSeat(priceOneSeat), firstClass(firstClass) {}
    virtual ~PublicTransport()
    {
        cout << "destructor PublicTransport " << code << " at " << departure <<  " from " << from << " to " << to << " done" << endl;
    }
    virtual void print()
    {
        cout << departure << " " << code << " from " << from << " to " << to;
        if (firstClass == true)
            cout << " first class";
        cout << " (" << get_price() << " EUR/person) transfer included" << endl;
    }
};

class Flight : public PublicTransport
{
protected :
    bool transfer;
public :
    Flight(Date departure, string code, string from, string to, double priceOneSeat, bool firstClass = false, bool transfer = true) :
        PublicTransport(departure, code, from, to, priceOneSeat, firstClass) {}
    virtual ~Flight()
    {
        cout << "destructor Flight done" << endl;
    }
    virtual bool includedTransfer()
    {
        return transfer;
    }
    virtual float get_price()
    {
        if (firstClass == true)
            return 2*priceOneSeat;
        else return priceOneSeat;
    }
    virtual void print()
    {
        cout << "flight ";
        PublicTransport::print();
    }
};

class Train : public PublicTransport
{
public :
    Train(Date departure, string code, string from, string to, double priceOneSeat, bool firstClass = false):
        PublicTransport(departure, code, from, to, priceOneSeat, firstClass = false) {}
    virtual ~Train()
    {
        cout << "destructor Train done" << endl;
    }
    virtual float get_price()
    {
        if (firstClass == true)
            return 1.5*priceOneSeat;
        else return priceOneSeat;
    }
    virtual void print()
    {
        cout << "train ";
        PublicTransport::print();
    }
};

class Request
{
private:
    Transport *transportOutward, *transportReturn;
    const unsigned int no;
    static unsigned int lastNo;
    Accomodation* accomodation;
    Request* next;
public:
    Request(Accomodation* acc, Request* nextReq = nullptr, Transport* transportOut = nullptr, Transport* transportRet = nullptr) :
        no(++lastNo), accomodation(acc), next(nextReq), transportOutward(transportOut), transportReturn(transportRet) {}
    ~Request()
    {
        delete accomodation;
        delete transportOutward;
        delete transportReturn;
        cout << "destructor Request " << no << " done" << endl;
    }
    int get_no()
    {
        return no;
    }
    Request* get_next()
    {
        return next;
    }
    void set_next(Request* newReq)
    {
        next = newReq;
    }
    float get_price()
    {
        return accomodation->get_price()+(transportOutward->get_price()+transportReturn->get_price())*accomodation->get_guests();
    }
    void print()
    {
        int width = 30;
        cout << "********REQUEST NO: " << no << "********" << endl;
        accomodation->print();
        cout << setw(width) << left << "outward journey: ";
        transportOutward->print();
        cout << setw(width) << left <<  "journey back: ";
        transportReturn->print();
        cout << setw(width) << left << "price total: " << get_price() << " EUR" << endl;
    }
};

unsigned int Request::lastNo = 0;

class CallCenter
{
private:
    Request* head;
public:
    CallCenter():head(nullptr) {}
    append(Request* req)
    {
        if (head == nullptr)
        {
            req->set_next(nullptr);
            head = req;
        }
        else
        {
            Request* temp = head;
            while (temp->get_next() != nullptr)
            {
                temp = temp->get_next();
            }
            temp->set_next(req);
        }
    }
    void cancel(Request* req)
    {
        Request* temp = head;
        if (head == req)
        {
            head = req->get_next();
            delete req;
        }
        else if (temp != req)
        {
            while (temp->get_next() != req && req->get_next() != nullptr)
            {
                temp = temp->get_next();
            }
            temp->set_next(req->get_next());
            delete req;
        }
    }
    Request* get_request(int reqNo)
    {
        Request* temp = head;
        while (temp->get_no() != reqNo && temp != nullptr)
        {
            temp = temp->get_next();
        }
        if (temp->get_no() == reqNo)
        {
            return temp;
        }
        else
        {
            return nullptr;
        }
    }
    void print_all()
    {
        Request* temp = head;
        while (temp != nullptr)
        {
            temp->print();
            temp=temp->get_next();
        }
    }
};

Transport* transportType(Date date)
{
    Transport* type;
    char choice;
    cout << "0 self travel" << endl;
    cout << "1 by flight" << endl;
    cout << "2 by train" << endl;
    cout << "your choice: ";
    cin >> choice;
    switch(choice)
    {
    case '0':
        type = new SelfTravel();
        return type;
    case '1':
    {
        char choice2;
        string code, departure, arrival;
        float price;
        bool firstClass;
        cout << "code of flight: ";
        cin >> code;
        cout << "airport of departure: ";
        cin >> departure;
        cout << "airport of arrival: ";
        cin >> arrival;
        cout << "first class ((y)es or (n)o): ";
        cin >> choice2;
        if (choice2 == 'y')
            firstClass = true;
        else firstClass = false;
        cout << "standard price for one passenger: ";
        cin >> price;
        type = new Flight(date, code, departure, arrival, price, firstClass);
        return type;
    }
    case '2':
        char choice2;
        string code, departure, arrival;
        float price;
        bool firstClass;
        cout << "code of train: ";
        cin >> code;
        cout << "main train station of departure: ";
        cin >> departure;
        cout << "main train station of arrival: ";
        cin >> arrival;
        cout << "first class ((y)es or (n)o): ";
        cin >> choice2;
        if (choice2 == 'y')
            firstClass = true;
        else firstClass = false;
        cout << "standard price for one passenger: ";
        cin >> price;
        type = new Train(date, code, departure, arrival, price, firstClass);
        return type;
    }
}

int main()
{
    CallCenter callCenter;
    Transport* transportOut, *transportRet;
    Request* nextReq;
    char choice;
    do
    {
        cout << fixed << setprecision(2);
        cout << "CALL CENTER BOOKING REQUEST" << endl;
        cout << "0 end" << endl;
        cout << "1 new reservation request standard" << endl;
        cout << "2 new reservation request superior" << endl;
        cout << "3 show reservation request" << endl;
        cout << "4 show all reservation requests" << endl << "your choice: ";
        cin >> choice;
        switch(choice)
        {
        case '0':
            return 0;
        case '1':
        {
            Accomodation* acc = new Accomodation(80.00, 150.00, 10.00);
            cout << "please choose transport outward" << endl;
            transportOut = transportType(acc->get_arrival());
            cout << "please choose transport return" << endl;
            transportRet = transportType(acc->get_checkout());
            Request* req = new Request(acc, nextReq = nullptr, transportOut, transportRet);
            callCenter.append(req);
            break;
        }
        case '2':
        {
            Accomodation* acc = new Accomodation(110.00, 210.00, 10.00);
            cout << "please choose transport outward" << endl;
            transportOut = transportType(acc->get_arrival());
            cout << "please choose transport return" << endl;
            transportRet = transportType(acc->get_checkout());
            Request* req = new Request(acc, nextReq = nullptr, transportOut, transportRet);
            callCenter.append(req);
            break;
        }
        case '3':
            int no;
            char choice2; //for optional deleting of request
            cout << "number of reservation request: ";
            cin >> no;
            if (callCenter.get_request(no) == nullptr)
            {
                cout << "reservation request not found";
            }
            else
            {
                callCenter.get_request(no)->print();
                cout << "(c)ancel this request or (n)ot: ";
                cin >> choice2;
                if (choice2 == 'c')
                {
                    callCenter.cancel(callCenter.get_request(no));
                }
                break;
            }
        case '4':
            callCenter.print_all();
            break;
        default :
            cout << "enter an appropriate number!" << endl << endl;
        }
    }
    while (choice != '0');
    return 0;
}
