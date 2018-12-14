#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sim_proc.h"

/*  argc holds the number of command line arguments
    argv[] holds the commands themselves

    Example:-
    sim 256 32 4 gcc_trace.txt
    argc = 5
    argv[0] = "sim"
    argv[1] = "256"
    argv[2] = "32"
    ... and so on
*/
int main (int argc, char* argv[])
{
    FILE *FP;               // File handler
    char *trace_file;       // Variable that holds trace file name;
    proc_params params;       // look at sim_bp.h header file for the the definition of struct proc_params
    //int op_type, dest, src1, src2;  // Variables are read from trace file
    //unsigned long int pc; // Variable holds the pc read from input file
    
    if (argc != 5)
    {
        printf("Error: Wrong number of inputs:%d\n", argc-1);
        exit(EXIT_FAILURE);
    }
    
    params.rob_size     = strtoul(argv[1], NULL, 10);
    params.iq_size      = strtoul(argv[2], NULL, 10);
    params.width        = strtoul(argv[3], NULL, 10);
    trace_file          = argv[4];
//    printf("rob_size:%lu "
//            "iq_size:%lu "
//            "width:%lu "
//            "tracefile:%s\n", params.rob_size, params.iq_size, params.width, trace_file);
    // Open trace_file in read mode
    FP = fopen(trace_file, "r");
    if(FP == NULL)
    {
        // Throw error and exit if fopen() failed
        printf("Error: Unable to open file %s\n", trace_file);
        exit(EXIT_FAILURE);
    }
    
    pipeline pipe(FP, params.width, params.rob_size, params.iq_size);
    pipe.Make_RMT();
    pipe.Make_ROB();
    pipe.Make_IQ();
    pipe.Make_execute_list();
//    while(fscanf(FP, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2) != EOF)
//    {
//
//        printf("%lx %d %d %d %d\n", pc, op_type, dest, src1, src2); //Print to check if inputs have been read correctly
//        /*************************************
//            Add calls to OOO simulator here
//        **************************************/
//    }
    //int i =0;
    
    do{
        pipe.Retire();
        pipe.Writeback();
        pipe.Execute();
        pipe.Issue();
        pipe.Dispatch();
        pipe.RegRead();
        pipe.Rename();
        pipe.Decode();
        pipe.Fetch();
        //i++;
        
    } while(pipe.Advance_Cycle());
    
    
    printf("# === Simulator Command =========\n");
    printf("#");
    printf(" ./sim %lu %lu %lu %s\n", params.rob_size, params.iq_size, params.width, trace_file);
    printf("# === Processor Configuration ===\n");
    printf("# ROB_SIZE = %lu\n", params.rob_size);
    printf("# IQ_SIZE = %lu\n", params.iq_size);
    printf("# WIDTH = %lu\n", params.width);
    printf("# === Simulation Results ========\n");
    pipe.print_results();
    //printf("");
    return 0;
}
