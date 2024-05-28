----!
Presentation
----!
# Power Supply
<ainfo>
STM32U5xxxxxx and STM32U5xxxxxx**Q**
</ainfo>
<p> </p>
**2 packages** available w/ or w/o SMPS

Internal voltage regulator: LDO and SMPS (requiring external 4.7 µF + 2.2 µH)
SMPS and LDO are in parallel and can be selected on the fly during application

- LDO: less noise 
- SMPS: better efficiency

All power modes are supported with SPMS and LDO:

- Exit from Stop and Standby modes is done in the selected regulator, always in range 4
- Exit from Shutdown and boot from reset is  always done on LDO 

SMPS and non-SMPS device are not Pin-to-Pin compatible 
- Not only in SMPS dedicated pins
- Pins on one side are shifted

<awarning>
SMPS is designed for supplying only internal load (cannot be used for supplying external components)
</awarning>
<p> </p>

## Internal Vcore voltage level
![image1](./img/Vcore.png) 

# Dynamic vs. Static operation
<ainfo>
BEST in ULP Class! Twice less dynamic consumption vs. L4/L4+ (3x less vs. L5)
</ainfo>
<p> </p>
STM32U5 is designed to perform dynamic scenario in **Autonomous mode(LPBAM)**

- Off load Core domain and thus reduce wakeup sequences
- Sensor data transfer, ADC acquisition, DAC generation,…

DataSheet values for LP modes are Static consumption only

- No tasks are performed

<awarning> 
Device selection must follow **Application demand** and NOT DataSheet comparison (U5 vs. L4/L4+/L5).
</awarning>
<p> </p>

## Current consumption profile periodical ADC acquisition and I2C trasmit in Stop mode
![image2](./img/ADC_consumption_profile.png) 

# System Architecture
Two digital domains CPU domain (CD) and Smart run domain (SRD)

CD & SRD contains full feature set

SRD contains only reduced peripheries (ADC4, DAC, UART, I2C ,SPI, UART, LPGPIO, SRAM4,…)

**In Run, Sleep, Stop 0 and Stop 1**

- CD & SRD fully powered => all peripherals are functional, thanks to GPDMA1 and LPDMA1

**In STOP2**

- CD in “retention” (lower leakage mode) => no dynamic activity possible
- SRD fully powered => SRD autonomous peripherals are functional thanks to LPDMA1.

## CPU domain (CD) and Smart run domain (SRD) block schema

![image3](./img/CD_SRD.png)

# Sleep mode
Core stops

High speed clocks run

Sleep Range 4 replaces STM32L4/L5 Low-power sleep mode

![image4](./img/sleep.png)

# Stop modes
<ainfo>
Lowest power mode with full retention and peripheral activity (LPBAM).
</ainfo>
<p> </p>
Core stops

High speed clocks runs only on peripheral’s demand

Full retention of SRAM and peripherals registers, with capability to individually **power down** SRAM pages in Stop 0,1,2,3:

- SRAM1 : 3 x 64KB-pages
- SRAM2 : 8KB and 56KB pages
- SRAM3 : 8 x 64KB-pages
- SRAM4
- ICACHE, DCACHE1, DMA2D SRAM, FMAC/FDRAM/USB SRAM, PKA SRAM

Wakeup clock is HSI16 or MSI up to 24 MHz (range 4 only)

Set ULPMEN to reduce consumption 

- BOR operates in Sampled mode 
- Caution: min. VDD falling slew rate must be respected

## Stop modes summary
![image](./img/stop.png)

Functional peripherals in Stop 2 mode – **SRD domain**
- LPDMA, SRAM4, LPUART1, SPI3, I2C3, LPTIM1/2/4, ADF1, DAC1, ADC1, COMP, VREFBUFF, RTC, LPGPIO

New **Low power GPIOs** control dynamically in down to Stop 2 mode thanks to LPDMA


# Stanby - Shutdown - Vbat
Backup domain active

- RTC, LSE, LSI (not in Shutdown)
- TAMP event (extra Supply and Temperature monitoring for TAMP)
- Backup Register
- CSS on LSE

**LSE clock security** system connected to tamper with down to Vbat mode

- Clock missing detection & Over-frequency detection (2MHz)
- Glitch filtering (2MHz)

**TAMP**

- Temperature and backup domain voltage monitoring connected to tamper (more details in TN1333 under NDA)
- A tamper detection can erase the backup registers, backup SRAM, SRAM2, caches and cryptographic peripherals.

**Vbat Brownout reset** (1.58V) ensures defined reset state for RTC domain when Vbat voltage drops below threshold (this is not the case for legacy device)

Extra **SRAM2 retention** in Standby

**2KB BKPSRAM** can be retained (optionally protected by tamper)

![image](./img/standby.png)

# Tips and tricks
Tips how to reduce power consumption in Run, Sleep, Stop modes.

![image](./img/tips.png)

# Clocks
Standard set of internal and external clock's sources

![image](./img/clock.png)

## New MSI (MSIK and MSIS)
**16 frequencies** from 100 kHz to 48 MHz

- The MSI is made of four RC oscillators
- Each MSIRCx feeds a prescaler providing a division by 1, 2, 3 or 4.
- 3.072MHz RC for audio frequencies
- Each MSIRCx has user trim code

Two output clocks are generated:

- **MSIS**, that can be selected as system clock
- **MSIK**, that can be selected by some peripherals as kernel clock. 

MSI-PLL mode can be automatically calibrated with LSE

![image](./img/msi.png)

# Autonomous peripherals clock request
The peripheral functionality is autonomous and is not impacted by the MCU power mode changes between Run, Sleep and Stop modes.

Depending on its need, the peripheral generates:

- **A kernel clock request**
- **A bus clock request AHBx / APBx**

The peripheral releases the request as soon as it does not need the clock anymore. 

**In low power mode like Stop modes**

- Peripherals request clock (kernel, AHB, APB)

  - Own request (data transfer, status register update)
  - Hardware asynchronously triggers to start IP and Clock request (I/Os events, LPTIM output triggers,comparators outputs…)


- Peripheral receive the requested clock after RC wakeup time

- The peripheral releases the request as soon as it does not need the clock anymore
  - Oscillator is automatically powered down

![image](./img/clock_request.png)

# Peripherals clock gating
To enable peripherals Autonomous operation new clock control schema is applied to U5 device

Legacy **xxEN** control bit for clock gating in Run mode

New **xxSMEN** for peripherals mapped in Core Domain (AHB1/APB1 and AHB2/APB2)

- Both xxEN and xxSMEN bits of the peripheral are set to keep clock on in Sleep mode and allow autonomous mode in Stop 0 and Stop 1
- xxSMEN is enabled by default after reset! Take care

Additional **xxAMEN** for peripherals mapped in Smart Run Domain (AHB3/APB3)

- Both xxEN and xxSMEN bits of the peripheral are set, plus the xxAMEN to enable the autonomous mode is enabled in Stop 0, Stop 1 and Stop 2

<awarning> 
If any periphery request clock -> Once HSI16/MSI is ready it’s propagated to all enabled peripherals configured by these control bits – must be well controlled by user to reach best power consumption
</awarning>
<p> </p>

![image](./img/gating.png)

# No more EXTI
<ainfo>
Almost all EXTI lines are not needed for peripheries to generate interrupt for wake up event.BEST in 
</ainfo>
<p> </p>

All event allow the possibility to wake up the system from Stop 0, 1, 2 modes. 

- Selectable active trigger edge 

**No more “direct” lines from peripherals**

- Peripherals with wakeup capability requests the AHB/APB clock
- When the system clock is present, the peripheral generates an interrupt if enabled
- The interrupt wakes-up the device, and is directly connected to NVIC

Individual secure and privilege configuration for each event

![image](./img/exti.png)
