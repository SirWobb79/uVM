# uVM (Micro Virtual Machine)
A project made to see what a good mix of minimality and ease of use makes.

## What is this?
This is essentially an assembly machine. It comes with its own assembly and syntax, with the tools necessary to use this comfortably.

## What can it do?
It can run any program written in its assembly, as long as it is converted to a .uvm file via the Python converter script (bundled). On a technical basis, it allows for 64KB of RAM to be toyed and worked with by 12 commands. These commands are nothing very unique, but should provide enough flexability to do at least some interesting things. Users can output various bits of text, data, and perhaps even make a game through this thing.

There is an example program to get an idea of the type of programming users are expected to write. It demonstrates things like looping, labels, and formatting outputs through a simple semi-infinite multiplier, where two numbers are entered and the program spits out the product. It will only terminate when both inputs are zero.

## Where can I use it?
Currently a Windows exclusive application, at least in terms of precompiled content. The C source should be avaliable for self compilation, if that is what you need.
