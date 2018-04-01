# assign3

Fully tested C implementation of Kruskal's Minimum Spanning Tree Algorithm. Includes makefile, test cases, and scripts to automatically run tests. Make test command runs a multitude of tests and then compare the output to correct output.

<em><i>Was able to implement at efficieny approximately 40% faster than professor.</i></em>


Command Line examples:</br>
~$ make</br>
~$ ./matilda -i -p -b file</br>
(takes the infix expression from 'file' and converts to postfix. -i prints input from 'file', -p prints postfix, -b prints binary tree used for storing equation)</br>
~$ make test</br>
(runs program on plethora of tests and uses diff command w/ previously known correct outputs to check correctness)</br>
~$ make clean</br>
(cleans directory of executables for full recompilation)
