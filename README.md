
# QuickInterp -- Superinstructions on OpenJDK
_QuickInterp_ is the name for OpenJDK Zero (version 11) with superinstructions, and is the implementation I created for my master thesis. All this code is based on OpenJDK 11+28. My master thesis is available here:  https://essay.utwente.nl/81408/ Chapter 5 describes the implementation of the code here.

## Scope of this project

This project was created solely to support and test the designs from my master thesis. I have no intention in creating this into an actual product, giving it back to OpenJDK, or maintaining it myself after getting it to a state where it can be understood by others. This is also reflected in the quality of the code (please be gentle). For example, the only usable garbage collector is epsilon (i.e. they're all broken), and the class verifier is also broken.

## Status and TODO

I am in the process of tidying up this repository. I originally worked directly on the Mercurial version of the OpenJDK repository, and have since converted it to git to help make my code accessible. Not everything is converted yet as some patch queue patches refuse to apply to the git project. Furthermore, not all code was implemented directly on top of OpenJDK, but rather as an external tool. In short, what I still have to add:

- Shortest path with equivalence (mq patch refuses to apply)
- The profile-parser-interpreter-optimizer-generator tool

Furthermore

- Rename certain runtime substitution classes as their names do not match the names used in my thesis (the current names are bad)
- Hardcoded path of the superinstruction set definition has to be removed (`/home/lukas/...`)
- Add build instructions and a short summary of how to use it
- Link the benchmarks (they will be in a separate repository)
- Give credits to the JNIF and ASM people as I added (derived versions of) their code to this repository
