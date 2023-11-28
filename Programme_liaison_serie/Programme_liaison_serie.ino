#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

// Définition des bits d'erreur
#define FRAMING_ERROR (1 << FE0)
#define PARITY_ERROR (1 << UPE0)
#define DATA_OVERRUN (1 << DOR0)

char receivedByte ;

void USART_Init() {
  // Configuration du registre UBRR
  UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
  UBRR0L = (uint8_t)UBRR_VALUE;

  UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void USART_Transmit(char data) {
  // Attente que le tampon de transmission soit prêt
  while (!(UCSR0A & (1 << UDRE0)));

  // Chargement des données dans le tampon de transmission
  UDR0 = data;
}

void USART_puts(unsigned char *str) {
  do {
    USART_Transmit(*str);
  } while (*++str != 0);
}

void USART_putsln(unsigned char *str) {
  USART_puts(str);
  USART_Transmit('\n');
}


// Fonction d'interruption appelée lorsqu'un caractère est reçu
ISR(USART_RX_vect) {
  char status = UCSR0A;
  char receivedByte = UDR0;

  // Vérification des bits d'erreur
  if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN)) == 0) {
    USART_Transmit(receivedByte +1);
  }
  else
  {
    USART_putsln("ERREUR");
  }
}

int main(void) {
  // Initialisation de la liaison série à 9600 bauds
  USART_Init();
  sei();  // Activer les interruptions globales


  while (1) {
    //USART_putsln("IUT DE Troyes");
    //USART_Transmit(receivedByte +1);


    // Attendre pendant 1 seconde
    _delay_ms(1000);
  }

  return 0;
}