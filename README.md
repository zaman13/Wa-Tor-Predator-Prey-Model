# Wa-Tor-Predator-Prey-Model
C++ SFML implementation of Wa-Tor Predator-Prey dynamics simulation. The algorithm is based on the 1984 paper by A.K. Dewdney. The simulation takes place on a toroidal space populated by fishes and sharks. The sharks hunt the fishes. By simulating over a large grid, oscillations in the populations can be observed which is a characterisitic of predator-prey models. Real-time plots of the fish and shark populations are presented alongside the evolution of the toroidal grid with time. 


## Algorithm
Every shark and fish has a age value and a 2D position coordinate. In addition, every shark has a *starve* value which indeicates the time elapsed after its last meal. The spawning rate of fishes and sharks are determined by the parameters *fbreed* and *sbreed*. The starving thereshold of sharks is set by the parameter *starve_max*. The movement of the fishes and sharks are controlled by simple rules:

__1.__ A fish moves to an empty neighboring cell at random. If it does not find an empty cell, it does not move.

__2.__ A shark looks for a neighboring cell occupied by a fish. If it finds multiple such cells, it moves to one of those cells at random. The fish at that location is devoured. If not, it will move to an empty neighboring cell at random. If it does not have an empty neigboring cell, then it does not move.

__3.__ At each time step, the age of the shark and fish are increased by 1.

__4.__ If the age of a fish is greater than *fbreed*, then it creates a new fish at its old position before moving to the new position. The age of the fish is reset to 1 (or zero depending on the implementation).

__5.__ If the age of a shark is greater than *sbreed*, then it creates a new shark at its old position before moving to the new position. The age of the shark is reset to 1 (or zero depending on the implementation).

__6.__ If at a time step a shark does not find a fish, then its starve value is incremented by 1. If starve value is greater than *starve_max*, the shark dies.

More details about the rules can be found in Ref. [1].

## Implementation notes
__1.__ If the position of fishes and sharks are updated sequentially (going through 2 nested loops with normal indexing), then a net motion of the fish schools (and sharks) can be observed. This is avoided by using random indexed nested loops.  

__2.__ SFML VertexArrays are used for the visualization. This way, a single draw command show all the fish/shark population at once. This is much more efficient then drawing each cell independently which can result in very low frame rate for large grids.  


## Dependencies
The code requires C++ compiler and SFML.

## Running the code
Run the __Makefile__ in the terminal. Then run the executable (__./wat-tor_output__) file. 

## Sample Output



## References
1. Alexander Keewatin Dewdney, "Sharks and fish wage an ecological war on the toroidal planet Wa-Tor." Scientific American 251.6 (1984): 14-22.
