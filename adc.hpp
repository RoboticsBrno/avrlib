#ifndef AVRLIB_ADC_HPP
#define AVRLIB_ADC_HPP

#include <avr/io.h>

namespace avrlib {
	
	
enum adc_timer_mode	
{
	adc_prescaler_2 = 1,
	adc_prescaler_4,
	adc_prescaler_8,	
	adc_prescaler_16,
	adc_prescaler_32,
	adc_prescaler_64,
	adc_prescaler_128	
};

/**
 * \brief Třída pro synchronní (čekající) A/D převod.
 * \details sync_adc lze použít i asynchronně, hlavní předností
	synchronního převodu je, že ihned po provedení operátoru ()
	je výsledek k dispozici bez nutnosti čekání ve vašem kódu.
 */
class sync_adc
{
	public:
	/**
	 *  \brief Konstruktor pro vytvoření instance třídy
	 *  
	 *  \param [in] channel Kanál A/D převodníku
	 *  \param [in] reverse 
	 *  
	 *  \details Konstruktor je explicitní, tzn. je potřeba
	 *  	ho použít následovně.
	 *  \code
	 *  sync_adc muj_adc(0, false);
	 *  \endcode
	 */
	explicit sync_adc(uint8_t channel, bool reverse = false)
		: m_channel(channel), m_reverse(reverse)
	{
	}
	
	/*! Inicializuje kanál A/D převodníku
	 *  
	 *  \details Metoda je statická, tzn. používá se na třídu samotnou,
	 *  	ne na vytvořenou instanci.
	 *  \code
	 *  sync_adc::init(adc_prescaler_2);
	 *  \endcode
	 *  \param [in] t Předdělička pro hodiny převodníku
	 */
	static void init(adc_timer_mode t)
	{
		ADCSRA = (1<<ADEN) | t;		
	}
	
	/*! Odstartuje převod
	 */
	void start()
	{
		ADCSRA |= (1<<ADSC);
	}
	
	/*! 
	 *  \return Vrátí, zda převod právě probíhá
	 */
	bool running() const
	{
		return (ADCSRA & (1<<ADSC)) != 0;
	}
	
	/*! Po ukončení převodu vrátí hodnotu výsledku
	 *  \return Výsledek
	 *  
	 *  \details Výsledek je platný pouze tehdy, když je převod
	 *  	už ukončen
	 */
	uint16_t value() const
	{
		uint16_t res = ADCL;
		res |= ADCH << 8;
		return res;
	}

	/*! Nejjednodušší funkce/operátor pro převod.
	 *  \return Přímo vrací výsledek snadno a rychle
	 *  
	 *  \code
	 *  uint16_t vysledek = muj_adc();
	 *  \endcode
	 */
	uint16_t operator()()
	{
		ADMUX = (1<<ADLAR) | m_channel;
		ADCSRA |= (1<<ADSC);
		while ((ADCSRA & (1<<ADIF)) == 0)
		{
		}
		/*ADCSRA |= (1<<ADSC);
		while ((ADCSRA & (1<<ADIF)) == 0)
		{
		}*/

		uint16_t res = ADCL;
		res |= ADCH << 8;

		if (m_reverse)
			res = -res;

		return res;
	}

private:
	uint8_t m_channel;
	bool m_reverse;
};

/*! Třída pro asynchronní převod.
 * \details Uživatel musí sám v kódu počkat, než se převod provede.
 */
class async_adc
{
public:

	typedef uint16_t value_type;
	
	/*! Konstruktor pro vytvoření instance.
	 *  \param [in] channel Kanál A/D převodníku
	 *  \param [in] reverse ?
	 *  
	 *  \see sync_adc
	 */
	explicit async_adc(uint8_t channel, bool reverse = false)
		: m_channel(channel), m_reverse(reverse), m_value(0), m_new_value(false)
	{
	}

	/*! Inicializace A/D převodníku. Nutné pro fungování.
	 *  \param [in] t Předdělička hodin pro převodník.
	 *  
	 *  \see sync_adc
	 */
	static void init(adc_timer_mode t)
	{
		ADCSRA = (1<<ADEN) | t;
	}	

	/*! Odstartuje A/D převod
	 */
	void start()
	{
		ADMUX = (1<<ADLAR) | m_channel;
		ADCSRA |= (1<<ADSC);
	}

	/*! Použijte pro zjištění, zda převod běží a zároveň
	 *  	pokud už skončil, uložení výsledku do m_value
	 *
	 *  \return Vrací zda převod už proběhl
	 */
	bool process()
	{
		if ((ADCSRA & (1<<ADSC)) != 0)
			return false;

		m_value = ADCL;
		m_value |= ADCH << 8;

		if (m_reverse)
			m_value = -m_value;
		
		m_new_value = true;
		
		return true;
	}

	/*! Vrátí výsledek
	 *  \return Hodnota výsledku
	 */
	value_type value()
	{
		m_new_value = false;
		return m_value;
	}
	
	/*! Zjistí, zda je dostupná nová hodnota
	 *  \return Vrátí, zda je dostupná nový výsledek
	 */
	bool new_value() const { return m_new_value; }
		
	/*! Vrátí kanál A/D převodníku na který je konkrétní
	 *  	instance vztažená
	 *  
	 *  \return Vrátí číslo kanálu ... WTF
	 */
	uint8_t channel() const { return m_channel; }

private:
	uint8_t m_channel;
	bool m_reverse;
	volatile value_type m_value;
	volatile bool m_new_value;
};

}

#endif
