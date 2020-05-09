# automata

Supported grammars
------------------
    r = (s)
    r = st
    r = s+t
    r = s*
    r = e (epsilon)
    

Examples

    (a|b)*
    (a*|b*)*
    ((e|a)b*)*
    (a|b)*abb(a|b)*


NFA string acceptance by Dynamic Memory
---------------------------------------
C implementation of a program which accepts a non-deterministic finite automata and a string.
Output if string is accepted by NFA or not.

NFA to DFA
----------
Converting non-deterministic finite automata to deterministic finite automata.

Package required - xdot
Installation -
sudo apt-get install xdot

RE to DFA
---------
C program which generates a deterministic finite automata graph from the input regular expression.

Package required - xdot
Installation -
sudo apt-get install xdot

RE to NFA (bottom up)
---------------------
C program which generates a non-deterministic finite automata graph from the input regular expression.
Using bottom up approach.

Package required - xdot
Installation -
sudo apt-get install xdot
