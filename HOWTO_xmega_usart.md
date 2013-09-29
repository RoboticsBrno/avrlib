Jak rozchodit sériovou linku na čipech Xmega
============================================

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


DFLL
----
DFLL znamená _Digital Frequency-Locked Loop_. Používá se pro výrazné zpřesnění frekvence 2MHz a 32MHz oscilátoru na Xmeze a je víceméně nutná pro provoz UARTu (pokud nepoužíváte krystal). Pokud váš kód ještě neobsahuje řádek podobný tomuto, určitě ho přidejte (vyžaduje běžící 32kHz oscilátor).

	DFLLRC32M.CTRL = DFLL_ENABLE_bm;


Program
=======

V této chvíli nám přichází na pomoc avrlib, který nás odprošťuje od nutnosti přistupovat k jednotlivým registrům čipu. Můžeme tak pomocí tříd a funkcí snadno ovládat hardware.

Budeme potřebovat:
1. [Atmel Studio][atmelstudio]
2. [avrlib repo][avrlibrepo]


Vložení potřebných knihoven
---------------------------

Jako první je třeba _includovat_ knihovny avrlibu, které se starají o obsluhu USARTu.

	#include "avrlib/uart_xmega.hpp"
	#include "avrlib/async_usart.hpp"
	#include "avrlib/format.hpp"

	using namespace avrlib; // pro jednoduchost


Vytvoření instance
------------------

Dále je třeba vytvořit instanci/proměnnou třídy, která bude zastupovat naší sériovou linku. Je dobré tuto deklaraci umístit do rozsahu souboru.

	async_usart<uart_xmega, 32, 32> debug; // 32 bajtů pro příjem i vysílání, název proměnné je debug


Nastavení pinů
--------------

Musíme nastavit TX pin na výstup a RX nechat na vstupu, ideálně mu ještě povolit pull-up. V tomto příkladě použijeme _USARTC0_, tedy nultý na portu C (piny 2 a 3).

	AVRLIB_DEFINE_XMEGA_PIN(pin_debug_tx, PORTC, 3); //
	AVRLIB_DEFINE_XMEGA_PIN(pin_debug_rx, PORTC, 2); // umístit někde na začátek

	pin_debug_rx::pullup();
	pin_debug_tx::make_high();


Otevření USARTu
---------------

	debug.usart().open(USARTC0, (-1 << 12)|102); // druhý parametr je nastavení rychlosti, 38400

Použití
-------

Nyní už můžeme používat všelijaké funkce typu _send()_, _send_bin()_, _format()_, metodu _write()_ atd.

	debug.write(0xFF);
	send(debug, "Ahoj\n");
	format(debug, "Vrchol zásobníku je: %") % SP;
	debug.flush();

[calc]: http://www.avrcalc.elektronik-projekt.de/xmega/baud_rate_calculator "Xmega Baud Rate Calculator"
[atmelstudio]: http://www.atmel.com/System/GetBinary.ashx?target=tcm:26-49769&type=soft&actualTarget=tcm:26-49767 "Atmel Studio"
[avrlibrepo]: https://technika.junior.cz/hg/avrlib "avrlib repozitář"

<!--- Jarek Malec @ 8. 9. 2013 -->