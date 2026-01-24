#ifndef BLE_H
#define BLE_H

void Bluetooth_Init(void);
uint8_t Get_Heart_Rate(void);
void BLE_UART_RxCpltCallback(UART_HandleTypeDef *huart);
#endif
