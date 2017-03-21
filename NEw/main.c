#include <msp430.h> 

/*
 * AUTEURS : CHAUVEL Benjamin, DUPONT Corentin
 * DATE : 21 mars 2017
 * PROJET : Interface capteur � Ultrasons
 */
#define TGEN		/*VALEUR A DEFINIR 2 */
#define TRECEPT		/*VALEUR A DEFINIR  */
#define TE   /*    */
#define BUFFER 16   /*Taille allou�e au stockage */

unsigned char BufferCompt = 0; /*Compte le nombre de valeur mit dans le BUFFER*/
unsigned char BufferEcho = 0; /* Stock les 16 derni�res donn�es re�us sur la broche echo*/
unsigned char indexTime = 0;
unsigned int finReception = 0; /*Bool�en de synchronisation pour lancer le calcul de la distance*/
unsigned int nbrUn = 0; /*Nombre de p�riode Te pour lesquelles le signal Echo est � 1*/

void configSysClock();
void writeP1(unsigned char val, unsigned char mask);
/*NE PAS OUBLIER D'INSERER LES FONCTIONS ECRIRE LECTURE INITPORT QUE L'ON VA CREER EN BAS*/

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	configSysClock();
	ConfigTimer();
	while(1)
	return 0;
}

#pragma vector = TIMERA0_VECTOR; //Routine d'interruption activ�e par le timer A � une fr�quence de 16KHz
__interrupt void ultraSoundmgt(void)
{
	++indexTime;
	if (indexTime <= TGEN)
	{
		/* pendant l'intervalle de temps Tgen : g�n�ration sur la broche Trig du module ultrasons (patte 1.4)*/
		//writePortIO(1, 0xFF, BIT4);
	}
	else if (indexTime <= TRECEPT + TGEN)
	{
		//writePortIO(1, 0x00, BIT4);
		/* pendant l'intervalle de temps Trecept : lecture de la valeur de la broche Echo et insertion dans le bufferEcho */
		bufferEcho = bufferEcho << 1;
		//bufferEcho += readPortIO(1, BIT0); //sur la patte 1.0
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
	/*Le signal sera g�n�r� sur la pate 1.3*/    //A VERIFIER
	P1DIR |= BIT3;
	P1SEL |= BIT3;

	P2DIR |= BIT3;
	P2SEL |= BIT3;
	P1OUT |= 0xFF;

	TACTL |= TACLR;

	TACTL |= TASSEL_2 | ID_0 | MC_1; // param�tre de l'horloge
	TACCR0 = 500; //Permet de diviser notre fr�quence. On obtient une fr�quence de 16kHz
	TACCR1 = 500; //Permet de diviser notre fr�quence. On obtient une fr�quence de 16kHz
	TACCTL1 |= OUTMOD_4; /*s�lection du mode toggle*/


}
void writeP1(unsigned char val, unsigned char mask)
{
	P1OUT |= (val & mask);
	P1OUT &= val | ~mask;
}


/*On doit faire une fonction pour initialiser les port IO*/
/*On doit faire une fonction pour lire sur les port IO*/
/*On doit faire une fonction pour �crire sur les port IO*/