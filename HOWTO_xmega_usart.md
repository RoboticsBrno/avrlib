XMEGA: Jak rozchodit sériovou linku UART/USART
==============================================

Ve zkratce:
1. Křižte Rx a Tx
2. Zapněte DFLL
3. [Xmega Baud Rate Calculator][calc]


---

Sériová linka (_UART/USART_) je jedna z nejpoužívanějších funkcí mikrokontrolérů. Pro naše účely je navíc nepostradatelná z hlediska debugování. Nejsme totiž vybaveni programovacími hračkami s funkcí OCD (_On-Chip Debug_), zato máme ale úžasné Shupito, které umožňuje vytvořit komunikační linku od Xmegy až do PC. K této COM# lince se potom připojíte v programu Lorris.


Křížení Rx/Tx
-------------

	Rx ----> Tx
	Tx ----> Rx

That's it. Kdyby se vám náhodou povedlo toto zapojení nedodržet, Shupito má na obou linkách ochranné odpory, takže i v případě `Tx---->Tx` se nic hrozného nestane. Jenom to nebude fungovat.


Rychlost přenosu
----------------

Rychlost sériové linky se udává v jednotce [Baud](http://cs.wikipedia.org/wiki/Baud). Pro nás to ale znamená prostě počet _bitů za sekundu_. Existují exotické rychlosti typu 9600, 38400, 115200, atd... Ačkoli je morálně správné se těchto rychlostí držet, není k tomu v dnešní době jediný důvod. Pro začátek ale použijeme bitrate _38400_, protože je v Lorris defaultně nastavený.

V ideálním případě by tedy měly být rychlosti vysílače a přijímače 100% shodné. Bohužel z technických důvodů toho skoro nikdy nelze dosáhnout, my se ale spokojíme s odchylkou cca. +-2%. Důvodem pro neshody rychlostí je způsob, jakým se odvozuje _baudrate_ od frekvence systému.

Existuje způsob jak spočítat hodnoty řídicích registrů pro konkrétní periferní frekvenci čipu a požadovaný baudrate. Lze pro to použít např. [tento kalkulátor][calc], který spočítá možné kombinace hodnot dvou řídicích registrů _BAUDCTRLA_ a _BAUDCTRLB_. Ty obsahují 12-bitovou hodnotu _BSEL_ a 4-bitovou _BSCALE_. To pro nás ale není podstatné, budeme používat jen konkrétní konstanty a avrlib za nás už uložení do registrů provede.

Oscilátor
---------
Standardně Xmega startuje na 2 MHz oscilátoru. To znamená, že provede 2 000 000 instrukcí za vteřinu. Xmega je ovšem vybavena i dalšími oscilátory a to konkrétně 32,768 kHz hodinovým, 32 kHz úsporným a v neposlední řadě 32 MHz standardní.

Tyto oscilátory můžete zapínat a vypínat z programu a zároveň se mezi nimi přepínat. Můžete tedy po nastartování čipu přepnout Xmegu z 2 MHz na 32 MHz (víc kód níže).

Zkopírujte tento kód do funkce _main_, abyste si nastavili Xmegu na 32 MHz oscilátor. V další části návodu, již budeme předpokládat, že Xmega běží na 32 MHz.

	// Run at 32MHz
	OSC.CTRL = OSC_RC32MEN_bm | OSC_RC2MEN_bm | OSC_RC32KEN_bm;
	while ((OSC.STATUS & OSC_RC32MRDY_bm) == 0)
	{
	}

	CCP = CCP_IOREG_gc;
	CLK.CTRL = CLK_SCLKSEL_RC32M_gc;

	OSC.CTRL = OSC_RC32MEN_bm | OSC_RC32KEN_bm;
	while ((OSC.STATUS & OSC_RC32KRDY_bm) == 0)
	{
	}


DFLL
----
DFLL znamená _Digital Frequency-Locked Loop_. Používá se pro výrazné zpřesnění frekvence 2 MHz a 32 MHz oscilátoru na Xmeze a je víceméně nutná pro provoz UARTu (pokud nepoužíváte krystal). Pokud váš kód ještě neobsahuje řádek podobný tomuto, určitě ho přidejte (vyžaduje běžící 32 kHz oscilátor).

	DFLLRC32M.CTRL = DFLL_ENABLE_bm;


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

	#include "avrlib/uart_xmega.hpp"
	#include "avrlib/async_usart.hpp"
	#include "avrlib/format.hpp"

	using namespace avrlib; // pro jednoduchost


Vytvoření instance
------------------

Dále je třeba vytvořit instanci/objekt třídy, která bude zastupovat naší sériovou linku. Je dobré tuto deklaraci umístit do rozsahu souboru (před funkci _main_, ale až za _include_).

	async_usart<uart_xmega, 32, 64> debug; // 32 bajtů pro přijímací buffer a 64 pro odesílací buffer, název objektu je debug (můžete si zvolit jakýkoliv název)


Nastavení pinů
--------------

Musíme nastavit TX pin na výstup a RX nechat na vstupu, ideálně mu ještě povolit pull-up. V tomto příkladě použijeme _USARTC0_, tedy nultý na portu C (piny 2 a 3).

    #include "avrlib/xmega_pin.hpp" // více info o knihovně xmega_pin v samostatném článku
	AVRLIB_DEFINE_XMEGA_PIN(pin_debug_tx, PORTC, 3);
	AVRLIB_DEFINE_XMEGA_PIN(pin_debug_rx, PORTC, 2); 
    // umístit někde na začátek programu (před funkci _main_)
    
    // umístěte do funkce _main_ 
	pin_debug_rx::pullup();
	pin_debug_tx::make_high();


Otevření USARTu
---------------

	debug.usart().open(USARTC0, (-1 << 12)|102); // druhý parametr je nastavení rychlosti, aktuálně odpovídá 38400; pro rychlost 115200: (-3 << 12)|131);  /*115200 bps @ 32 MHz*/

Použití
-------

Nyní už můžeme používat všelijaké funkce typu _send()_, _send_bin()_, _format()_, metodu _write()_ a další (funkce format() - složitější použití -> popíšeme v budoucnu).

	debug.write(0xFF);
	send(debug, "Ahoj\n");
	format(debug, "Vrchol zásobníku je: %") % SP;
	debug.flush();
    
Kompletní program
-----------------

	#include "avrlib/uart_xmega.hpp"
	#include "avrlib/async_usart.hpp"
	#include "avrlib/format.hpp"
    #include "avrlib/xmega_pin.hpp"
    
    using namespace avrlib;
    
	AVRLIB_DEFINE_XMEGA_PIN(pin_debug_tx, PORTC, 3);
	AVRLIB_DEFINE_XMEGA_PIN(pin_debug_rx, PORTC, 2); 
    
    async_usart<uart_xmega, 32, 64> debug; 
    
    int main(void)
    {
       	// Run at 32 MHz
        OSC.CTRL = OSC_RC32MEN_bm | OSC_RC2MEN_bm | OSC_RC32KEN_bm;
        while ((OSC.STATUS & OSC_RC32MRDY_bm) == 0)
        {
        }

        CCP = CCP_IOREG_gc;
        CLK.CTRL = CLK_SCLKSEL_RC32M_gc;

        OSC.CTRL = OSC_RC32MEN_bm | OSC_RC32KEN_bm;
        while ((OSC.STATUS & OSC_RC32KRDY_bm) == 0)
        {
        }
        
        // DFLL
        DFLLRC32M.CTRL = DFLL_ENABLE_bm;
        
        pin_debug_rx::pullup();
        pin_debug_tx::make_high();
        
        debug.usart().open(USARTC0, (-1 << 12)|102); // aktuálně odpovídá 38400; pro rychlost 115200: (-3 << 12)|131); 
        
        debug.write(0xFF);
        send(debug, "Ahoj\n");
        send_int(debug, SP);
        debug.flush();
        
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

V případě nejasnosti, nefunkčnosti nebo jakéhokoliv jiného problému mě kontaktujte na email: _<mailto:paral@robotikabrno.cz>_


[calc]: http://www.avrcalc.elektronik-projekt.de/xmega/baud_rate_calculator "Xmega Baud Rate Calculator"
[atmelstudio]: http://www.atmel.com/System/GetBinary.ashx?target=tcm:26-49769&type=soft&actualTarget=tcm:26-49767 "Atmel Studio"
[avrlibrepo]: https://technika.junior.cz/hg/avrlib "avrlib repozitář"

<!--- Jarek Malec @ 8. 9. 2013; upraveno: Jarek Páral @ 30.9.2014  -->