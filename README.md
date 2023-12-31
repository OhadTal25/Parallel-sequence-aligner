# Parallel-sequence-aligner

## Contributors
- Ohad Tal
- Michael Hohashvili

## Algorithm and Data Structures

### Algorithm:
This project implements a parallel computation approach using MPI to distribute sequences for alignment scoring with OpenMP optimization. It involves:
- Reading sequences from an input file.
- Distributing sequences among MPI processes.
- Calculating alignment scores using OpenMP.
- Aggregating and displaying results in the master process.

### Data Structures:
- `scoreTable`: A matrix for alignment scores.
- `local sequences`: Chunks of sequences processed by each MPI unit.
- `results`: An array holding the alignment score, offset, and mutation details.
- `sequences`: An array containing the main and input file sequences.

## Usage
- Compile with `make`.
- Execute with `make run` (utilizes 'input.txt').
- Clean with `make clean`.

## Performance
The parallel implementation significantly optimizes execution times, as evidenced by the comparison with non-parallel execution.

### Parallel Execution Times:
- Ranging from approximately 4.86 to 6.48 seconds.

### Non-Parallel Execution Times:
- Consistently around 5.59 to 5.64 seconds.
