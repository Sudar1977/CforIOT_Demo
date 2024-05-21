int main(void)
{
    uint8_t MSG[35] = {'\0'};
    uint8_t X = 0;

    HAL_Init();
    SystemClock_Config();

    initGPIO();
    MX_USAT2_UART_Init();

    while(1)
    {
        HAL_Delay(1000);
        HAL_UART_Transmit(&huart2, (uint8_t *)"Hello, world!\r\n", 15U, 100U);
        sprintf(MSG, "Hello Students! Tracing X = %d\r\n", X);
        HAL_UART_Transmit(&huart2, MSG, sizeof(MSG), 100);
        X++;
    };

    return 0;
}
