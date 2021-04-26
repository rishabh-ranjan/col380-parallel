NOTES:

1. Add the following to bashrc in order:

module load apps/anaconda/3
module load compiler/gcc/7.4.0/compilervars
module load compiler/mpi/openmpi/2.0.1/gnu

2. Use make to compile in DEBUG mode: prints input (inp), processing (prc) and output (out) times.

3. Use compile.sh to compile in RELEASE mode: no output.

4. make tc N=1000 will make a 1000 x 1000 example input 1000.A.

5. make clean removes executables, tests and outputs.

6. make is for personal convenience, not for submission.

7. See run.sh for how to run.

TODO:

1. Test on Linux on personal machine.

2. Rigorous testing:
	2.1 correctness: even though 1000 <= N <= 5000 and T = 2,4,8,16 will be used for competitive scoring,
		correctness for all values of N and T matters. Check corner cases eg. T=1, N=1.
	2.2 stability: check with format_checker.py for many inputs, especially for MPI
		(I have used floats instead of doubles in MPI under the assumption that floats provide
		sufficient precision -- floats are faster to move around than doubles)

3. Specifications:
	3.1 30Gb RAM
	3.2 scripts etc
	3.3 stuff mentioned in pdf (and piazza)
	3.4 exit(0) for L[j,j] == 0

4. Experiments, Plots, Explanation (I'll do that: Rishabh), Report
	
