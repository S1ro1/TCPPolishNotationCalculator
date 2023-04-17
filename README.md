## IPK 2023 - Projekt 2 
### Varianta IOTA - vzdálená kalkulačka

#### Matej Sirovatka - xsirov00

### Popis projektu

Cieľom projektu je implementovať jednoduchú kalkulačku výrazov v poľskej notácii. \
Kalkulačka je implementovaná ako serverová aplikácia a podporuje komunikáciu pomocou protokolu TCP alebo UDP.

### Spustenie

Server sa preloží pomocou príkazu make
```console
$ make
```
Následne sa dá server spustiť pomocou príkazu
```console
$ ./ipkcpd [-h host] [-p port] [-m mode]
```
Kde:
- host - host adresa
- port - číslo portu
- mode - režim komunikácie (TCP alebo UDP)

Tieto argumenty sú povinné a podporované len v tomto poradí.

### Implementácia

Celá implementácia je rozdelená do viacerých modulov. Každý modul sa sklada z hlavičkového súboru a zdrojového súboru.
- **main.cpp** - hlavný modul, ktorý obsahuje funkciu main
- **utils.{cpp, h}** - modul, ktorý obsahuje funkcie pre spracovanie argumentov a rôzne pomocné funkcie
- **Token.{cpp, h}** - modul, ktorý obsahuje triedu Token, ktorá reprezentuje 1 element výrazu
- **Parser.{cpp, h}** - modul, ktorý obsahuje triedu Parser, ktorá slúži na spracovanie výrazu a výpočet výsledku
- **UDPConnection.{cpp, h}** - modul, ktorý obsahuje triedu UDPConnection, ktorá slúži na komunikáciu pomocou protokolu UDP
- **TCPServer.{cpp, h}** - modul, ktorý obsahuje triedu TCPServer, ktorej úlohou je starať sa o jednotlivé TCP spojenia
- **TCPConnection.{cpp, h}** - modul, ktorý obsahuje triedu TCPConnection, ktorá predstavuje jedno TCP spojenie

Beh programu začína v súbore main.cpp. V tejto funkcii sa spracujú argumenty a vytvorí sa objekt triedy UDPConnection alebo TCPServer. \

### Spracovanie výrazov 

- Výraz sa spracováva pomocou triedy Parser, ktorá má metódu parse_expr, ktorá ako argument dostane reťazec reprezentujúci výraz
- Metóda parse_expr je implementovaná ako stavový automat so zásobníkom, do ktorého sa ukladajú Tokeny a počítadlom, ktoré sa stará o počet zátvoriek
- V prípade, že sa vyskytne chyba, metóda vyhodí výnimku, ktoré sú neskor odchytené samotnou TCP/UDP implementáciou
- Inak sa tokeny zapisujú do zásobníku Tokenov, v prípade že sa vyskytne ')' tak sa celý výraz na vrchu zásobníku (po poslednú ')') vyhodnotí metódou reduce_stack
- Táto metóda rozdelí zásobník na 2, 1 obsahuje výraz na redukciu a druhý obsahuje zbytok z povodného zásobníka
- Výraz sa vyhodnotí pomocou funkcie accumulate zo štandardnej knižnice C++ a výsledok sa zapíše na vrch druhého zásobníka, a ten sa vráti ako výsledok metódy parse_expr
- V prípade, že sa vyskytne chyba, metóda vyhodí výnimku, ktoré sú neskor odchytené samotnou TCP/UDP implementáciou

### UDP

- Pri vytvorení objektu triedy UDPConnection sa vytvorí socket typu SOCK_DGRAM a nastavia sa potrebné parametre
- Zavolá sa funkcia metóda triedy listen, ktorá v nekonečnom cykle čaká na príchod správy vo funkcíi recvfrom
- Pri príchode správy sa spracuje, v prípade že správa má nesprávny op_code, odošle sa chybová správa
- V prípade že správa má správny op_code, načíta sa počet bajtov špecifikovaný v 2. bajte správy, oreže sa správa a tá sa spracuje
- Podľa toho, či metóda triedy Parser parse_expr, ktorá ako argument dostane reťazec reprezentujúci správu,  vyhodí výnimku, nastaví sa status bajt na 1 a odošle sa správa s obsahom "Couldn't parse the expression!"
- Inak sa status byte nastaví na 0 a odošle sa správa s výsledkom výpočtu

### TCP

- Pri vytvorení objektu triedy TCPServer sa vytvorí socket typu SOCK_STREAM a nastavia sa potrebné parametre
- Objekt takisto obsahuje zoznam objektov triedy TCPConnection, ktoré predstavujú jednotlivé TCP spojenia, ktorých atribút status predstavuje stav spojenia a socket_num predstavuje číslo socketu
- Tie sú inicializované na hodnotu NOT_CONNECTED a 0
- Zavolá sa funkcia metóda triedy WaitConnections, ktorá v nekonečnom cykle pomocou funkcie pselect čaká na aktivitu na 1 zo client socketov alebo príjme nové spojenie
- Pri príchode spojenia sa vytvorí nový objekt triedy TCPConnection so statusom INITIALIZED a príslušným číslom socketu a pridá sa do zoznamu
- Potom sa iteruje cez zoznam spojení a v prípade že číslo socketu nie je 0, zavolá sa metóda triedy TCPConnection HandleConnection, ktorá spracuje spojenie
- V prípade, že status daného spojenia je DISCONNECTED, zavrie sa socket a uvoľní sa miesto v zozname spojení
- Metóda HandleConnection potom pomocou funkcie recv prijíma správy až dokým sa v nich nenachádza znak nového riadku
- Potom sa správa konvertuje do typu string_stream zo śtandardnej knižnice C++ a iteruje sa cez ňu pomocou funkcie getline
- Každý riadok sa potom samostatne spracuje, a to podľa jej obsahu a toho, v akom stave je spojenie, ak je spojenie v stave INITIALIZED, tak sa očakáva že správa bude obsahovať HELLO
- Ak áno, stav sa zmení na CONNECTED a odošle sa HELLO\n, a v ďalších riadkoch/správach sa už očakáva že bude obsahovať výraz
- Ak je stav CONNECTED, najprv sa riadok spracuje, či začína s "SOLVE ", ak áno, tak sa riadok oreže a zostane len výraz ktorý je ďalej spracovaný metódou triedy Parser parse_expr
- V prípade, že správa nie je v správnom formáte, odošle sa BYE\n a stav sa zmení na DISCONNECTED, BYE\n sa odošle aj v prípade, že doručená správa obsahuje BYE\n
- V prípade správneho výrazu sa vypočíta výsledok a odošle sa v tvare "RESULT {výsledok}\n"


### Testovanie

- Testovanie bolo vykonávané pomocou python skriptov **tests/{udp_tests, tcp_tests}.py**, a knižnice pytest
- Niektoré zložitejšie testy boli vykonávané ručne, napr. testovanie viacerých simultánných spojení, CTRL+C atď.
- Testovanie sa vykonávalo na lokálnom počítači, kde bol spustený server a spúšťané buď TCP, alebo UDP testy

### UDP Test cases
| Input | Output | Status |
|-------|--------| ------ |
| b"\000\007(+ 1 2)" | b"\x01\x00\x013" | Passed |
| b"\000\007(* 3 4)" | b"\x01\x00\x0212" | Passed |
| b"\000\010(- 10 4)" | b"\x01\x00\x016" | Passed |
| b"\000\010(/ 12 3)" | b"\x01\x00\x014" | Passed |
| b"\000\017(+ 1 (* 2 3))" | b"\x01\x00\x017" | Passed |
| b"\x01\x01" | b"\x01\x01\x05Error" | Passed |
| b"\x00\x07(+ 1 2)345" | b"\x01\x00\x013" | Passed |
| b"\x00\x07(+ 1 2" | b"\x01\x01\x1aCouldn't parse expression!" | Passed |

### TCP Test cases
| Input | Output | Status |
|-------|--------| ------ |
| b"HELLO\nSOLVE (+ 1 2)\nBYE\n" | b"HELLO\nRESULT 3\nBYE\n" | Passed |
| b"HELLO\nSOLVE (* 3 4)\nBYE\n" | b"HELLO\nRESULT 12\nBYE\n" | Passed |
| b"HELLO\nSOLVE (- 10 4)\nBYE\n" | b"HELLO\nRESULT 6\nBYE\n" | Passed |
| b"HELLO\nSOLVE (/ 12 3)\nBYE\n" | b"HELLO\nRESULT 4\nBYE\n" | Passed |
| b"HELLO\nSOLVE (+ 1 (* 2 3))\nBYE\n" | b"HELLO\nRESULT 7\nBYE\n" | Passed |
| b"hell\nSOLVE\n" | b"BYE\n" | Passed |
| b"HELLO\nSOLVE (+ 1 2\n" | b"HELLO\nBYE\n" | Passed |
| b"HELLO\nSOLVE (/ 1 0\n" | b"HELLO\nBYE\n" | Passed |
| b"HELLO\nSOLVE (+ (+ 3 9) (+ (+ 2 (+ 6 1) 4) 9 7) 5)\nBYE\n" | b"HELLO\nRESULT 46\nBYE\n" | Passed |
| b"HELLO\nSOLVE (- (- 3 9) (- (- 2 (- 6 1) 4) 9 7) 5)\nBYE\n" | b"HELLO\nRESULT 12\nBYE\n" | Passed |
| b"HELLO\nSOLVE (* (* 3 9) (* (* 2 (* 6 1) 4) 9 7) 5)\nBYE\n" | b"HELLO\nRESULT 408240\nBYE\n" | Passed |
| b"HELLO\nSOLVE (/ (/ 7000 9) (/ (/ 86207 (/ 16 1) 4) 2 7) 5)\nBYE\n" | b"HELLO\nRESULT 1\nBYE\n" | Passed |
| b"HELLO\nSOLVE (+ 1 (* 45 (+ 3 4)) (* 3 6) (/ 7 8) (/ 21 4))\nBYE\n" | b"HELLO\nRESULT 339\nBYE\n" | Passed |
| b"HELLO\nSOLVE (/ (* (- 64 4) (+ 3 2)) (- 12 4) (* (+ (/ 10 2) (- 7 4)) (/ (- 8 2) 3)))\nBYE\n" | b"HELLO\nRESULT 2\nBYE\n" | Passed |


### Zdroje
- https://linux.die.net/man/2/pselect
- https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
- https://git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs