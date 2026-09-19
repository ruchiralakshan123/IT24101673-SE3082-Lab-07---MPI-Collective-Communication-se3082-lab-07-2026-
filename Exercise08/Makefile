# Compile and run all six MPI array-sum exercises.
# Usage (from Exercise08): make
#                           make run
#                           make clean

NP := 4

ifeq ($(OS),Windows_NT)
    # Windows fallback for Microsoft MPI.  The SDK/runtime are installed by
    # winget packages Microsoft.msmpisdk and Microsoft.msmpi respectively.
    EXE := .exe
    CC := gcc
    MPI_SDK := C:/Program Files (x86)/Microsoft SDKs/MPI
    CFLAGS := -O2 -Wall -I"$(MPI_SDK)/Include"
    LDFLAGS := -L"$(MPI_SDK)/Lib/x64"
    LDLIBS := -lmsmpi
    RUNNER := "C:/Program Files/Microsoft MPI/Bin/mpiexec.exe"
    CLEAN := cmd /C del /Q
else
    # Standard MPI setup used on Linux/macOS, where mpicc and mpirun are on PATH.
    EXE :=
    CC := mpicc
    CFLAGS := -O2 -Wall
    LDFLAGS :=
    LDLIBS :=
    RUNNER := mpirun
    CLEAN := $(RM)
endif

TARGETS := ex1_sum_bcast$(EXE) ex2_sum_scatter$(EXE) ex3_sum_gather$(EXE) \
           ex4_sum_reduce$(EXE) ex5_sum_allreduce$(EXE) ex6_sum_scan$(EXE)

all: $(TARGETS)

ex1_sum_bcast$(EXE): ../Exercise01/ex1_sum_bcast.c

ex2_sum_scatter$(EXE): ../Exercise02/ex2_sum_scatter.c

ex3_sum_gather$(EXE): ../Exercise03/ex3_sum_gather.c

ex4_sum_reduce$(EXE): ../Exercise04/ex4_sum_reduce.c

ex5_sum_allreduce$(EXE): ../Exercise05/ex5_sum_allreduce.c

ex6_sum_scan$(EXE): ../Exercise06/ex6_sum_scan.c

$(TARGETS):
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) $(LDLIBS)

run: all
	@echo "===== Exercise 1: Bcast + Send/Recv ====="
	$(RUNNER) -np $(NP) ./ex1_sum_bcast$(EXE)
	@echo ""
	@echo "===== Exercise 2: Scatter + Send/Recv ====="
	$(RUNNER) -np $(NP) ./ex2_sum_scatter$(EXE)
	@echo ""
	@echo "===== Exercise 3: Scatter + Gather ====="
	$(RUNNER) -np $(NP) ./ex3_sum_gather$(EXE)
	@echo ""
	@echo "===== Exercise 4: Scatter + Reduce ====="
	$(RUNNER) -np $(NP) ./ex4_sum_reduce$(EXE)
	@echo ""
	@echo "===== Exercise 5: Scatter + Allreduce ====="
	$(RUNNER) -np $(NP) ./ex5_sum_allreduce$(EXE)
	@echo ""
	@echo "===== Exercise 6: Scatter + Scan ====="
	$(RUNNER) -np $(NP) ./ex6_sum_scan$(EXE)

clean:
	-$(CLEAN) $(TARGETS)

.PHONY: all run clean
