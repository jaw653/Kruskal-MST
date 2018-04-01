# assign3

Fully tested C implementation of Kruskal's Minimum Spanning Tree Algorithm. Includes makefile, test cases, and scripts to automatically run tests. Make test command runs a multitude of tests and then compare the output to correct output.

<em><i>Was able to implement at efficieny approximately 40% faster than professor.</i></em>


Command Line examples:</br>
~$ make</br>
~$ kruskal graphFile</br>
(takes graph file filled with edges, end nodes, and those edge weights and runs Krukal's algorithm to find the minimum spanning tree)</br>
~$ make test</br>
(runs program on plethora of tests and uses diff command w/ previously known correct outputs to check correctness. Found to be accurate to 100,000+ nodes)</br>
~$ make clean</br>
(cleans directory of executables for full recompilation)
