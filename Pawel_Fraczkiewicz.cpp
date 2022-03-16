#include <iostream>
#include <vector>
#include <fstream>
#include <string> //getline
#include <conio.h> //getch()
#include <sstream> 
#include <locale.h> // polskie znaki
#include <windows.h>  //gotoxy
#include <iomanip> //setw
#include <string> //getline

using namespace std;

const int BLACK = 0;
const int BLUE = 1;
const int GREEN = 2;
const int CYAN = 3;
const int RED = 4;
const int MAGENTA = 5;
const int BROWN = 6;
const int LIGHTGRAY = 7;
const int DARKGRAY = 8;
const int LIGHTBLUE = 9;
const int LIGHTGREEN = 10;
const int LIGHTCYAN = 11;
const int LIGHTRED = 12;
const int LIGHTMAGENTA = 13;
const int YELLOW = 14;
const int WHITE = 15;

int __BACKGROUND = BLACK;
int __FOREGROUND = LIGHTGRAY;

void textcolor(int color, int color2) //kolory
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color + (color2 << 4));
}

void gotoxy(int y, int x) // poruszanie po konsoli
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//zmienne stale globalne
const int roomCount = 3; // ilosc sal
const int roomSits = 100; // ilosc miejsc
int userID = 0; // początkowe ID
bool koniec = false; // do zakończenia programu
string password = "1234"; // hasło admina

string header = "KINO - System rezerwacji miejsc\n\n"; // nagłówek 

class User
{
public:
    int idUser;
    string email; // login
    string imie;
    string nazwisko;
};

class Moovie
{
public:
    int idMoovie;
    string title;
    string date;
    string time;
    int room;
    int sits[roomSits] = { 0 }; // tablica miejsc
};

class Booking
{
public:
    string email;
    int idMoovie;
    int sit; //???
};

//tablice dynamiczne globalne
vector<Moovie> Moovies; // tablica filmów
vector<User> Users; // tablica uzytkownikow

void pause()//pauza powyswietleniu menu
{
    cout << "nacisnij dowolny klawisz...";
    _getch();
}

//funkcja rozdziela linijkę tekstu/łączy elementy przez _  (info o filmie) 
string Rozdziel(string linia,int typ)//wskaźnik do adresu tablicy
{
    string tytul = "";
    for (int j = 0; j < linia.size(); j++)
    {
          if (typ == 1)
          {
              if (linia[j] == '_')
              {
                  tytul = tytul + " ";
              }
              else
              {
                  tytul = tytul + linia[j];
              }
          }
          else
          {
              if (linia[j] == ' ')
              {
                  tytul = tytul + '_';
              }
              else
              {
                  tytul = tytul + linia[j];
              }


          }
    }
    return tytul;
}

// odczyt plików
void readUsers()
{
    //odczyt użytkowników z pliku do tablicy
    User user;
    ifstream plik;
    plik.open("users.txt");//otwarcie pliku users
    if (plik) {//zabezpieczenie czy plik istnieje
        while (!plik.eof()) {//zczytanie do konca pliku
            try {//zabezpieczenie na wypadek bledu
                //przypisanie z pliku
                plik >> user.idUser;
                plik >> user.email;
                plik >> user.nazwisko;
                plik >> user.imie;
                Users.push_back(user);//dodanie do tablicy users
            }
            catch (std::runtime_error& e) {}
        }
        plik.close();
    }
    else
    {
        cout << "Blad pliku users" << endl;
    }

}

void readMoovies()
{
    //odczyt fimów z pliku do tablicy
    ifstream plik;
    string line;
    Moovie moovie;
    plik.open("moovies.txt");//otwarcie pliku moovies
    if (plik) {//zabezpieczenie czy plik istnieje
        while (!plik.eof()) {//zczytanie do konca pliku
            try {//zabezpieczenie na wypadek bledu
                
                //stoi - zamiana tekstu na liczbe - przypisanie Id
                plik >> moovie.idMoovie;
                plik >> moovie.title;
                plik >> moovie.date;
                plik >> moovie.time;
                plik >> moovie.room;
                moovie.title = Rozdziel(moovie.title,1);//usuniecie _ i wstawienie spacji
                Moovies.push_back(moovie);
            }
            catch (std::runtime_error& e) {

            }
        }
        plik.close();
    }
    else
        cout << "Blad pliku MOOVIES" << endl;

}

void readBooking()
{
    //odczyt rezerwacji z pliku do tablicy Moovies[id].sits[i]
    ifstream plik;
    Moovie moovie;
    plik.open("booking.txt");//otwarcie pliku booking
    if (plik) //zabezpieczenie czy plik istnieje
    {
        while (!plik.eof())//zczytanie do konca pliku
        {
            try {//zabezpieczenie na wypadek bledu
            int id;
            plik >> id;
            //wczytaj rezerwacje do tablicy miejsc na seansie
            for (int i = 0; i < roomSits; i++)
                plik >> Moovies[id - 1].sits[i];
            }
            catch (std::runtime_error& e) {}
        }
    }
    plik.close();
}

//zapis plików
void writeMoovies()
{
    //Zapis filmow do pliku
    ofstream plik;
    plik.open("moovies.txt");//otwarcie pliku moovies
    int i;
    for (i = 0; i < Moovies.size(); i++)
    {
        Moovies[i].title = Rozdziel(Moovies[i].title, 2);
        plik << Moovies[i].idMoovie << " "
            << Moovies[i].title << " "
            << Moovies[i].date << " "
            << Moovies[i].time << " "
            << Moovies[i].room; 
        if (i < Moovies.size() - 1) plik << endl;//przeniesienie na nastepna linie jesli nie jest ostatni tytul
    }
    plik.close();
}
void writeUsers()
{
    //zapis userów do pliku
    ofstream plik;
    plik.open("users.txt");//otwarcie pliku users
    int i;
    for (i = 0; i < Users.size(); i++) {
        plik << Users[i].idUser << " "
            << Users[i].email << " "
            << Users[i].nazwisko << " "
            << Users[i].imie;
        if (i < Users.size() - 1) plik << endl;//przeniesienie na nastepna linie jesli nie jest ostatni tytul
    }
    plik.close();
}
void writeBooking()
{
    ofstream plik;
    plik.open("booking.txt");//otwarcie pliku booking
    for (int i = 0; i < Moovies.size(); i++)
    {
        plik << Moovies[i].idMoovie << " ";//zapis id eansu
        for (int j = 0; j < roomSits; j++) 
            plik << Moovies[i].sits[j] << " ";//zapis rezerwacji
        if (i < Moovies.size() - 1) plik << endl;//przeniesienie na nastepna linie jesli nie jest ostatni tytul
    }

    plik.close();
}

//seanse
void showMoovies()
{
    textcolor(8, 0);//kolory
    //setw - rezerwacja ilosci miejsca na ekranie
    //left - wyrównaj do lewej
    cout << setw(5) << left << "ID"
        << setw(30) << left << "Tytuł"
        << setw(20) << left << "Data"
        << setw(10) << left << "Czas"
        << setw(10) << left << "Sala"
        << endl;
    cout << "---------------------------------------------------------------------\n";
    textcolor(9, 0);//kolory
    for (int i = 0; i < Moovies.size(); i++)//wyswietlenie filmów
    {
        Moovies[i].title = Rozdziel(Moovies[i].title, 1);
        cout << setw(5) << left
            << Moovies[i].idMoovie << setw(30) << left
            << Moovies[i].title << setw(20) << left
            << Moovies[i].date << setw(10) << left
            << Moovies[i].time << setw(10)
            << Moovies[i].room << endl;
    }
    textcolor(8, 0);
    cout << "---------------------------------------------------------------------\n";
    textcolor(9, 0);
}
void addMoovie()
{
    //dodanie nowego seansu w kinie
    string title;
    int room;
    string date;
    string time;
    Moovie moovie;
    char odp;
    showMoovies();
    while (true)
    {

        //przypisanie nowego id
        if (Moovies.size() > 0)
            moovie.idMoovie = Moovies.size() + 1;
        else
            moovie.idMoovie = 1;

        cout << endl << "ID:" << moovie.idMoovie << endl;
        cout << "Data:"; cin >> moovie.date;
        cout << "Tytuł:"; cin >> moovie.title;
        cout << "czas:"; cin >> moovie.time;
        cout << "Sala:"; cin >> moovie.room;
        Moovies.push_back(moovie);//dodanie do tablicy filmów
        cout << "dodać nasępny film?(t/n):"; cin >> odp;
        if (odp == 'n') break;
    }
    //zapis seansu do pliku
    writeMoovies();

}

int findMoovie(int id)// wyszukanie wskazanego id w tablicy  Moovies - zwraca pozycje id
{
    for (int i = 0; i < Moovies.size(); i++)
        if (Moovies[i].idMoovie == id)
            return i;
    return 0;
}

//użytkownicy
int findMail(string mail) // wyszukanie maila/loginu w tablicy Users - zwraca pozycje id
{
    for (int i = 0; i < Users.size(); i++)
        if (mail == Users[i].email)
            return Users[i].idUser;
    return 0;
}
int findUser()
{
    //sprawdza czy mail istnieje i zwraca idUsera
    string mail;
    cout << "Podaj email:"; cin >> mail;
    return findMail(mail);
}

int registerUser()//rejestracja uzytkownika
{
    User user;
    if (Users.size() > 0)//przypisanie id
        user.idUser = Users.size() + 1;
    else
        user.idUser = 1;
    string mail = "";
    do {
        cout << "Podaj email:"; cin >> mail;
        if (findMail(mail) > 0) {//sprawdzenie dostepnosci loginu
            cout << "email zajęty\n";
        }         
        else
            break;
    } while (true);
    if (mail == "0")
    {
        return 0;
    }
    user.email = mail;//przypisanie maila
    cout << "Nazwisko:"; cin >> user.nazwisko;
    cout << "Imie:"; cin >> user.imie;
    Users.push_back(user);//dodanie do tablicy
    userID = user.idUser;//zapamietanie id usera globalnie-automatyczne logowanie
    writeUsers();
    return userID;
}

void showUser(int userID)//wyswietlenie danych uzytkownika
{
    for (int i = 0; i < Users.size(); i++)
        if (Users[i].idUser == userID)
        {
            textcolor(RED, 0);
            cout << "Nazwisko i imię: ";
            textcolor(YELLOW, 0);
            cout << Users[i].nazwisko << " " << Users[i].imie << endl << endl;
            textcolor(WHITE, 0);
            break;
        }
}
void showUsers() //wyswietlenie danych uzytkownikow
{
    textcolor(8, 0);//kolory
//setw - rezerwacja ilosci miejsca na ekranie
//left - wyrównaj do lewej
    cout << setw(5) << left << "Id"
        << setw(20) << left << "Login/mail"
        << setw(20) << left << "Nazwisko"
        << setw(20) << left << "imie"
        << endl;
    cout << "---------------------------------------------------------------------\n";
    textcolor(9, 0);//kolory
    for (int i = 0; i < Users.size(); i++)
    {
        textcolor(9, 0);
        cout << setw(5)  << left << Users[i].idUser
             << setw(20) << left << Users[i].email 
             << setw(20) << left << Users[i].nazwisko 
             << setw(20) << left << Users[i].imie << endl;
        textcolor(15, 0);
    }
    textcolor(8, 0);
    cout << "---------------------------------------------------------------------\n";
    textcolor(9, 0);
}

//Rezerwacje
void showBooking(int userID)//wyswietla rezerwacje usera
{
    //dane usera
    showUser(userID);
    cout << endl << "REZERWACJE" << endl << endl;
    //wyszukiwanie zarezerwowanych miejsc
    textcolor(8, 0);
    cout << setw(20) << left
        << "DATA" << setw(10) << left
        << "GODZINA" << setw(10) << left
        << "SALA" << setw(30) << left
        << "TYTUŁ" << setw(10)
        << "MIEJSCE" << endl;
    cout << "------------------------------------------------------------------------------------------" << endl;
    textcolor(9, 0);

    Moovie moovie;
    for (int i = 0; i < Moovies.size(); i++)
    {
        moovie = Moovies[findMoovie(Moovies[i].idMoovie)];
        for (int j = 0; j < roomSits; j++)
            if (moovie.sits[j] == userID)
            {
                cout << setw(20) << left
                    << moovie.date << setw(10) << left
                    << moovie.time << setw(10) << left
                    << moovie.room << setw(30) << left
                    << moovie.title << setw(10) << left
                    << j+1 << endl;
            }
    }
    textcolor(8, 0);
    cout << "------------------------------------------------------------------------------------------" << endl;
    textcolor(9, 0);
}
void showRoom(int nr, bool admin)//wyswietlenie miejsc
{
    Moovie moovie;
    moovie = Moovies[findMoovie(nr)];
    textcolor(GREEN, 0);
    cout << " Sala nr: ";
    textcolor(YELLOW, 0);
    cout << moovie.room << "     ";
    textcolor(GREEN, 0);
    cout << " Seans: ";
    textcolor(YELLOW, 0);
    cout << moovie.title;
    cout << " Godzina: ";
   textcolor(GREEN, 0);
   cout << moovie.time << endl << endl;
    textcolor(9, 0);
    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << "rzad/miejsce" << endl;
    for (int i = 0; i < roomSits; i++)
    {
        if (i > 0 && i % 20 == 0) cout << endl;
        cout.width(5);
        if (!admin) //jeśli nie admin to wyświetl nr miejsca lub puste
        {
            if (moovie.sits[i] == 0)
                cout << right << i + 1;
            else
                cout << "";
        }
        else //jeśli admin to wyświetl idUsera lub 0 na miejscu
        {
            if (moovie.sits[i] == 0) textcolor(9, 0);
            else textcolor(RED, 0);
            cout << right << moovie.sits[i];

        }
    }
    textcolor(9, 0);
    cout << "\n----------------------------------------------------------------------------------------------------";
    cout << endl << endl;
}
bool checkBooking(int IDseans, int msc)// wsprawdzanie dostepnosci wskazanego miejsca
{
    if (Moovies[findMoovie(IDseans)].sits[msc - 1] == 0)
        return true;
    else
        return false;


}
void addBooking(int userID)//dodaj rezerwacje
{
    int nr, pos;
    showMoovies();
    do {
        cout << "Podaj nr seansu: "; 
        cin >> nr;
    } while (nr > Moovies.size()); //aż wprowadzi poprawny numer

    system("cls");//czyszczenie ekranu
    showRoom(nr, false);//wyswietlenie sali
    cout << endl << "Wybierz miejsce:"; cin >> pos;
    if (checkBooking(nr, pos))
    {
        Moovies[findMoovie(nr)].sits[pos-1] = userID;//wyszukanie wskazanego filmu po id
        writeBooking();
    }
    else
    {
        textcolor(RED, 0);
        cout << "Miejsce zajete\n";
        textcolor(9, 0);
        pause();
    }

}

//menu
int userBookingMenu(int userID)
{
    do {
    system("cls");
    cout << header;
    showUser(userID);
    textcolor(GREEN, 0);
    cout << "------------------------------\n";
    cout << "1 - Wyswietl rezerwacje\n";
    cout << "2 - Wyswietl seanse\n";
    cout << "3 - Dodaj rezerwacje\n";
    cout << "Esc - Powrót" << endl;
    cout << "------------------------------\n";
    textcolor(RED, 0);
    cout << "wybierz opcję:";
    textcolor(WHITE, 0);

        switch (_getch()) {
        case '1':
            system("cls");
            showBooking(userID);
            pause();
            break;
        case '2':
            system("cls");
            showMoovies();
            pause();
            break;
        case '3':
            system("cls");
            addBooking(userID);
            break;
        case 27:
            return 0;
        }
    } while (true);
}
int userMenu()
{
    do {
    system("cls");
    cout << header;
    textcolor(GREEN, 0);
    cout << "------------------------------\n";
    cout << "1 - Login" << endl;
    cout << "2 - Rejestracja" << endl;
    cout << "Esc - Powrót" << endl;
    cout << "------------------------------\n";
    textcolor(RED, 0);
    cout << "wybierz opcję:";
    textcolor(WHITE, 0);

        switch (_getch()) {
        case '1':
            system("cls");
            userID = findUser();
            if (userID > 0)
                userBookingMenu(userID);
            else {
                cout << "Blad logowania.\n\n";
                pause();
            }
            return 0;
        case '2':
            system("cls");
            if (registerUser() != 0)
                    userBookingMenu(userID);
            return 0;
        case 27:
            return 0;
        }
    } while (true);
}
int adminMenu()
{
    do {
    system("cls");
    cout << header;
    textcolor(GREEN, 0);
    cout << "------------------------------\n";
    cout << "1 - Wyswietl uzytkownikow\n";
    cout << "2 - Wyswietl seanse\n";
    cout << "3 - Wyswietl sale\n";
    cout << "5 - Dodaj seans\n";
    cout << "Esc - Powrót" << endl;
    cout << "------------------------------\n";
    textcolor(RED, 0);
    cout << "wybierz opcję:";
    textcolor(WHITE, 0);
  
        switch (_getch()) {
        case '1':
            system("cls");
            showUsers();
            pause();
            break;
        case '2':
            system("cls");
            showMoovies();
            pause();
            break;
        case '3':
            system("cls");
            int nr;
            showMoovies();
            cout << "Podaj id seansu:"; cin >> nr;
            system("cls");
            showRoom(nr, true);
            pause();
            break;
        case '4':
            addMoovie();
            break;
        case 27:
            return 0;
        }
    } while (true);

}
int mainMenu() // funkcja bez argumentów
{
    do {
    system("cls");
    string pass;

    textcolor(9, 0);
    cout << "                                         AKADEMIA GÓRNICZO-HUTNICZA \n";
    cout << "                                            Wydział  elektroniki    \n\n";
    cout << "                                          -- PAWEŁ FRACZKIEWICZ --  \n\n\n";

    textcolor(YELLOW, 0);
    cout << header;
    textcolor(GREEN, 0);
    cout << "------------------------------\n";
    cout << "1 - Rezerwacje" << endl;
    cout << "2 - Administracja" << endl;
    cout << "Esc - Zamknij system" << endl;
    cout << "------------------------------\n";
    textcolor(RED, 0);
    cout << "wybierz opcję:";
    textcolor(WHITE, 0);

        switch (_getch()) {
        case  '1':
            userMenu();
            break;
        case  '2':
            cout << endl << "haslo: "; cin >> pass;
            if (pass == password)
                adminMenu();
            else
            {
                cout << "zle haslo" << endl;
                pause();
            }
            break;
        case  27:
            koniec = true;
            return 0;
        }
    } while (true);
}

int main()
{
    setlocale(LC_CTYPE, "Polish"); // czcionka polska
    //odczyt danych z plików
    readMoovies();
    readBooking();
    readUsers();
    //showRoom(1, true);
    do {
        system("cls");
        mainMenu();
    } while (!koniec);

}