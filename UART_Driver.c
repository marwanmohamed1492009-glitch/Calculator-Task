#define F_CPU 16000000UL
#include "UART-Driver.h"
#include <stddef.h>
#include <avr/io.h>
#include "BIT_MATH.h"

void USART_voidInit(uint32_t Copy_u32BaudRate, uint8_t Copy_u8DataBits, uint8_t Copy_u8Parity, uint8_t Copy_u8StopBits) {
    uint16_t local_u16UBRRValue = (uint16_t)((F_CPU / (16UL * Copy_u32BaudRate)) - 1);

    /* 1. ضبط الـ Baud Rate */
    UBRR0H = (uint8_t)(local_u16UBRRValue >> 8);
    UBRR0L = (uint8_t)(local_u16UBRRValue);

    /* 2. تفعيل الاستقبال والإرسال */
    SET_BIT(UCSR0B, RXEN0);
    SET_BIT(UCSR0B, TXEN0);

    /* 3. تصفير الـ Register أولاً لضمان حالة نظيفة */
    UCSR0C = 0;

    /* 4. ضبط الـ Parity (بيتشفت بـ 0 أو 2 أو 3 على بت UPM00) */
    UCSR0C |= (uint8_t)(Copy_u8Parity << UPM00);

    /* 5. ضبط الـ Stop Bit (بيتشفت بـ 0 أو 1 على بت USBS0) */
    UCSR0C |= (uint8_t)(Copy_u8StopBits << USBS0);

    /* 6. ضبط الـ Data Bits مباشرة بالقيمة المحددة (0 أو 1 أو 2 أو 3) */
    UCSR0C |= (uint8_t)(Copy_u8DataBits << UCSZ00);
}

void USART_voidSendChar(uint8_t Copy_u8Data) {
    while ((UCSR0A & (1U << UDRE0)) == 0U);

    UDR0 = Copy_u8Data;
}

uint8_t USART_u8ReceiveChar(void) {
    while ((UCSR0A & (1U << RXC0)) == 0U);
    return UDR0;
}

void USART_voidSendString(const uint8_t *Copy_pu8String) {
    uint16_t local_u16Index = 0;

    while (Copy_pu8String[local_u16Index] != '\0') {
        USART_voidSendChar(Copy_pu8String[local_u16Index++]);
    } 
}
void USART_voidReceiveString(uint8_t *Copy_pu8Buffer, uint16_t Copy_u16BufferSize) {
    uint16_t local_u16Index;

    for (local_u16Index = 0; local_u16Index < Copy_u16BufferSize; local_u16Index++) {
        /* نفس فكرة الـ Send ولكن بالاستقبال حرف ورا حرف */
        Copy_pu8Buffer[local_u16Index] = USART_u8ReceiveChar();
    }

    /* إنهاء النص بـ Null عشان يتعامل كـ String سليم */
    Copy_pu8Buffer[local_u16Index] = '\0';
}
