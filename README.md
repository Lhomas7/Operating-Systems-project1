# Operating-Systems-project1
# Author: Landon Thomas

# KNOWN PROBLEMS
This project has been designed to meet the expectations of the project 
description and outline, and in doing so has several shortcomings. The
foremost is that the project has only been tested to work on Linux x86-64
servers with the most notable piece being reading from the return register. 
Both programs fail on MacOS. 

Another shortcoming is the reliance on proper 
command-line arguments. Leakcount expects there to be executable arguments 
given by command line, and when there are none ther program fails. Similarly,
sctracer expects 3 command line arguments each time, the first being the 
program executed, the second is the program to execute, and the third is the
output file to print results to. If there are less and 3 arguments or they are
given out of order, the program will fail.

# DESIGN
This project is designed to have two programs within it, one for tracking
memory leaks and another for tracing system calls. The memory leak tracker 
functions by utilizing a shim as a preloaded library that intercepts calls to
malloc, calloc, realloc, and free, providing overridden versions that place 
allocated data into a dynamically allocated and resizeable array that tracks
them, then prints the leaks to STDERR through a destrcutor at the program's 
end.

The system call tracer works by forking the program and executing another
program in the child process and tracing it using ptrace. The parent process
will wait until a system call occurs, then once it does it stores the system
call number and its number of occurrences throughout the program, doing so 
only on an entry. Once the child process exits, the parent process sorts the
call numbers from lowest to highest and prints the results to an output file

# References
Working on this project required me to know some library functions, how ptrace
functioned, and also proper syntax for code. Below are listed the websites used
as reference:

https://man7.org/linux/man-pages/man2/ptrace.2.html (Linux manual page)

https://www.geeksforgeeks.org/cpp/strtok-strtok_r-functions-c-examples/ (geeksforgeeks)

https://linux.die.net/man/3/execv (linux manual page)
