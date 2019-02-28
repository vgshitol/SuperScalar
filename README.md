# SuperScalar

Develop a SuperScalar Pipelined Architecture that Implements ROB, In order and out of order execution

## Running the Code 

1. Type "make" to build.  (Type "make clean" first if you already compiled and want to recompile from scratch.)

2. Run trace reader:

./sim ROB_SIZE IQ_SIZE WIDTH ../../proj3_traces/val_trace_gcc1.txt

   To run without throttling output:
   ./sim 256 32 4 ./validationTraces/val_trace_gcc1
