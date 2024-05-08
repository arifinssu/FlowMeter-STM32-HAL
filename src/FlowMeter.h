#ifndef FLOWMETER_H
#define FLOWMETER_H

// setup STM32 board HAL & IRQ pin
#include "stm32f4xx_hal.h"
#define IRQ EXTI4_IRQn

#include <math.h>
#define disableInterrupt NVIC_DisableIRQ(IRQ);
#define enableInterrupt NVIC_EnableIRQ(IRQ);

typedef struct {
    float capacity;
    float kFactor;
    float mFactor[10];
} FlowSensorProperties;

typedef struct {
    GPIO_TypeDef            *gpio;
    uint16_t                pin;
    FlowSensorProperties    properties;
    void                    (*interruptCallback)(void);
    uint8_t                 interruptMode;
    volatile uint32_t       currentPulses;
    double                  currentFrequency;
    uint32_t                currentDuration;
    double                  currentFlowrate;
    double                  currentVolume;
    double                  currentCorrection;
    uint32_t                totalDuration;
    double                  totalVolume;
    double                  totalCorrection;
    uint32_t                ticks;
} FlowMeter_TypeDef;

FlowMeter_TypeDef FlowMeter_Init(GPIO_TypeDef *gpio, uint16_t pin, FlowSensorProperties prop);
double FlowMeter_GetCurrentFlowrate(FlowMeter_TypeDef* flowMeter);
double FlowMeter_GetCurrentVolume(FlowMeter_TypeDef* flowMeter);
double FlowMeter_GetTotalFlowrate(FlowMeter_TypeDef* flowMeter);
double FlowMeter_GetTotalVolume(FlowMeter_TypeDef* flowMeter);
void FlowMeter_Tick(FlowMeter_TypeDef* flowMeter, uint32_t duration);
void FlowMeter_Count(FlowMeter_TypeDef* flowMeter);
void FlowMeter_Reset(FlowMeter_TypeDef* flowMeter);
uint16_t FlowMeter_GetPin(FlowMeter_TypeDef* flowMeter);
uint32_t FlowMeter_GetCurrentDuration(FlowMeter_TypeDef* flowMeter);
double FlowMeter_GetCurrentFrequency(FlowMeter_TypeDef* flowMeter);
double FlowMeter_GetCurrentError(FlowMeter_TypeDef* flowMeter);
uint32_t FlowMeter_GetTotalDuration(FlowMeter_TypeDef* flowMeter);
double FlowMeter_GetTotalError(FlowMeter_TypeDef* flowMeter);
void FlowMeter_SetTotalDuration(FlowMeter_TypeDef* flowMeter, uint32_t totalDuration);
void FlowMeter_SetTotalVolume(FlowMeter_TypeDef* flowMeter, double totalVolume);
void FlowMeter_SetTotalCorrection(FlowMeter_TypeDef* flowMeter, double totalCorrection);

extern FlowSensorProperties UncalibratedSensor;
extern FlowSensorProperties FS300A;
extern FlowSensorProperties FS400A;
extern FlowMeter_TypeDef FlowMeter;

#endif /* FLOWMETER_H */
