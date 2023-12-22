/* This file has been prepared for Doxygen automatic documentation generation.*/

/*! \file *********************************************************************
 *
 * \brief
 *      communication GBF
 *
 * \par Note: A partir de la Datasheet ATmega328P
 *
 * \par Documentation: https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
 *
 * \author
 *      Github: https://github.com/Dorian-Chauvet2002
 *      Email: dorian.chauvet1@etudiant.univ-reims.fr
 *
 * $Name: Liaison série $
 * $RCSfile: main.c,v $
 * $Date: 2023/11/21 10:43:51 $  \n
 ******************************************************************************/


#define FOSC 16000000  // Fréquence du quartz (Clock Speed)
#define BAUD 9600
#define MYUBRR FOSC / 16 / BAUD - 1
#define FRAMING_ERROR (1 << FE0)
#define PARITY_ERROR (1 << UPE0)
#define DATA_OVERRUN (1 << DOR0)

int flag;  // Variable pour stocker la donnée reçue
char byteReceive;

int main(void) {
  USART_Init(MYUBRR);

  sei();

  while (1) {
    // Votre code principal ici

    _delay_ms(1);
  }
}

void USART_Init(unsigned int ubrr) {
  // Initialisation de l'USART 0 (UART sur la carte Arduino Mega)
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);  // Activation du récepteur, de l'émetteur et de l'interruption de réception
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // Format de trame: 8 données, 1 bit d'arrêt

  // Initialisation de l'USART 1
  UBRR1H = (unsigned char)(ubrr >> 8);
  UBRR1L = (unsigned char)ubrr;
  UCSR1B = (1 << RXEN1) | (1 << TXEN1) | (1 << RXCIE1);  // Activation du récepteur, de l'émetteur et de l'interruption de réception
  UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);  // Format de trame: 8 données, 1 bit d'arrêt

  // Initialisation de l'USART 3
  UBRR3H = (unsigned char)(ubrr >> 8);
  UBRR3L = (unsigned char)ubrr;
  UCSR3B = (1 << RXEN3) | (1 << TXEN3) | (1 << RXCIE3);  // Activation du récepteur, de l'émetteur et de l'interruption de réception
  UCSR3C = (1 << UCSZ31) | (1 << UCSZ30);  // Format de trame: 8 données, 1 bit d'arrêt
}

void USART_Transmit0(unsigned char data) {
  // Attente du buffer de transmission vide
  while (!(UCSR0A & (1 << UDRE0)))    ;
  // Placement des données dans le tampon, envoi des données
  UDR0 = data;
}

void USART_Transmit1(unsigned char data) {
  // Attente du buffer de transmission vide
  while (!(UCSR1A & (1 << UDRE1)))    ;
  // Placement des données dans le tampon, envoi des données
  UDR1 = data;
}

unsigned char USART_Receive0(void) {
  // Attente de la réception de données
  while (!(UCSR0A & (1 << RXC0)))    ;
  // Récupération et retour des données reçues depuis le tampon
  return UDR0;
}



ISR(USART0_RX_vect) {
  // Lecture de la donnée reçue
  char status = UCSR0A;
  byteReceive = UDR0;
  if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN)) == 0) {
    // Traitement des données reçues (ajout de 1 et renvoi)
    USART_Transmit1(byteReceive + 1);
  } else {
    // Gestion des erreurs
   // USART_putsln("Error");
  }
}

ISR(USART1_RX_vect) {
  // Lecture de la donnée reçue
  char status = UCSR1A;
  byteReceive = UDR1;
  if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN)) == 0) {
    // Traitement des données reçues (ajout de 1 et renvoi)
    USART_Transmit0(byteReceive + 1);
  } else {
    // Gestion des erreurs
   // USART_putsln("Error");
  }
}
/*
void USART_putsln(unsigned char *str) {
  // Envoi de la chaîne de caractères suivie d'un saut de ligne
  USART_puts(str);
  USART_Transmit1('\n');
  USART_Transmit1('\r');
}

void USART_puts(unsigned char *str) {
  // Envoi de la chaîne de caractères
  do {
    USART_Transmit1(*str);
  } while (*++str != 0);
}*/
