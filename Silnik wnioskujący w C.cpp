#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct DANE {
    char* nazwa;
    int ilosc;
    int** wartosci;          // wartosc[x][0] = ilu_argumentowy, wartosc[x][1] = jaka_wartosc_logiczna (2 - szukana, 3 - porazka)
} dana;

typedef struct WZORY {
        int ile_w;
        int* wynik;                // wynik[0]  = czy przed stoi ! czy nie;
        int ile_r;
        int* regula;
} wzor;

typedef struct POZIOM {
        int ile_r;
        int ile_w;
        int wartosc;
        int poziom;
        int* regula;
        int* wynik;
} poziom;

char** argumenty;
dana* rekordy;
wzor* reguly;
int ile_danych;
int ile_wzorow;

int liczba_poziomow=0;
int ostatni_poziom=0;
poziom* poziomy;

char* wczytaj_plik ( char* nazwa_pliku )
{
        FILE* plik = fopen ( nazwa_pliku, "r" );
    if (plik == NULL) perror("Nie udalo sie otworzyc pliku");

    int rozmiar_bufora = 32;
    char* wczytane_znaki = (void*)malloc ( sizeof(char) * rozmiar_bufora ) ;
    int liczba_wczytanych_znakow;
    int przedzial = 32;
    int i = 0;    // liczba ogolnie wczytanych znakow
    while(1)
    {
        liczba_wczytanych_znakow = fread( &wczytane_znaki[i], sizeof (char) , przedzial, plik );
        if(liczba_wczytanych_znakow == przedzial)
        {
            i = rozmiar_bufora-1;
            rozmiar_bufora *= 2;
            przedzial *= 2;
            wczytane_znaki = (void*)realloc ( wczytane_znaki, rozmiar_bufora * sizeof(char) );
        }
        else break;
    }
    fclose(plik);
    return wczytane_znaki;
}

int wczytaj_dane ( char* szukana, char* dane)
{
        int ktory_argument = 0;
        int ile_danych = 0;

        //zmienne dotyczace petli
        int i=-1;
        int poczatek_nazwy;
        int ile_argumentow;
        int w_nawiasie = 0;
        int zakonczone = 1;
        int dlugosc_nazwy;
        char* tymczasowa;
        int n;
        int istnieje = 0;
        int do_ktorej;    // do ktorej danych tworzymy wartosci
        int ilosc;
        int warunek;
        int jest;

        do
        {
                i++;
                warunek = ( szukana[i] == ',' || szukana[i] == '(' || szukana[i] == ' ' || szukana[i] == ')' || szukana[i] == '\n' || szukana[i] == '\0');
                if( zakonczone == 0 && warunek)
                {
                        dlugosc_nazwy == i-poczatek_nazwy + 1;
                        tymczasowa = (void*)realloc (tymczasowa ,sizeof(char) * dlugosc_nazwy );
                        memcpy( tymczasowa, &szukana[poczatek_nazwy], dlugosc_nazwy-1);
                        tymczasowa[dlugosc_nazwy-1] = '\0';
                        if( w_nawiasie == 1 )
                        {
                                for(n=0; n< ktory_argument; n++)
                                {
                                        jest = 0;
                                        if( strcmp( tymczasowa, argumenty[n]) == 0 )
                                        {
                                                jest=1;
                                                break;
                                        }
                                }
                                if(jest == 0)
                                {
                                        argumenty = (void*)realloc (argumenty, (ktory_argument+1)*sizeof(char*));
                                        argumenty[ktory_argument] = (void*)malloc (sizeof(char) * dlugosc_nazwy );
                                        strcpy ( argumenty[ktory_argument], tymczasowa);
                                        ilosc = rekordy[do_ktorej].ilosc;
                                        rekordy[do_ktorej].wartosci[ilosc-1][0] ++ ;
                                        rekordy[do_ktorej].wartosci[ilosc-1][ rekordy[do_ktorej].wartosci[ilosc-1][0]+1 ] = ktory_argument;
                                        ktory_argument;
                                }
                                if(jest == 1)
                                {
                                        ilosc = rekordy[do_ktorej].ilosc;
                                        rekordy[do_ktorej].wartosci[ilosc-1][0] ++ ;
                                        rekordy[do_ktorej].wartosci[ilosc-1] = (void*)realloc (rekordy[do_ktorej].wartosci[ilosc-1],sizeof(int) * (rekordy[do_ktorej].wartosci[ilosc-1][0]+2) );
                                        rekordy[do_ktorej].wartosci[ilosc-1][ rekordy[do_ktorej].wartosci[ilosc-1][0]+1 ] = n;
                                }
                        }
                        else
                        {
                                for(n=0; n< ile_danych; n++)
                                {
                                        jest = 0;
                                        if( strcmp( tymczasowa, argumenty[n]) == 0 )
                                        {
                                                jest=1;
                                                break;
                                        }
                                }
                                if(jest == 0)
                                {
                                        do_ktorej = ile_danych;
                                        ile_danych += 1;
                                        rekordy = (void*)realloc( rekordy, sizeof( dana*) * ile_danych );
                                        rekordy[do_ktorej].nazwa = (void*)malloc (sizeof(char) * dlugosc_nazwy);
                                        strcpy ( rekordy[do_ktorej].nazwa , tymczasowa);
                                        rekordy[do_ktorej].ilosc = 1;
                                }
                                if(jest == 1)
                                {
                                        do_ktorej = n;
                                        rekordy[do_ktorej].ilosc ++;
                                }
                                ilosc = rekordy[do_ktorej].ilosc;
                                rekordy[ do_ktorej].wartosci = (void*)realloc( rekordy[ do_ktorej].wartosci,sizeof(int*) * ilosc);
                                rekordy[ do_ktorej].wartosci[ilosc-1] = (void*)realloc (rekordy[ do_ktorej].wartosci[ilosc-1] ,sizeof(int) * 2);
                                rekordy[ do_ktorej].wartosci[ilosc-1][0] = 0;
                                rekordy[ do_ktorej].wartosci[ilosc-1][1] = 2;
                        }
                }
                else if( zakonczone == 1 && !warunek)
                {
                        zakonczone = 0;
                        poczatek_nazwy = i;
                }
                if( szukana[i] == '(' )
                        w_nawiasie++;
                if( szukana[i] == ')')
                        w_nawiasie--;
        }
        while (szukana[i] != '\0');

        i = -1;

        do
        {
                i++;
                warunek = ( dane[i] == ',' || dane[i] == '(' || dane[i] == ' ' || dane[i] == ')' || dane[i] == '\n' || dane[i] == '\0' || dane[i] == '=' || dane[i] == '>');
                if( zakonczone == 0 && warunek)
                {
                        dlugosc_nazwy == i-poczatek_nazwy + 1;
                        tymczasowa = (void*)realloc (tymczasowa ,sizeof(char) * dlugosc_nazwy );
                        memcpy( tymczasowa, &dane[poczatek_nazwy], dlugosc_nazwy-1);
                        tymczasowa[dlugosc_nazwy-1] = '\0';
                        if( w_nawiasie == 1 )
                        {
                                for(n=0; n< ktory_argument; n++)
                                {
                                        jest = 0;
                                        if( strcmp( tymczasowa, argumenty[n]) == 0 )
                                        {
                                                jest=1;
                                                break;
                                        }
                                }
                                if(jest == 0)
                                {
                                        argumenty = (void*)realloc (argumenty ,(ktory_argument+1)*sizeof(char*));
                                        argumenty[ktory_argument] = (void*)malloc (sizeof(char) * dlugosc_nazwy );
                                        strcpy ( argumenty[ktory_argument], tymczasowa);
                                        ilosc = rekordy[do_ktorej].ilosc;
                                        rekordy[do_ktorej].wartosci[ilosc-1][0] ++ ;
                                        rekordy[do_ktorej].wartosci[ilosc-1][ rekordy[do_ktorej].wartosci[ilosc-1][0]+1 ] = ktory_argument;
                                        ktory_argument;
                                }
                                if(jest == 1)
                                {
                                        ilosc = rekordy[do_ktorej].ilosc;
                                        rekordy[do_ktorej].wartosci[ilosc-1][0] ++ ;
                                        rekordy[do_ktorej].wartosci[ilosc-1] = (void*)realloc (  rekordy[do_ktorej].wartosci[ilosc-1] , sizeof(int) * (rekordy[do_ktorej].wartosci[ilosc-1][0]+2) );
                                        rekordy[do_ktorej].wartosci[ilosc-1][ rekordy[do_ktorej].wartosci[ilosc-1][0]+1 ] = n;
                                }
                        }
                        else
                        {
                                for(n=0; n< ile_danych; n++)
                                {
                                        jest = 0;
                                        if( strcmp( tymczasowa, argumenty[n]) == 0 )
                                        {
                                                jest=1;
                                                break;
                                        }
                                }
                                if(jest == 0)
                                {
                                        do_ktorej = ile_danych;
                                        ile_danych += 1;
                                        rekordy = (void*)realloc( rekordy, sizeof( dana*) * ile_danych );
                                        rekordy[do_ktorej].nazwa = (void*)malloc (sizeof(char) * dlugosc_nazwy);
                                        strcpy ( rekordy[do_ktorej].nazwa , tymczasowa);
                                        rekordy[do_ktorej].ilosc = 1;
                                }
                                if(jest == 1)
                                {
                                        do_ktorej = n;
                                        rekordy[do_ktorej].ilosc ++;
                                }
                                ilosc = rekordy[do_ktorej].ilosc;
                                rekordy[ do_ktorej].wartosci = (void*)realloc( rekordy[ do_ktorej].wartosci, sizeof(int*) * ilosc);
                                rekordy[ do_ktorej].wartosci[ilosc-1] = (void*)realloc (rekordy[ do_ktorej].wartosci[ilosc-1] ,sizeof(int) * 2);
                                rekordy[ do_ktorej].wartosci[ilosc-1][0] = 0;
                        }
                }
                else if( zakonczone == 1 && !warunek)
                {
                        zakonczone = 0;
                        poczatek_nazwy = i;
                }
                if( dane[i] == '(' )
                        w_nawiasie++;
                if( dane[i] == ')')
                        w_nawiasie--;
                if( dane[i] == '>')
                {
                        while( dane[i] != 'T' && dane[i] != 'F')
                                i++;
                        if(dane[i] == 'T')
                                rekordy[ do_ktorej].wartosci[ilosc-1][1] = 1;
                        else
                                rekordy[ do_ktorej].wartosci[ilosc-1][1] = 0;
                }
        }
        while( dane[i] != '\0' );

        free(tymczasowa);
}

void wczytaj_wzory ( char* wzory, wzor* reguly)
{
        ile_wzorow = 0;

        // sterowanie petla
        int i;
        int znak_wzoru = -1;
        int w_nawiasie = 0;
        int zakonczone = 1;            // 1 - nie rozpoczeto wczytywania nazwy, 0 - przechodzimy przez c-string jakiejs nazwy
        int co_robimy;           // 0 - regule, 1 - wynik
        int poczatek_nazwy;
        int warunek;
        int dlugosc_nazwy;
        int buf=0;

        // sterowanie zmiennymi we wzorze    $
        char** zmienne;
        int ile_zmiennych=0;
        int n;

        for( i=-1; wzory[i] != '\0'; )
        {
                i++;
                if( (znak_wzoru+1) == buf)
                {
                        if(co_robimy == 0)
                        {
                                buf+=4;
                                reguly[ile_wzorow].regula = (void*)realloc (reguly[ile_wzorow].regula, sizeof(int)*buf);
                        }
                        if(co_robimy == 1)
                        {
                                buf+=3;
                                reguly[ile_wzorow].wynik = (void*)realloc (reguly[ile_wzorow].wynik, sizeof(int)*(buf+2));
                                if(buf == 3)
                                {
                                        znak_wzoru++;
                                        reguly[ile_wzorow].wynik[0] = 0;
                                }
                        }
                }
                warunek = (wzory[i] == '(' || wzory[i] == ')' || wzory[i] == '|' || wzory[i] == '&' || wzory[i] == '!' || wzory[i] == '=' || wzory[i] == ' ' || wzory[i] == '\n' || wzory[i] == '\0');
                if( !warunek && zakonczone )
                {
                        poczatek_nazwy = i;
                        continue;
                }
                if( warunek && !zakonczone)
                {
                        dlugosc_nazwy = i-poczatek_nazwy;
                        if( w_nawiasie == 1 )
                        {
                                for(n=0; n<ile_zmiennych; n++)
                                {
                                        if( dlugosc_nazwy == strlen(zmienne[n]) && !strncmp(zmienne[n], &wzory[poczatek_nazwy], dlugosc_nazwy) )
                                        {
                                                if(co_robimy == 0)
                                                        reguly[ile_wzorow].regula[++znak_wzoru] = n;
                                                else
                                                        reguly[ile_wzorow].wynik[++znak_wzoru] = n;
                                                break;
                                        }
                                }
                                if( n == ile_zmiennych)
                                {
                                        zmienne = (void*)realloc ( zmienne, (ile_zmiennych+1)*sizeof(char*));
                                        zmienne[ile_zmiennych] = (void*)malloc ( sizeof(char)* (dlugosc_nazwy+1) );
                                        memcpy(zmienne[ile_zmiennych], &wzory[poczatek_nazwy], dlugosc_nazwy);
                                        zmienne[ile_zmiennych][dlugosc_nazwy] = '\0';
                                        if( co_robimy == 0 )
                                                reguly[ile_wzorow].regula[++znak_wzoru] = ile_zmiennych;
                                        else
                                                reguly[ile_wzorow].wynik[++znak_wzoru] = ile_zmiennych;
                                        ile_zmiennych++;
                                }
                        }
                        else
                        {
                                for(n=0; n<ile_danych; n++)
                                {
                                        if( dlugosc_nazwy == strlen(rekordy[n].nazwa) && !strncmp(rekordy[n].nazwa, &wzory[poczatek_nazwy], dlugosc_nazwy) )
                                        {
                                                if( co_robimy == 0 )
                                                        reguly[ile_wzorow].regula[++znak_wzoru] = n;
                                                else
                                                        reguly[ile_wzorow].wynik[++znak_wzoru] = n;
                                                break;
                                        }
                                }
                                if( n == ile_danych)
                                {
                                        rekordy = (void*)realloc ( rekordy, sizeof(dana*) *(ile_danych+1) );
                                        rekordy[ile_danych].nazwa = (void*)malloc ( sizeof(char) * (dlugosc_nazwy+1));
                                        memcpy ( rekordy[ile_danych].nazwa, &wzory[poczatek_nazwy], dlugosc_nazwy);
                                        rekordy[ile_danych].nazwa[dlugosc_nazwy] = '\0';
                                        rekordy[ile_danych].ilosc = 0;
                                        if( co_robimy == 0 )
                                                reguly[ile_wzorow].regula[++znak_wzoru] = ile_danych;
                                        else
                                                reguly[ile_wzorow].wynik[++znak_wzoru] = ile_danych;
                                        ile_danych++;
                                }
                        }
                }
                if( wzory[i] == '=' )
                {
                        co_robimy = 1;
                        reguly[ile_wzorow].ile_r = znak_wzoru+1;
                        znak_wzoru = -1;
                        buf = 0;
                        continue;
                }
                if( wzory[i] == '(' || wzory[i] == ')')
                {
                        if(reguly[ile_wzorow].regula[znak_wzoru-1] < 0 || znak_wzoru == 0)
                        {
                                if(wzory[i] == '(')
                                        reguly[ile_wzorow].regula[++znak_wzoru] = -4;
                                else if (wzory[i] == ')')
                                        reguly[ile_wzorow].regula[++znak_wzoru] = -5;
                        }
                        else
                                w_nawiasie != w_nawiasie;
                        continue;
                }
                if(wzory[i] == '!')
                {
                        if(co_robimy == 0)
                                reguly[ile_wzorow].regula[++znak_wzoru] = -1;
                        else
                                reguly[ile_wzorow].wynik[0] = -1;
                        continue;
                }
                if(wzory[i] == '|')
                {
                        i++;
                        reguly[ile_wzorow].regula[++znak_wzoru] = -2;
                        continue;
                }
                if(wzory[i] == '&')
                {
                        i++;
                        reguly[ile_wzorow].regula[++znak_wzoru] = -3;
                        continue;
                }
                if( (wzory[i] == ',' || wzory[i] == '\n') && co_robimy)
                {
                        reguly[ile_wzorow].ile_w = znak_wzoru+1;
                        ile_wzorow++;
                        co_robimy = 0;
                        znak_wzoru = -1;
                        buf = 0;
                }
        }
        ile_wzorow++;
}

int lub(int a, int b)
{
        return (a||b);
}

int i(int a, int b)
{
        return (a&&b);
}

int obliczanie_wzoru (int* wzor, int dlugosc)
{
        int znak = 0;             // na ktorym znaku wzoru jestesmy
        int tablica[3] = {2,2,2};             // wartosci na ktorych beda za chwile odbywac sie obliczenia, 2 - oznacza puste miejsce
        int buf;             // wartosc gotowa do przypisania do jednego z miejsc tablicy
        int ile_nawiasow=0;    // ilosc nawiasow zamykajacych do pominiecia
        int stan = 0;        //odpowiada za kolejnosc wykonywania dzialan,
                                                 // 0 - poczatek(nic),
                                                 // 1 - czekanie na wczytanie drugiego argumentu do &&,
                                                 // 2 - pojawienie sie || i czekanie do wczytania kolejnej operacji,
                                                 // 3 - pojawienie sie drugi raz z rzêdu ||,
                                                 // 4 - pojawienie sie && po ||,
        int negacja = 0;
        while(1)
        {
                if(wzor[znak] == -4)
                {
                        buf = obliczanie_wzoru (&wzor[znak+1], dlugosc);
                        if(buf==2)
                                return 2;
                        if(negacja==1)
                        {
                                negacja = 0;
                                buf=!buf;
                        }
                        if(tablica[0]==2) { tablica[0] = buf; }
                        else if(tablica[1]==2) {tablica[1] = buf; }
                        else {tablica[2] = buf;}
                        ile_nawiasow = 1;
                        while(ile_nawiasow!=0)
                        {
                                znak++;
                                if(wzor[znak]==-4)
                                {
                                        ile_nawiasow++;
                                }
                                if(wzor[znak]==-5)
                                {
                                        ile_nawiasow--;
                                }
                        }
                        znak++;
                }
                if(wzor[znak] >= 0)
                {
                        buf = wzor[znak];
                        if(buf==2)
                                return 2;
                        if(negacja==1)
                        {
                                negacja=0;
                                buf=!buf;
                        }
                        if(tablica[0]==2) { tablica[0] = buf; }
                        else if(tablica[1]==2) {tablica[1] = buf; }
                        else {tablica[2] = buf;}
                        znak++;
                }
                if(wzor[znak] == -3)
                {
                        znak+=2;
                        if(stan == 2)
                        {
                                stan = 4;
                        }
                        else stan = 1;
                }
                if(wzor[znak] == -1)
                {
                        negacja=!negacja;
                        znak++;
                }
                if(wzor[znak] == -2)
                {
                        znak+=2;
                        if(stan == 2)
                        {
                                stan = 3;
                        }
                        else stan = 2;
                }
                if(wzor[znak] == -5 || dlugosc==(znak+1))        //koniec dzialania funkcji przy napodkaniu ) i =
                {
                        if(stan==1) { return i(tablica[0], tablica[1]); }
                        if(stan==2) { return lub(tablica[0], tablica[1]); }
                        return tablica[0];
                }
                //przetwarzanie stanow
                if(stan == 1)
                {
                        if( tablica[0] != 2 && tablica[1] != 2)
                        {
                                tablica[0] = i(tablica[0], tablica[1]);
                                tablica[1] = 2;
                                stan = 0;
                        }
                }
                if(stan == 3)
                {
                        tablica[0] = lub(tablica[0], tablica[1]);
                        tablica[1] = 2;
                        stan = 2;
                }
                if(stan == 4)
                {
                        tablica[1] = i(tablica[1], tablica[2]);
                        tablica[2] = 2;
                        stan = 2;
                }
        }
}

void wyswietl( int* tekst, int dlugosc)
{
        int i;
        int stan;
        for(i=0; i<dlugosc; i++)
        {
                if(tekst[i] >=0)
                {
                        if(stan == 0)
                        {
                                 printf("%s",rekordy[tekst[i]].nazwa);
                                 stan = 1;
                                 continue;
                        }
                        if(stan == 1)
                        {
                                printf("%s", argumenty[tekst[i]]);
                                continue;
                        }
                }
                stan = 0;
                switch(tekst[i])
                {
                        case -1: printf("!");
                                break;
                        case -2: printf("||");
                                break;
                        case -3: printf("&&");
                                break;
                        case -4: printf("(");
                                break;
                        case -5: printf(")");
                                break;
                }
        }
}

int znajdz_szukana(int *k, int *m)
{
        int n;
        int j;
        for(n=0; n<ile_danych; n++)
        {
                for(j=0; j<rekordy[n].ilosc; j++)
                {
                        if( rekordy[n].wartosci[j][1] == 2)
                        {
                                *k = n;
                                *m = j;
                                return 1;
                        }
                }
        }
        *k = -1;
        return -1;
}

int szukanie_wartosci (int * wskaznik, int ile_arg, int poziom)
{
        int ilosc = rekordy[*wskaznik].ilosc;
        int n;
        int zwrotna;
        for( n = 0; n < ilosc; n++)
        {
                if( rekordy[*wskaznik].wartosci[n][0] == ile_arg )
                {
                        if( memcmp ( &wskaznik[1], &rekordy[*wskaznik].wartosci[n][2], sizeof(int)*ile_arg) == 0 )
                        {
                                if( rekordy[*wskaznik].wartosci[n][1] < 2 )
                                {
                                        return rekordy[*wskaznik].wartosci[n][1];
                                }
                                else
                                {
                                        zwrotna = zacznij_przypadek( ile_arg, *wskaznik, n, poziom);
                                        rekordy[*wskaznik].wartosci[n][1] = zwrotna;
                                        return zwrotna;
                                }
                        }
                }
        }

        // tworzenie nowego przypadku w rekordzie
        rekordy[*wskaznik].ilosc++;
        rekordy[*wskaznik].wartosci = (void*)realloc(  rekordy[*wskaznik].wartosci ,sizeof(int*) * rekordy[*wskaznik].ilosc);
        rekordy[*wskaznik].wartosci[rekordy[*wskaznik].ilosc - 1] = (void*)realloc( rekordy[*wskaznik].wartosci[rekordy[*wskaznik].ilosc - 1] ,sizeof(int) * ( ile_arg+2));
        rekordy[*wskaznik].wartosci[rekordy[*wskaznik].ilosc - 1][0] = ile_arg;
        rekordy[*wskaznik].wartosci[rekordy[*wskaznik].ilosc - 1][1] = 3;
        memcpy( &rekordy[*wskaznik].wartosci[rekordy[*wskaznik].ilosc - 1][2], &wskaznik[1], ile_arg*sizeof(int));

        zwrotna = zacznij_przypadek( ile_arg, *wskaznik, n, poziom);
        rekordy[*wskaznik].wartosci[n][1] = zwrotna;
        return zwrotna;
}

int rozwin_przypadek (int i, int k, int m, int ile_arg, int poziom)
{
        int* tablica_arg = (void*)malloc ( sizeof(int)* 2* ile_arg );
        int n;
        for( n = 0; n < ile_arg; n++)
        {
                tablica_arg[n] = reguly[i].wynik[n+2];
                tablica_arg[n+1] = rekordy[k].wartosci[m][n+2];
        }
        int* uzupelniona_regula = (void*)malloc ( sizeof(int) * reguly[i].ile_r );

        int stan = 0;        // 1- wczytana nazwa danej, oczekiwanie na argumenty;
        int l;
        for( n = 0; n < reguly[i].ile_r; n++)
        {
                if( reguly[i].regula[n] < 0 )
                {
                        stan = 0;
                        uzupelniona_regula[n] = reguly[i].regula[n];
                }
                else
                {
                        if(stan == 0)
                        {
                                stan = 1;
                        }
                        if(stan == 1)
                        {
                                for(l =0; l < (2*ile_arg); l+=2)
                                        if( reguly[i].regula[n] == tablica_arg[l] )
                                        {
                                                uzupelniona_regula[n] = tablica_arg[l+1];
                                                break;
                                        }
                        }
                }
        }

        // czesc odpowiedzialna za zapisane poziomy
        poziomy = (void*)realloc ( poziomy, sizeof(poziom) * (ostatni_poziom+1));
        poziomy[ostatni_poziom].ile_r=reguly[i].ile_r;
        poziomy[ostatni_poziom].ile_w=reguly[i].ile_w;
        poziomy[ostatni_poziom].poziom = poziom;
        poziomy[ostatni_poziom].regula = (void*)realloc( poziomy[ostatni_poziom].regula, poziomy[ostatni_poziom].ile_r * sizeof(int) );
        poziomy[ostatni_poziom].wynik = (void*)realloc( poziomy[ostatni_poziom].wynik, poziomy[ostatni_poziom].ile_w * sizeof(int) );
        memcpy ( poziomy[ostatni_poziom].regula, uzupelniona_regula, poziomy[ostatni_poziom].ile_r * sizeof(int) );
        poziomy[ostatni_poziom].wynik[0] = reguly[i].wynik[0];
        poziomy[ostatni_poziom].wynik[1] = reguly[i].wynik[1];
        for(n=0; n < ile_arg; n++)
        {
                poziomy[ostatni_poziom].wynik[n+2] = tablica_arg[n+1];
        }
        ostatni_poziom++;
        ///////////////////////////////////////////////

        int* poczatek_nazwy;
        int ile_arg2 =0 ;
        stan = 1;             // 1 - wczytywanie argumentow; 0 - poza nazwa i argumentami
        int wartosc;
        int znak = 0;
        int dlugosc;
        for( n = 0; n < reguly[i].ile_r; n++)
        {
                if( uzupelniona_regula[n] > 0 && stan == 0)
                {
                        poczatek_nazwy = &uzupelniona_regula[n];
                        stan = 1;
                }
                else if ( uzupelniona_regula[n] > 0 && stan == 1)
                {
                        ile_arg2 ++;
                }
                else if ( stan == 1 )
                {
                        stan = 0;
                        wartosc = szukanie_wartosci( poczatek_nazwy, ile_arg2, poziom+1 );
                        uzupelniona_regula[znak] = wartosc;
                        znak++;
                        dlugosc++;
                        ile_arg2 = 0;
                }
                else
                {
                        dlugosc++;
                        uzupelniona_regula[znak] = uzupelniona_regula[n];
                        znak++;
                }
        }
        wartosc =  obliczanie_wzoru (uzupelniona_regula, dlugosc);
        poziomy[ostatni_poziom-1].wartosc = wartosc;
        return wartosc;
}

int zacznij_przypadek( int ile_arg, int k, int  m , int poziom)
{
        int i;
        int zwracana;
        for(i=0; i<ile_wzorow; i++)
        {
                if(reguly[i].wynik[1] == k && (reguly[i].ile_w-2) == ile_arg )
                {
                        zwracana = rozwin_przypadek ( i, k, m, ile_arg, poziom );
                        if( zwracana == 0 )
                        {
                                if( reguly[i].wynik[0] == -1 )
                                        return 1;
                                else
                                        return 3;
                        }
                        else if( zwracana == 1)
                        {
                                if( reguly[i].wynik[0] == -1 )
                                        return 0;
                                else
                                        return 1;
                        }
                }
        }
}

void w_tyl()
{
        int i,n,m;
        for(i=0; i<ostatni_poziom; i++)
        {
                for(m=0; m<ostatni_poziom; m++)
                {
                        if(poziomy[m].poziom == i)
                        {
                                for(n=i;n>0;n--)
                                        printf("\t");
                                wyswietl( poziomy[m].regula, poziomy[m].ile_r);
                                printf( " => ");
                                wyswietl( poziomy[m].wynik, poziomy[m].ile_w);
                                printf( "\n");
                        }
                }
        }
        for(i=(ostatni_poziom-1); i>=0 ; i--)
        {
                for(m=(ostatni_poziom-1); m>=0; m--)
                {
                        if(poziomy[m].poziom == i)
                        {
                                for(n=i;n>0;n--)
                                        printf("\t");
                                wyswietl( poziomy[m].regula, poziomy[m].ile_r);
                                printf( " => ");
                                wyswietl( poziomy[m].wynik, poziomy[m].ile_w);
                                printf( "     " );
                                if( poziomy[m].wartosc == 1 )
                                        printf("T\n");
                                else
                                        printf("F\n");
                        }
                }
        }
}

void w_przod()
{
        int i, n, m;
        for(i = (ostatni_poziom-1); i>=0; i--)
        {
                for(m = (ostatni_poziom-1); m>=0; i--)
                {
                        if(poziomy[m].poziom == i)
                        {
                                for(n=i;n>0;n--)
                                        printf("\t");
                                wyswietl( poziomy[m].regula, poziomy[m].ile_r);
                                printf( " => ");
                                wyswietl( poziomy[m].wynik, poziomy[m].ile_w);
                                printf( "      " );
                                if( poziomy[m].wartosc == 1 )
                                        printf("T\n");
                                else
                                        printf("F\n");
                                printf("\n");
                        }
                }
        }
}

void wnioskowanie(int tryb)
{
        int k, m;
        znajdz_szukana(&k, &m);
        int zwrotna;
        int ilu_arg_szukana;
        while(znajdz_szukana(&k, &m) != -1)
        {
                ilu_arg_szukana = rekordy[k].wartosci[m][0];
                zwrotna = zacznij_przypadek(ilu_arg_szukana, k, m, 0);
                rekordy[k].wartosci[m][1] = zwrotna;
        }

        if( tryb )
                w_tyl();
        else
                w_przod();
}

int main(int args, char ** argv)
{
        char* szukane = wczytaj_plik (argv[1]);
        char* dane = wczytaj_plik (argv[2]);
        char* wzory = wczytaj_plik (argv[3]);

        ile_danych = wczytaj_dane (szukane, dane);
        wczytaj_wzory ( wzory, reguly );
        if( strcmp("-wprzod", argv[4] ) == 0 )
                wnioskowanie(0);
        else if( strcmp("-wtyl", argv[4] ) == 0 )
                wnioskowanie(1);
}

