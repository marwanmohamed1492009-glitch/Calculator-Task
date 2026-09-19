#include <avr/io.h>
#include <UART-Driver.h>
#include <BIT_MATH.h>

  void USART_voidInit(uint32_t BaudRate , uint8_t Data , uint8_t Parity , uint8_t Stop)
    {
        uint16_t BRR_Value = (uint16_t)((F_CPU / (16UL * BaudRate))-1);

         UBRR0H = (uint8_t)(BRR_Value >> 8);
        UBRR0L = (uint8_t)(BRR_Value);

        SET_BIT(UCSR0B ,TXEN0 );
         SET_BIT(UCSR0B ,RXEN0 );

         SET_BIT(UCSR0C , UCSZ01);
         SET_BIT(UCSR0C , UCSZ00);
       
    }



    void USART_voidSendChar(uint8_t Copy_u8Data)
    {
        while (GET_BIT(UCSR0A,UDRE0)==0)
        
            UDR0 = Copy_u8Data;
    }


    uint8_t USART_u8ReceiveChar(void)
    {
        while (UCSR0A,RXC0)
        
           return UDR0;
        
    }

    USART_voidSendString(const uint8_t *Copy_pu8String)
    {
        uint16_t Index = 0 ;
        while (Copy_pu8String[Index] != '\0')
        {
            USART_voidSendChar(Copy_pu8String[Index]);
        }
        
    }


    void USART_voidReceiveString(uint8_t *Copy_pu8Buffer, uint16_t Copy_u16BufferSize) {

        uint16_t Index;

        for (Index = 0 ; Index < (Copy_u16BufferSize - 1) ; Index++ )
        {
            Copy_pu8Buffer[Index] = USART_u8ReceiveChar();
            if(Copy_pu8Buffer[Index] == '\n'|| Copy_pu8Buffer[Index] == '\r')
            {
                break;
            }
        }

          Copy_pu8Buffer[Index] = '\n';
    }


