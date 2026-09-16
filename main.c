#include <stdio.h>
#include <stdlib.h>
#include "UART-Driver.h"

int main(void)
{
    /* تهيئة الـ UART على 9600 Baud, 8 Data Bits, No Parity, 1 Stop Bit */
    USART_voidInit(9600, USART_DATA_FRAME_8BITS, USART_NO_PARITY, USART_ONE_STOP_BIT);

    uint8_t buffer[20];
    int32_t num1 = 0, num2 = 0, result = 0;
    uint8_t op = 0;

    USART_voidSendString((const uint8_t *)"\r\n=== Bare-Metal UART Calculator ===\r\n");

    while (1)
    {
        /* 1. استقبال الرقم الأول */
        USART_voidSendString((const uint8_t *)"\r\nEnter First Number: ");
        USART_voidReceiveString(buffer, sizeof(buffer));
        num1 = atol((const char *)buffer);

        /* 2. استقبال العملية الحسابية (+, -, *, /) */
        USART_voidSendString((const uint8_t *)"\r\nEnter Operator (+, -, *, /): ");
        op = USART_u8ReceiveChar();
        USART_voidSendChar(op); // Echo للعلامة

        /* 3. استقبال الرقم الثاني */
        USART_voidSendString((const uint8_t *)"\r\nEnter Second Number: ");
        USART_voidReceiveString(buffer, sizeof(buffer));
        num2 = atol((const char *)buffer);

        /* 4. حساب النتيجة */
        uint8_t valid = 1;
        switch (op)
        {
            case '+': result = num1 + num2; break;
            case '-': result = num1 - num2; break;
            case '*': result = num1 * num2; break;
            case '/':
                if (num2 != 0)
                {
                    result = num1 / num2;
                }
                else
                {
                    USART_voidSendString((const uint8_t *)"\r\nError: Division by zero!");
                    valid = 0;
                }
                break;
            default:
                USART_voidSendString((const uint8_t *)"\r\nError: Invalid Operator!");
                valid = 0;
                break;
        }

        /* 5. طباعة النتيجة */
        if (valid)
        {
            sprintf((char *)buffer, "\r\nResult: %ld\r\n", result);
            USART_voidSendString(buffer);
        }

        USART_voidSendString((const uint8_t *)"---------------------------------");
    }

    return 0;
}
