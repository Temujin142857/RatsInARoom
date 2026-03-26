# Welcome to RatsInARoom, the worlds first rat oriented programming language

It's based off of the copy pasta: "Crazy? I was crazy once. They locked me in a room. A rubber room. A rubber room with rats. The rats made me crazy. Crazy? I was crzy once..."

The general idea is that all data is stored in the RubberRoom, in the form of rats. And all looping and if statement type operations are done through rng.

The Crazy keyword serves simultaneously as a goto and a label. Each time it is encountered it jumps to a random Crazy statement. If you're lucky it will jump to the one you want and loop/work like an if statement, otherwise it could jump to first one and restart the program, or the last one and just exit.

##In this repo
You'll find the basic syntax rules defined in the RatsInARoom.drawio file, and the lexical transistion diagram in RRTransistionDigram.draw.io.
Both are more written from a development perspective rather than an explanatory persective.

The RatsInARoom.c file is the compiler, it transpiles to C and then lets the GCC take it from there. Or at least it will once it's done, for the moment it just does lexical analysis, tokenizes the source code, and does the syntax analysis.
