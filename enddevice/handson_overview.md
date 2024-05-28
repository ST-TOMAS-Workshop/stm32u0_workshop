# Wired BUS Architecture
Let imagine simple BUS architecture one Master Panel and multiple End devices.

- Two wires BUS 
    - DC power supply + communication
    - VSS
- Data stream are modulated as Edges in power line
- Defined Low pulse widths for 1/0 bit in range hundred of microseconds to few miliseconds
- Proprietary protocol
    - typically start bit + data + stop bits

![image](./img/MasterBusEnd.png)

![image](./img/stream.png)

