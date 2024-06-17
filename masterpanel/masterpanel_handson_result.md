----!
Presentation
----!

# Test Functionality
Check functionality we develop in past 30 minutes!

- Supply Nucleo board
- After every press blue user button a Duty cycle of Low pulse is updated
- Visible change in **LED brightness**

## Measure consumption in LP Sleep
- Measured consumption **49 uA** for LP Sleep mode @1MHz and PWM operation @200Hz
- System Clock can be slow down for further reduction
- Measured results matches with DataSheet typical characteristics ~ 48 uA
    - LP Sleep: 38.5 uA/MHz
    - DMA + TIM2: 9 uA/MHz
    - GPIOs + BUS matrix: 0.55 uA/MHz
- Triangle wave due to Low Power regulator operation

![image](./img/sleep.png)