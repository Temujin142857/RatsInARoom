# Welcome to RatsInARoom, the worlds first rat oriented programming language

RatsInARoom is an eso-lang based off of the copy pasta: "Crazy? I was crazy once. They locked me in a room. A rubber room. A rubber room with rats. The rats made me crazy. Crazy? I was crzy once. They locked me in a room. A rubber room. A rubber room with rats. The rats made me crazy. Crazy?..."

The general idea is that all data is stored in the RubberRoom, in the form of rats. You can have the rats hold values, they can speak, you can take their values, and you can instruct them to perform functions.

All looping and if statement type operations are done through rng for that reason I call it a non-deterministic programming language.  
The Crazy keyword serves simultaneously as a goto and a label. Each time it is encountered it jumps to a random Crazy statement. If you're lucky it will jump to the one you want and loop/work like an if statement, otherwise it could jump to first one and restart the program, or the last one and just exit. It's similar to the copy pasta, how people repeat it over and over off of the word crazy.

## In this repo
You'll find the basic syntax rules defined in the RatsInARoom.drawio file, and the lexical transistion diagram in RRTransistionDigram.draw.io.
Both are more written from a development perspective rather than an explanatory persective. I'll probably write some more explanatory documentation once it's finished.

The RatsInARoom.c file is the compiler, it transpiles to C and then lets the GCC take it from there. Or at least it will once it's done, for the moment it just does lexical analysis, tokenizes the source code, and does the syntax analysis.

I've also included two demo files, firstly helloWorldDemo.rr which shows how hello world would be written in RatsInARoom.  
And loopDemo.rr, which shows how the crazy statements work as a loop. Note based on how the rng works you could end up with a lot of differnt things being printed. maybe it will keep looping back to opening Crazy?; line, and printing 1 over and over again. Maybe it will immediately jump to the closing Crazy?; statement, ending the program. Or maybe it will go infinite, looping between the first three crazy's until it crashes.
