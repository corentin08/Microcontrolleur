#include <msp430.h> 

/*
 * AUTEURS : CHAUVEL Benjamin, DUPONT Corentin
 * DATE : 15 mars 2017
 * PROJET : Interface capteur à Ultrasons
 */

void configSysClock()
{
	DCOCTL=CALDCO_16MHZ;
	BCSCTL1=CALBC1_16MHZ;
}

void ConfigTimer()
{

	P2DIR |= BIT3;
	P2SEL |= BIT3;
	P1OUT |= 0xFF;

	TACTL |= TACLR;

	TACTL |= TASSEL_2|ID_0|MC_1; // paramètre de l'horloge
	TACCR0 = 500; //Permet de diviser notre fréquence. On obtient une fréquence de 16kHz
	TACCR1 = 500; //Permet de diviser notre fréquence. On obtient une fréquence de 16kHz
	TACCTL1 |=OUTMOD_4; /*sélection du mode toggle*/


}
void writeP1(unsigned char val, unsigned char mask)
{
	P1OUT |= (val & mask);
	P1OUT &= val |~mask ;
}
void ultraSoundmgt()  //Routine d'interruption activée par le timer A à une fréquence de 16KHz
{
	//int indexTime = 0 ;
	P1DIR |= BIT4;
	P1SEL |= BIT4;
	__delay_cycles(1);
	P1OUT &=~4;
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	configSysClock();
	ConfigTimer();
	ultraSoundmgt();
	while(1)
	return 0;
}

#pragma vector = TIMERA0_VECTOR;
__interrupt void TimeA1_ISR(void)
{
	ultraSoundmgt();
}
