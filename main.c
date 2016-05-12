#include <xc.h>
#include "test.h"
#include "afficheur.h"

/**
 * Bits de configuration:
 */
#pragma config FOSC = INTIO67  // Osc. interne, A6 et A7 comme IO.
#pragma config IESO = OFF      // Pas d'osc. au démarrage.
#pragma config FCMEN = OFF     // Pas de monitorage de l'oscillateur.

// Nécessaires pour ICSP / ICD:
#pragma config MCLRE = EXTMCLR // RE3 est actif comme master reset.
#pragma config WDTEN = OFF     // Watchdog inactif.
#pragma config LVP = OFF       // Single Supply Enable bits off.

#ifndef TEST

void low_priority interrupt bassePriorite() {
    static char n = 0;
    if (PIR1bits.TMR1IF) {
        TMR1 = 60535;       
        PIR1bits.TMR1IF = 0;
        PORTA = digit(n);
        switch(n) {
            case 0:
                PORTCbits.RC1 = 1;
                PORTCbits.RC0 = 0;
                n = 1;
                break;
            case 1:
                PORTCbits.RC1 = 0;
                PORTCbits.RC0 = 1;
                n = 0;
                break;
        }
        ADCON0bits.GO = 1;
    }
    
    if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;
        afficheurEtablitDigits(ADRESH);
    }
}

/**
 * Initialise les périphériques.
 */
void initialisationHardware() {
    
    // Configure le module A/D:
    TRISBbits.RB4 = 1;      // Active RB4 comme entrée.
    ANSELBbits.ANSB4 = 1;   // Active AN11 comme entrée analogique.
    ADCON0bits.ADON = 1;    // Allume le module A/D.
    ADCON0bits.CHS = 11;    // Branche le convertisseur sur AN11
    ADCON2bits.ADFM = 0;    // Les 8 bits plus signifiants sur ADRESH.
    ADCON2bits.ACQT = 3;    // Temps d'acquisition à 6 TAD.
    ADCON2bits.ADCS = 0;    // À 1MHz, le TAD est à 2us.

    PIE1bits.ADIE = 1;      // Active les interr. A/D
    IPR1bits.ADIP = 0;      // Interr. A/D sont de basse priorité.

    // Active les sorties digitales:
    ANSELA = 0;
    ANSELC = 0;
    TRISA  = 0x00;
    TRISC &= 0b11111100;
            
    // Temporisateur 1 (T = 20ms avec FOSC = 1MHz: TMR1 = 10^6*20*10^-3/4 = 5000
    T1CONbits.TMR1CS = 0;       // Source: FOSC / 4
    T1CONbits.T1CKPS = 0;       // Pas de diviseur de fréquence.
    T1CONbits.T1RD16 = 1;       // Temporisateur de 16 bits.
    T1CONbits.TMR1ON = 1;       // Active le temporisateur 1    
    
    PIE1bits.TMR1IE = 1;        // Active les interruptions du T1 ...
    IPR1bits.TMR1IP = 0;        // ... de basse priorité
    
    // Active les interruptions de haute et de basse priorité:
    RCONbits.IPEN = 1;
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}

/**
 * Point d'entrée du programme.
 */
void main(void) {
    initialisationHardware();
    afficheurEtablitDigits(77);
    while(1);
}
#endif

#ifdef TEST

/**
 * Point d'entrée des tests unitaires.
 */
void main() {
    
    initialiseTests();
    
    testAfficheur();
    
    finaliseTests();
    
    while(1);
}
#endif
