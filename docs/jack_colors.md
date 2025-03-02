# Jack Colors

want each jack to have a RGB LED to indicate current input or output. not just the value of the voltage, but also 
what the voltage represents (gate, trigger, 1v/oct, etc). Since the modulencer will have many jacks, and 
most of the jacks can handle different signal types,  this could really 
help user keep track of what they are doing. 

## Proposed colors
Note: Since dealing with many different types, not going to use different colors for inputs and outputs. 

| Function | Color | Voltage Range | Description |
|----------|-------|-------------|---------|
| Trigger  | ${\textsf{\color{red}ff0000 Red}}$      | 0 or 7V | Momentary voltage spike. Voltage is binary |
| Gate     | ${\textsf{\color{orange}ffa500 Orange}}$| 0 or 7V | Voltage is binary - high or low |
| Envelope | ${\textsf{\color{yellow}ffa500 Yellow}}$| 0 to 7V | Intensity proportional to voltage |
| Clock    | ${\textsf{\color{white}ffffff White}}$. | 0 or 7V | Gate, but specifically a clock signal |
| CV       | ${\textsf{\color{purple}6000ff Purple}}$ to ${\textsf{\color{brown}60ff00 Brown}}$ | -7V to 7V | For modulation. Negative values are purple and positive brown |
| LFO      | ${\textsf{\color{blue}0000ff Blue}}$ to ${\textsf{\color{green}00ff00 Green}}$ | -7V to 7V | For slow changing modulation. Negative values are blue and positive green |
| 1V/oct   | ${\textsf{\color{purple}800080 Purple}}$  | -3V to 7V | Pitch. Only single color used for entite range, with proportional intensity relative to pitch |
| MIDI     | ${\textsf{\color{yellow}ffa500 Yellow}}$| MIDI | Any MIDI command, except for clock ones since those are so verbose |

Notes:
 * Might determine that voltages should actually range from -10V to 10V instead of just -7V to 7V, but sure seems that -7 to 7V range would be sufficient. 
 * 1V/Oct upper limit is 5V instead of standard 7V since all inputs/outputs will have same upper limit, which is currently 5V 

## Buchla colors
Buchla synths use colored jacks to indicate their purpose. these colors are described in 
the [Buchla 200h Users Guide](https://buchla.com/guides/200h_Users_Guide_v0.9.pdf) and are dis-layed below.
but these colors represent just a couple of functions, plus they are not consistently used Therefore
not using them for Modulencee.

![image](https://github.com/user-attachments/assets/07a82e04-7929-4b97-b308-0184866b12fe)

![image](https://github.com/user-attachments/assets/14e4e557-b865-4d02-ae30-7a0bca546145)

