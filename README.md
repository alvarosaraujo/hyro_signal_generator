# hyro_signal_generator
Signal generator exercise

# Compiling and running
Clone this repository to `$hyro_workspace/src`, navigate to `$hyro_workspace/build` create build files using<br/>
`cmake ../src`<br/>
compile the package with<br/>
`make`<br/>
and run the package with the command<br/>
`bin/hyro_signal_generator`

# Package description
This exercise creates two HyRO components that communicates with each other and shows the results on a Plot2d widget, those components are SignalGeneratorComponent and DigitalConverterComponents. The former generates a sinusoid signal based on properties that can be dynamically changed, such as frequency and amplitude. The output of this component is sent to the other component using a custom message. The latter receives this signal and generates a digital signal based on properties that can also be dynamically changed, which are amplitude and threshold. Both outputs are sent to the plot2d component, which creates two graphs for those signals in real time, while the application is running.
