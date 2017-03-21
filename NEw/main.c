#include <msp430.h> 

/*
 * AUTEURS : CHAUVEL Benjamin, DUPONT Corentin
 * DATE : 21 mars 2017
 * PROJET : Interface capteur à Ultrasons
 */
#define TGEN		/*VALEUR A DEFINIR 2 */
#define TRECEPT		/*VALEUR A DEFINIR  */
#define TE   /*    */
#define BUFFER 16   /*Taille allouée au stockage */

unsigned char BufferCompt = 0; /*Compte le nombre de valeur mit dans le BUFFER*/
unsigned char BufferEcho = 0; /* Stock les 16 dernières données reçus sur la broche echo*/
unsigned char indexTime = 0;
unsigned int finReception = 0; /*Booléen de synchronisation pour lancer le calcul de la distance*/
unsigned int nbrUn = 0; /*Nombre de période Te pour lesquelles le signal Echo est à 1*/

void configSysClock();
void ConfigTimer();
void InitPortIO(1, config Config);
unsigned char readPortIO(unsigned char num, unsigned char mask);
void writePortIO(unsigned char mask);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	configSysClock();
	ConfigTimer();
	while(1)
	return 0;
}

#pragma vector = TIMERA0_VECTOR; //Routine d'interruption activée par le timer A à une fréquence de 16KHz
__interrupt void ultraSoundmgt(void)
{
	++indexTime;
	if (indexTime <= TGEN)
	{
		/* pendant l'intervalle de temps Tgen : génération sur la broche Trig du module ultrasons (patte 1.4)*/
		//writePortIO(0xFF, BIT4);
	}
	else if (indexTime <= TRECEPT + TGEN)
	{
		//writePortIO(0x00, BIT4);
		/* pendant l'intervalle de temps Trecept : lecture de la valeur de la broche Echo et insertion dans le bufferEcho */
		bufferEcho = bufferEcho << 1;
		//bufferEcho += readPortIO(BIT0); //sur la patte 1.0
		++BufferCompt;
	}
	else
	{
		indexTime = 0;
	}
}

void configSysClock() // Configuration des horloges internes
{
	DCOCTL = CALDCO_16MHZ;
	BCSCTL1 = CALBC1_16MHZ;
	_BIS_SR(GIE); // Enable Interrupt
}

void ConfigTimer()
{
	/*Le signal sera généré sur la pate 1.3*/    //A VERIFIER
	P1DIR |= BIT3;
	P1SEL |= BIT3;

	P2DIR |= BIT3;
	P2SEL |= BIT3;
	P1OUT |= 0xFF;

	TACTL |= TACLR;

	TACTL |= TASSEL_2 | ID_0 | MC_1; // paramètre de l'horloge
	TACCR0 = 500; //Permet de diviser notre fréquence. On obtient une fréquence de 16kHz
	TACCR1 = 500; //Permet de diviser notre fréquence. On obtient une fréquence de 16kHz
	TACCTL1 |= OUTMOD_4; /*sélection du mode toggle*/


}

/*On doit faire une fonction pour initialiser les port IO*/
typedef struct{
	char output;
	char input;
	char IE;
	char IES;
	char RE;
	char pullup1down0;
} config;

void InitPortIO(1, config Config)
{
	unsigned char mask = Config.output | Config.input; 
	/*Initialisation du port 1*/
	P1DIR |= Config.output; 
	P1DIR &= ~Config.output;

	P1IE &= ~mask;
	P1IE |= (Config.IE&mask);

	P1IES &= ~mask;
	P1IES |= (Config.IES&mask);

	P1REN &= ~mask;
	P1REN |= (Config.RE&mask);

	P1SEL &= ~mask;

	P1OUT &= ~Config.input;
	P1OUT |= Config.pullup1down0 & Config.input;


}
/*lire sur les port IO*/
unsigned char readPortIO(unsigned char mask)
{
	unsigned char valPortIO; 
	/*lire sur port 1*/
	valPortIO = P1IN & mask; 
	return valPortIO; 

}

/*ecrire sur les port IO*/
void writePortIO(unsigned char val, unsigned char mask)
{
	/*écrire sur port 1*/
	P1OUT |= (val & mask);
	P1OUT &= val | ~mask ; 
}
