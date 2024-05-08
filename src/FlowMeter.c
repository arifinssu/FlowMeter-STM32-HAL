#include "FlowMeter.h"

#include <math.h>
#define min(a,b) (((a) < (b)) ? (a) : (b))

FlowMeter_TypeDef FlowMeter;

void EXTI4_IRQHandler(void) {
    if (__HAL_GPIO_EXTI_GET_IT(FlowMeter.pin) != RESET) {
    	__HAL_GPIO_EXTI_CLEAR_IT(FlowMeter.pin);
    	FlowMeter_Count(&FlowMeter);
    }
}

FlowMeter_TypeDef FlowMeter_Init(GPIO_TypeDef *gpio, uint16_t pin, FlowSensorProperties prop) {
    FlowMeter_TypeDef flowMeter;
    flowMeter.gpio = gpio;
    flowMeter.pin = pin;
    flowMeter.properties = prop;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = flowMeter.pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(flowMeter.gpio, &GPIO_InitStruct);
    HAL_NVIC_SetPriority(IRQ, 2, 0);
    HAL_NVIC_EnableIRQ(IRQ);
    FlowMeter_Reset(&flowMeter);
    return flowMeter;
}

double FlowMeter_GetCurrentFlowrate(FlowMeter_TypeDef* flowMeter) {
    return round(flowMeter->currentFlowrate * 1000) / 1000.0;
}

double FlowMeter_GetCurrentVolume(FlowMeter_TypeDef* flowMeter) {
    return round(flowMeter->currentVolume * 1000) / 1000.0;
}

double FlowMeter_GetTotalFlowrate(FlowMeter_TypeDef* flowMeter) {
    return flowMeter->totalVolume / (flowMeter->totalDuration / 1000.0f) * 60.0f;
}

double FlowMeter_GetTotalVolume(FlowMeter_TypeDef* flowMeter) {
    return round(flowMeter->totalVolume * 1000) / 1000.0;
}

void FlowMeter_Tick(FlowMeter_TypeDef* flowMeter, uint32_t duration) {
    if((HAL_GetTick() - flowMeter->ticks) > duration) {
        flowMeter->ticks = HAL_GetTick();
        // disableInterrupt;
        volatile uint32_t pulses = flowMeter->currentPulses;
        flowMeter->currentPulses = 0;
        // enableInterrupt;
        double seconds = duration / 1000.0f;
        double frequency = pulses / seconds;
        uint8_t decile = floor(10.0f * frequency / (flowMeter->properties.capacity * flowMeter->properties.kFactor));
        uint8_t ceiling = 9;
        flowMeter->currentCorrection = flowMeter->properties.kFactor / flowMeter->properties.mFactor[min(decile, ceiling)];
        flowMeter->currentFlowrate = frequency / flowMeter->currentCorrection;
        flowMeter->currentVolume = flowMeter->currentFlowrate / (60.0f/seconds);
        flowMeter->currentDuration = duration;
        flowMeter->currentFrequency = frequency;
        flowMeter->totalDuration += duration;
        flowMeter->totalVolume += flowMeter->currentVolume;
        flowMeter->totalCorrection += flowMeter->currentCorrection * duration;
    }
}

void FlowMeter_Count(FlowMeter_TypeDef* flowMeter) {
    flowMeter->currentPulses++;
}

void FlowMeter_Reset(FlowMeter_TypeDef* flowMeter) {
    // disableInterrupt;
    flowMeter->currentPulses = 0;
    // enableInterrupt;
    flowMeter->currentFrequency = 0.0f;
    flowMeter->currentDuration = 0.0f;
    flowMeter->currentFlowrate = 0.0f;
    flowMeter->currentVolume = 0.0f;
    flowMeter->currentCorrection = 0.0f;
    flowMeter->totalVolume = 0.0f;
}

uint16_t FlowMeter_GetPin(FlowMeter_TypeDef* flowMeter) {
    return flowMeter->pin;
}

uint32_t FlowMeter_GetCurrentDuration(FlowMeter_TypeDef* flowMeter) {
    return flowMeter->currentDuration;
}

double FlowMeter_GetCurrentFrequency(FlowMeter_TypeDef* flowMeter) {
    return flowMeter->currentFrequency;
}

double FlowMeter_GetCurrentError(FlowMeter_TypeDef* flowMeter) {
    return (flowMeter->properties.kFactor / flowMeter->currentCorrection - 1) * 100;
}

uint32_t FlowMeter_GetTotalDuration(FlowMeter_TypeDef* flowMeter) {
    return flowMeter->totalDuration;
}

double FlowMeter_GetTotalError(FlowMeter_TypeDef* flowMeter) {
    return (flowMeter->properties.kFactor / flowMeter->totalCorrection * flowMeter->totalDuration - 1) * 100;
}

void FlowMeter_SetTotalDuration(FlowMeter_TypeDef* flowMeter, uint32_t totalDuration) {
    flowMeter->totalDuration = totalDuration;
}

void FlowMeter_SetTotalVolume(FlowMeter_TypeDef* flowMeter, double totalVolume) {
    flowMeter->totalVolume = totalVolume;
}

void FlowMeter_SetTotalCorrection(FlowMeter_TypeDef* flowMeter, double totalCorrection) {
    flowMeter->totalCorrection = totalCorrection;
}

FlowSensorProperties UncalibratedSensor = {60.0f, 5.0f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
FlowSensorProperties FS300A = {60.0f, 5.5f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
FlowSensorProperties FS400A = {60.0f, 4.8f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
FlowSensorProperties OF05ZAT = {30.0f, 2.73f, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};
