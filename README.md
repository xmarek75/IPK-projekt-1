# IPK - Projekt 1

Server, vytvoreny v jazyce C, komunikujici prostrednictvim protokolu HTTP, ktery poskytuje informace o systemu.
 
# Autor
Pavel Marek
* login: xmarek75
* email: xmarek75@vutbr.cz

## Zadani

Úkolem je vytvoření serveru v jazyce C/C++ komunikujícího prostřednictvím protokolu HTTP, který bude poskytovat různé informace o systému. Server bude naslouchat na zadaném portu a podle url bude vracet požadované informace. Server musí správně zpracovávat hlavičky HTTP a vytvářet správné HTTP odpovědi. Typ odpovědi bude text/plain. Komunikace se serverem by měla být možná jak pomocí webového prohlížeče, tak nástroji typu wget a curl. Server musí být spustitelný v prostředí Linux Ubuntu 20.04 LTS  (https://ubuntu.com/).

Server bude přeložitelný pomocí Makefile, který vytvoří spustitelný soubor hinfosvc.
Tento server bude spustitelný s argumentem označující lokální port na kterém bude naslouchat požadavkům:

./hinfosvc 12345


Server bude možné ukončit pomocí CTRL+C. Server bude umět zpracovat následující tři typy dotazů, které jsou na server zaslané příkazem GET:


1. Získání doménového jména

Vrací síťové jméno počítače včetně domény, například:

GET http://servername:12345/hostname

merlin.fit.vutbr.cz


2. Získání informací o CPU 

Vrací informaci o procesoru, například:

GET http://servername:12345/cpu-name


Intel(R) Xeon(R) CPU E5-2640 0 @ 2.50GHz


3. Aktuální zátěž 

Vrací aktuální informace o zátěži. Tento vyžaduje výpočet z hodnot uvedených v souboru /proc/stat (viz odkaz níže). Výsledek je například:

GET http://servername:12345/load

65%

### Implementace

Server byl implementovan v jazyce C.
Pro implementaci bylo nutne vyuzit nekolik knihoven:

* <stdlib.h>
* <stdio.h>
* <unistd.h>
* <string.h>
* <strings.h>
* <sys/types.h>
* <sys/socket.h>
* <netinet/in.h>

## Spusteni/ukonceni serveru

* Pro spusteni serveru postupujte nasledovne:

    ```
    $ make
    $ ./hinfosvc 1212       //jako cislo portu muzete zvolit libovolne cislo
    ```
    V pripade ze chcete komunikovat se serverem ze stejneho terminalu, pridejte za cislo portu "&" 
    ```
    $ ./hinfosvc 1212 &
    ```
* Pro ukonceni serveru zmacknete klavesy CTRL+C

## Komunikace se serverem

Se serverem muzete komunikovat pomoci curl, wget nebo GET
a pomoci argumentu: hostname, cpu-name, load.
Dale ukazka komunikace se serverem vytvorenym na portu 1212.


* pomoci curl:
    ```
    $ curl http://localhost:1212/hostname
    >>xmarek75
    $ curl http://localhost:1212/cpu-name
    >>Intel(R) Core(TM) i5-6400 CPU @ 2.70GHz
    $ curl http://localhost:1212/load
    >>10,32422%
    ```

* pomoci wget:
    ```
    $ curl http://localhost:1212/hostname
    >>vytvoreni text/plain dokumentu hostname, ve kterem je ulozen vysledek"
    $ curl http://localhost:1212/cpu-name
    >>vytvoreni text/plain dokumentu cpu-name
    $ curl http://localhost:1212/load
    >>vytvoreni text/plain dokumentu load
    ```
* pomoci GET:
    ```
    $ GET http://localhost:1212/hostname
    >>xmarek75
    $ GET http://localhost:1212/cpu-name
    >>Intel(R) Core(TM) i5-6400 CPU @ 2.70GHz
    $ GET http://localhost:1212/load
    >>10,32422%
    ```