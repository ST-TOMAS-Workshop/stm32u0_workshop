----!
Presentation
----!
# End device

- Supplied from BUS and continuously decoding stream on BUS
- Depends on timing requirements End device combining Sleep/StopX modes
- Need also provide asnwers back to power line
- Pulse can be very distorted
  
## In a Workshop

- Use **STM32U83-DK** board
- Device is not supplied from BUS (it would require extra power supply)
- Measure incoming pulses by LPTIM with external LSE(quartz) crystal
- Re-shape edges by internal **Comparator**
- Between edges and during Idle state on BUS the device enter in STOP1 mode
- Perform simple data process
- Duty cycle of low pulse is displayed on **Glass LCD**
- Manage TimeOut with RTC unit when no TX activity on BUS
- Monitor consumption profile in **STM32CubeMonitor-Power**

![image](./img/EndDevice.png)
