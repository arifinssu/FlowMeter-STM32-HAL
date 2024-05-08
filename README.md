# FlowMeter-STM32-HAL Library

*Special thanks to Sebastian Kraus for his amazing work in https://github.com/sekdiy/FlowMeter*

**FlowMeter-STM32-HAL** is a FlowMeter library that specialized for STM32Cube based board and firmware. FlowMeter library provides calibrated flow and volume measurement with flow sensors.

You can use it to count flow and volume of liquids and gases (although the documentation focuses on applications using liquids) and can support multiple flow sensors at the same time.

It also works as a totalizer, accumulating total volume and average flow rate over the run time of your project.

A provision for calibration helps you to get the most out of your sensor. You can even estimate the recent and overall error margin.

The library is intended for use with flow sensors that provide a frequency output signal proportional to flow, although other types of sensors could be made to work.

## First Setup

To use this library, you must setup the board and IRQ pin in `src/FlowMeter.h`. Take a look in line 4 to 6 in this header: 
```c
// setup STM32 board HAL & IRQ pin
#include "stm32f4xx_hal.h"
#define IRQ EXTI4_IRQn
```

## Getting Started

Getting started with **FlowMeter** is easy. Take a look at this simple example:

```c
#include "FlowMeter.h"

int main(void) {
	FlowMeter = FlowMeter_Init(GPIOE, GPIO_PIN_4, UncalibratedSensor);
	while(1) {
		// tick flowmeter every 1 seconds
		FlowMeter_Tick(&flow, 1000);
		
		// get flowmeter result
		float current_flowrate = FlowMeter_GetCurrentFlowrate(&FlowMeter);
		float current_volume = FlowMeter_GetCurrentVolume(&FlowMeter);
		float total_volume = FlowMeter_GetTotalVolume(&FlowMeter);
	}
}
```

## Unit of measure

The **FlowMeter** library expresses flow in the unit **l/min**.
Most units of measure can be derived by simple conversion (just try not to measure in [Sverdrups](https://en.wikipedia.org/wiki/Sverdrup)).

As an example, conversion between **l/min** and US **gal/min** can be done with a factor of *3.78541178*, conversion from **min**  to **s** with a factor of *60*.

```math
3.78541178 l/min ≈ 1 gal/min ≈ 0.0167 gal/s.
```

Please make sure you consult the [documentation](https://github.com/sekdiy/FlowMeter/wiki/Properties) in order to further understand how the library works.

## Should you calibrate your own sensor?

The **FlowMeter** library can be used with many different flow sensors right away. Some [sensor examples](https://github.com/sekdiy/FlowMeter/wiki/Sensors) are listed in the documentation.

For many projects you don't need to worry about calibration. But it still makes sense to be aware of the limitations that come with an uncalibrated sensor in a metering application.

It's easy to calibrate yourself. Preferrably you'd do this after installing the sensor into your project. The flow meter then benefits from increased precision within the viscosity and flow range of your application.

There's a [complete how-to](https://github.com/sekdiy/FlowMeter/wiki/Calibration) in the documentation.

![Calibration Example: Irrigation with FS400A](https://github.com/sekdiy/FlowMeter/wiki/images/FS400A-calibration.jpg)

## Documentation

For further details please take a look at the **FlowMeter** [documentation pages](https://github.com/sekdiy/FlowMeter/wiki).