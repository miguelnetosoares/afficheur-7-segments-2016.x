#include "test.h"
#include <stdio.h>

/** Les digits de l'afficheur. */
static unsigned char digits[2];

/** Conversion BCD*/
static char ascii7Segments[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void afficheurEtablitDigits(char nombre) {
    // À implémenter...
    
    char d1, d2;
    
    if(nombre>99){          // Test si la valeur dépace 99
        digits [0]=9;
        digits [1]=9;
    }
    else {     
        d1 = nombre / 10;   // dizaine
        d2 = nombre % 10;   // unité
        digits[0] = d1;     // dizaine
        digits[1] = d2;     // unité
    }
}

unsigned char digit(unsigned char position) {
    // À implémenter...
    // code ascii selon tableau conversion BCD
    
    
    if (position == 0 && digits[0]==0){         //retourne 0 sur le digit des dizaines
        return (0);
    }
    else if (position == 0){                    //retourne un code ascii sur le digit des dizaines
        return (ascii7Segments[digits[0]]);
    }
   else {
        return (ascii7Segments[digits[1]]);     //retourne un code ascii sur le digit des unitées
    }
}


#ifdef TEST

int testAffichePasDeZeroAGauche() {
    afficheurEtablitDigits(0);
    testeEgaliteEntiers("AZG00a", digit(1), 0x3F);
    testeEgaliteEntiers("AZG00b", digit(0), 0);

    afficheurEtablitDigits(9);
    testeEgaliteEntiers("AZG09a", digit(1), 0x6F);
    testeEgaliteEntiers("AZG09b", digit(0), 0);
}

int testAfficheDe10A99() {
    afficheurEtablitDigits(10);
    testeEgaliteEntiers("AFF10a", digit(1), 0x3F);
    testeEgaliteEntiers("AFF10b", digit(0), 0x06);

    afficheurEtablitDigits(99);
    testeEgaliteEntiers("AFF99a", digit(1), 0x6F);
    testeEgaliteEntiers("AFF99b", digit(0), 0x6F);
}

int testAfficheAuMaximum99() {
    afficheurEtablitDigits(100);
    testeEgaliteEntiers("AMX100a", digit(1), 0x6F);
    testeEgaliteEntiers("AMX100b", digit(0), 0x6F);

    afficheurEtablitDigits(255);
    testeEgaliteEntiers("AMX255a", digit(1), 0x6F);
    testeEgaliteEntiers("AMX255b", digit(0), 0x6F);
}

void testAfficheur() {
    testAffichePasDeZeroAGauche();
    testAfficheDe10A99();
    testAfficheAuMaximum99();
}

#endif
