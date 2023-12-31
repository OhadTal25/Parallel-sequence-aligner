README FILE:
Name: Ohad Tal
Name: Michael Hohashvili

Algorithm and Data Structures Used in this project:

Algorithm:
- The program reads a main sequence and a set of sequences from an input file.
- It then distributes the sequences evenly among MPI processes using parallel computation.
- For each local sequence, the program calculates alignment scores with the main sequence.
- The alignment score is calculated by comparing characters from local and main sequences.
- The program uses OpenMP to further parallelize the calculation of alignment scores for each local sequence.
- The alignment score, offset, and mutation are determined for each sequence.
- The results are gathered and printed in the master process.
- The program uses MPI to manage process communication.

Data Structures:
- The scoreTable is a 26x26 matrix that stores scores for character alignment.
- Each process receives a chunk of local sequences to work on.
- The results array stores alignment score, offset, and mutation for each local sequence.
- The sequences array holds the main sequence and all sequences read from the input file.

Run the project:
- Use 'make' to compile the program.
- Use 'make run' to run the program with the input provided in 'input.txt'.
- Use 'make clean' to remove compiled objects and executable.



Paralle program:
Total execution time: 4.858334 seconds
Total execution time: 5.090483 seconds
Total execution time: 5.303408 seconds
Total execution time: 5.332927 seconds
Total execution time: 5.798989 seconds
Total execution time: 6.111139 seconds
Total execution time: 6.481624 seconds


Non-parallel program: 
Total execution time: 5.636489 seconds
Total execution time: 5.597018 seconds
Total execution time: 5.649328 seconds
Total execution time: 5.647196 seconds
Total execution time: 5.590400 seconds
Total execution time: 5.608740 seconds
Total execution time: 5.630834 seconds
