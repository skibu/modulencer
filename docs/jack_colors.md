# Jack Colors

want each jack to have a RGB LED to indicate current input or output. not just the value of the voltage, but also 
what the voltage represents (gate, trigger, 1v/oct, etc). Since the modulencer will have many jacks, and 
most of the jacks can handle different signal types,  this could really 
help user keep track of what they are doing. 

## Proposed colors
Note: Since dealing with many different types, not going to use different colors for inputs and outputs. 

| Function | Color | Voltage Range | Description |
|----------|-------|-------------|---------|
| Gate     | ${\textsf{\color{orange}ffa500 Orange}}$| 0 or 5V | Voltage is binary - high or low |
| Trigger  | ${\textsf{\color{red}ff0000 Red}}$      | 0 or 5V | Momentary voltage spike. Voltage is binary |
| Clock    | ${\textsf{\color{yellow}ffa500 Yellow}}$| 0 or 5V | Gate, but specifically a clock signal |
| CV       | ${\textsf{\color{blue}0000ff Blue}}$ to ${\textsf{\color{blue}0000ff Blue}}$ | -5V to 5V | For modulation. Negative values are blue and positive green |
| 1V/oct   | ${\textsf{\color{purple}800080 Purple}}$  | -3V to 5V | Pitch. only single color used for entite range, with proportional intensity relative to pitch |
| LFO      |
| MIDI     |

Notes:
 * Might determine that voltages should actually range from -7V to 7V instead of just -5V to 5V.
 * 1V/Oct upper limit is 5V instead of standard 7V since all inputs/outputs will have same upper limit, which is currently 5V 

## Buchla colors
Buchla synths use colored jacks to indicate their purpose. these colors are described in 
the [Buchla 200h Users Guide](https://buchla.com/guides/200h_Users_Guide_v0.9.pdf) and are dis-layed below.
but these colors represent just a couple of functions, plus they are not consistently used Therefore
not using them for Modulencee.

![image](https://github.com/user-attachments/assets/07a82e04-7929-4b97-b308-0184866b12fe)

![image](https://github.com/user-attachments/assets/14e4e557-b865-4d02-ae30-7a0bca546145)

