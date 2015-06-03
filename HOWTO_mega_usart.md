MEGA: Jak rozchodit sériovou linku UART/USART
==============================================

Pozn. Velká část textu je přejata z tutoriálu _XMEGA: Jak rozchodit sériovou linku UART/USART_

---

Sériová linka (_UART/USART_) je jedna z nejpoužívanějších funkcí mikrokontrolérů. Pro naše účely je navíc nepostradatelná z hlediska debugování. Nejsme totiž vybaveni programovacími hračkami s funkcí OCD (_On-Chip Debug_), zato máme ale úžasné Shupito (lze ovšem také využít převodníky ze sériové linky na USB - např. _FTDI FT232RL_ nebo jeho čínské kopie), které umožňuje vytvořit komunikační linku od Megy až do PC. K této COM# lince se potom připojíte v programu [Lorris][lorris].


Křížení Rx/Tx
-------------

	Rx ----> Tx
	Tx ----> Rx

That's it. Kdyby se vám náhodou povedlo toto zapojení nedodržet, Shupito má na obou linkách ochranné odpory, takže i v případě `Tx---->Tx` se nic hrozného nestane. Jenom to nebude fungovat.


Rychlost přenosu
----------------

Rychlost sériové linky se udává v jednotce [Baud](http://cs.wikipedia.org/wiki/Baud). Pro nás to ale znamená prostě počet _bitů za sekundu_. Existují exotické rychlosti typu 9600, 38400, 115200, atd... Ačkoli je morálně správné se těchto rychlostí držet, není k tomu v dnešní době jediný důvod. Pro začátek ale použijeme bitrate _38400_, protože je v Lorris defaultně nastavený.

V ideálním případě by tedy měly být rychlosti vysílače a přijímače 100% shodné. Bohužel z technických důvodů toho skoro nikdy nelze dosáhnout, my se ale spokojíme s odchylkou cca. +-2%. Důvodem pro neshody rychlostí je způsob, jakým se odvozuje _baudrate_ od frekvence systému.


Oscilátor
---------
Standardně Megy běží na 1 MHz oscilátoru. Je možné ale možné připojit externí krystal (oscilátor) a to v rozmezí do 16 MHz (ATmega16/128) nebo až 20 MHz (ATmega48/88/168/328). To znamená, že standardně čip provede 1 000 000 _instrukcí/operací_ za vteřinu, ale po přidání externího krystalu je schopen provést 16-20x krát více _instrukcí/operací_. 

To který oscilátor používáme, musíme zaprvé nastavit v pojistkách čipu přes programátor a zadruhé nadefinovat v programu pomocí konstrukce _#define F_CPU vase_frekvence_.

    #define F_CPU 1000000   // pro 1 MHz
    #define F_CPU 16000000  // pro 16 MHz
    #define F_CPU 20000000  // pro 20 MHz
    // vyberete jen jednu variantu



Počet sériových linek na Megách
-------------------------------

Megy standardně mívají jen jednu linku typu UART/USART, jsou ale výjimky, jako ATmega128 (a i některé další mikrokontroléry), které mají dva sériové linky. U těchto čipů je potřeba si pohlídat s kterou linkou pracujete, abyste se nedivili proč vám to nejede.    
    
Program
=======

V této chvíli nám přichází na pomoc avrlib, který nás odprošťuje od nutnosti přistupovat k jednotlivým registrům čipu. Můžeme tak pomocí tříd a funkcí snadno ovládat hardware.

Budeme potřebovat:
1. [Atmel Studio][atmelstudio]
2. [avrlib repo][avrlibrepo]


Vložení potřebných knihoven
---------------------------

Knihovnu Avrlib lze stáhnout [zde][http://technika.junior.cz/hg/avrlib/archive/57de39d37122.zip]. Tento archiv je potřeba rozbalit do složky s projektem (tam kde je soubor nazev_projektu.c nebo nazev_projektu.cpp) a následně je nutné složku přejmenovat z názvu _avrlib-XXxxXXxXXXXX_ na _avrlib_ (jinak budete mít problémy s kompilací).

Jako první je třeba _includovat_ knihovny avrlibu, které se starají o obsluhu USARTu.

    #include "avrlib/usart0.hpp"    
    #include "avrlib/usart1.hpp"    // v případě, že máte čip se dvěma sériovými linkami
	#include "avrlib/async_usart.hpp"
	#include "avrlib/format.hpp"    
    #include "avrlib/bootseq.hpp"   // knihovna pro obsluhu bootloaderu - zavaděče programu přes sériovou linku

	using namespace avrlib; // pro jednoduchost


Vytvoření instance
------------------

Dále je třeba vytvořit instanci/objekt třídy, která bude zastupovat naší sériovou linku. Je dobré tuto deklaraci umístit do rozsahu souboru (před funkci _main_, ale až za _include_).

	typedef avrlib::async_usart<avrlib::usart0, 64, 128, avrlib::nobootseq> uart_t; // 64 bajtů pro přijímací buffer a 128 pro odesílací buffer; nobootseq = bez bootloaderu
    uart_t uart(115200UL, true);    .// komunikační rychlost 115200; true = použiji přerušení pro příjem (RX = receive = příjem)     
    // název objektu je uart (můžete si zvolit jakýkoliv název - např. debug, com, data, ....)

    ISR(USART_RX_vect) { uart.process_rx(); }   // definice přerušení pro příjem
    
Nastavení pinů
--------------

Musíme nastavit TX pin na výstup a RX nechat na vstupu, ideálně mu ještě povolit pull-up. V tomto příkladě použijeme _USART0_, na portu D s piny _PD0(RX)_ a _PD1 (TX)_.

    #include "avrlib/pin.hpp"
    #include "avrlib/portb.hpp" 
    #include "avrlib/portc.hpp"
    #include "avrlib/portd.hpp" // vyberu knihovnu v závislosti na tom, s jakým portem chci pracovat (USART -> portd)
    
    typedef pin<portd, 0> uart_rx;  // vytvořen objekt uart_rx nad kterým je defínováno několik metod/funkcí
    typedef pin<portd, 1> uart_tx;  
    // funkční pro ATmega16/48/88/168

    // umístěte do funkce _main_ 
    uart_rx::pullup();
	uart_tx::make_high();
	
    
Použití
-------

Nyní už můžeme používat všelijaké funkce typu _send()_, _send_bin()_, _format()_, metodu _write()_ a další (funkce format() - složitější použití -> popíšeme v budoucnu).

	uart.write(0xFF);
	send(uart, "Ahoj\n");
	format(uart, "Vrchol zásobníku je: %") % SP;
	uart.flush();
    
Kompletní program
-----------------

    #define F_CPU 16000000UL
    
    //#include "avrlib/usart.hpp"     // pro ATmegu16
    #include "avrlib/usart0.hpp"    // pro ATmegu48/88/128/168/328 
    //#include "avrlib/usart1.hpp"    // pro ATmegu128
	#include "avrlib/async_usart.hpp"
	#include "avrlib/format.hpp"    
    #include "avrlib/bootseq.hpp"

    #include "avrlib/pin.hpp"
    #include "avrlib/portd.hpp" 
    
    using namespace avrlib;
    
    typedef pin<portd, 0> uart_rx; 
    typedef pin<portd, 1> uart_tx;  
    
    // ATmega16
    //typedef avrlib::async_usart<avrlib::usart, 64, 128, avrlib::nobootseq> uart_t; 
    
    // Ostatní
    typedef avrlib::async_usart<avrlib::usart0, 64, 128, avrlib::nobootseq> uart_t; 
    uart_t uart(115200UL, true);
    
    ISR(USART_RX_vect) { uart.process_rx(); } 
        
    int main(void)
    {
        uart_rx::pullup();
        uart_tx::make_high();
        
        uart.write(0xFF);
        send(uart, "Ahoj\n");
        send_int(uart, SP);
        uart.flush();
        
        /*
            - program vytvoří objekt debug
            - nastaví oscilátor na 32 MHz
            - aktivuje DFLL
            - nastaví pinu RX a TX
            - otevře usartu
            - předá do odesílacího bufferu jeden bajt 0xFF
            - předá do odesílacího bufferu řetězec "Ahoj"
            - předá do odesílacího bufferu hodnotu registru SP a ta se zobrazí jako číslo
            - odešle data z odesílacího bufferu po sériové lince ven z čipu
        */
    }
    

Kontakt
-----------------

V případě nejasnosti, nefunkčnosti nebo jakéhokoliv jiného problému mě kontaktujte na email: _paral@robotikabrno.cz_
    
[lorris]: http://tasssadar.github.io/Lorris/cz/index.html "Lorris Toolbox"
[atmelstudio]: http://www.atmel.com/System/GetBinary.ashx?target=tcm:26-56037&type=soft&actualTarget=tcm:26-56036 "Atmel Studio"
[avrlibrepo]: https://technika.junior.cz/hg/avrlib "avrlib repozitář"

<!--- Jarek Páral @ 30. 9. 2014 -->