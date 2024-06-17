----!
Presentation
----!
# Compile Code with optimization
<p> </p>
To reduce time spent in Run mode (ISR) compile code for speed.
<p> </p>

Go to `Project - Properties - C/C++ Build - Settings - MCU GCC Compiler - Optimization`

- select `Optimize for speed (-Ofast)`
- click `Apply and Close`

<p> </p>

![image](./img/optimization.png)


# Compile Code
<p> </p>

![gif](./img/compile.gif)

# Debug configuration
<awarning> 
Disable Debug in Low Power modes. Otherwise MCU does no enter in genuine LP modes.
</awarning>

<p> </p>
- Debug configurations window is opened during first debug session or open it manually.
<p> </p>

![gif](./img/debugconf.gif)
<p> </p>

- **Disable Debug** in Low Power modes.
<p> </p>

![gif](./img/disdebug.gif)

# Flash device 
<p> </p>

![gif](./img/debug.gif)

# Terminate debug session
<p> </p>
<awarning> 
To avoid extra consumption.
</awarning>
<p> </p>

![gif](./img/terminate.gif)
  