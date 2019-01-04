#ifndef SIM_PROC_H
#define SIM_PROC_H
#include <vector>
using namespace std;

typedef struct proc_params{
    unsigned long int rob_size;
    unsigned long int iq_size;
    unsigned long int width;
}proc_params;

class instructions{
public:
    unsigned long int pc;
    int ins_op_type; //could be type 0,1 or 2 (1 cycles, 2 cycles, 5 cycles).
    int dest; //desitination register
    int src1; //source register
    int src2; //source register
    int ins_cycle; //keeps track of 1 cycles, 2 cycles or 5 cycles then used to decremnt in execute
    int age; //keeps track of age of the instruction
    int execute_valid;
    int orig_src1;
    int orig_src2;
    bool src1_ready; //taken RMT check / if true its taken from the ROB
    bool src2_ready; //taken RMT check
    bool src1_in_rob;
    bool src2_in_rob;
    int FE_entered;
    int DE_entered;
    int RN_entered;
    int RR_entered;
    int DI_entered;
    int IS_entered;
    int EX_entered;
    int WB_entered;
    int RT_entered;
    int FE_exit;
    int DE_exit;
    int RN_exit;
    int RR_exit;
    int DI_exit;
    int IS_exit;
    int EX_exit;
    int WB_exit;
    int RT_exit;
    instructions(unsigned long int input_pc=0, int input_ins_op_type=0, int input_dest=0, int input_src1=0,int input_orig_src1=0 ,int input_orig_src2=0, int input_src2=0, int in_age=0, int input_cycle=0){
        pc = input_pc;
        ins_op_type = input_ins_op_type;
        dest = input_dest;
        src1 = input_src1;
        src2 = input_src2;
        orig_src1 = input_orig_src1;
        orig_src2 = input_orig_src2;
        age = in_age;
        if(input_ins_op_type == 0) input_cycle = 1;
        else if(input_ins_op_type == 1) input_cycle = 2;
        else input_cycle = 5;
        ins_cycle = input_cycle;
        execute_valid = 0; //set flag high if taken in execute stage for easy search through the list
        src1_ready = false; //taken RMT check / if true its taken from the ROB
        src2_ready = false; //taken RMT check
        src1_in_rob = false;
        src2_in_rob = false;
        FE_entered = 0;
        DE_entered = 0;
        RN_entered = 0;
        RR_entered = 0;
        DI_entered = 0;
        IS_entered = 0;
        EX_entered = 0;
        WB_entered = 0;
        RT_entered = 0;
        FE_exit = 0;
        DE_exit = 0;
        RN_exit = 0;
        RR_exit = 0;
        DI_exit = 0;
        IS_exit = 0;
        EX_exit = 0;
        WB_exit = 0;
        RT_exit = 0;
    }
};

class ROB_type{
public:
    int ROB_dest;
    unsigned long int ROB_pc;
    int ROB_ready;
    instructions ROB_instructions;
    ROB_type(unsigned long int input_pc=0, int input_ROB_dest=0, int input_ROB_ready=0):ROB_dest(input_ROB_dest), ROB_pc(input_pc) ,ROB_ready(input_ROB_ready){}
};

class Rename_Map_Table{
public:
    int valid;
    unsigned long int ROB_tag;
    Rename_Map_Table(int input_valid=0, int input_ROB_tag=-1):valid(input_valid), ROB_tag(input_ROB_tag){}
};

class IQ_type{
public:
    int IQ_valid;
    int IQ_dest_reg;
    int IQ_src1_ready;
    int IQ_src2_ready;
    instructions IQ_instructions;
    IQ_type(int in_IQ_valid=0, int in_dest_reg=0, int in_IQ_src1_ready=0, int in_IQ_src2_ready=0){
         IQ_valid= in_IQ_valid;
         IQ_dest_reg = in_dest_reg;
         IQ_src1_ready = in_IQ_src1_ready;
         IQ_src2_ready = in_IQ_src2_ready;
    }
};

class pipeline{
public:
    FILE *pointer;
    int totalCycle;
    unsigned long int width;
    unsigned long int rob_size;
    unsigned long int iq_size;
    unsigned long int ROB_head;
    unsigned long int ROB_tail;
    int ROB_counter; //keeps track to see if ROB is full or not;
    int ADV_cycle_counter;
    int set_max_Issue; //keeps limit of how many to instructions to send to execute
    int IQ_counter; //keeps track to see if IQ is full
    int age_counter;
    int fetch_counter;
    int decode_counter;
    int rename_counter;
    int function_end;
    void Fetch();
    void Decode();
    void Rename();
    void RegRead();
    void Dispatch();
    void Issue();
    void Execute();
    void Writeback();
    void Retire();
    bool Advance_Cycle();
    void Make_RMT();
    void Make_ROB();
    void Make_IQ();
    void Make_execute_list();
    void wakeup(int rob_tag_to_wake, unsigned long int iqSize,unsigned long int width );
    unsigned long int free_EX_spot(unsigned long int ex_size);
    unsigned long int check_iq_free(unsigned long int iq_SIZE);
    void IQ_clear(int index); //clear values of IQ once moved to ex stage
    unsigned long int free_IQ_spot(unsigned long int iq_SIZE);
    void comp_RMT_to_ROB(unsigned long int RobTag);
    void print_from_retire(unsigned long int ROB_index_to_print);
    void print_results();
    
    vector <instructions> DE;
    vector <instructions> RN;
    vector <instructions> RR;
    vector <instructions> DI;
    vector <IQ_type> IQ;
    vector <instructions> execute_list;
    vector <instructions> WB;
    vector <ROB_type> ROB;
    vector <Rename_Map_Table> RMT;
    
    
    pipeline(FILE *FP, unsigned long int in_width, unsigned long int in_rob_size,unsigned long int in_iq_size) {
        pointer = FP;
        width = in_width;
        rob_size = in_rob_size;
        iq_size = in_iq_size;
        totalCycle = 0;
        ROB_head = 0;
        ROB_tail = 0;
        ROB_counter = 0;
        ADV_cycle_counter = 0;
        IQ_counter = 0;
        age_counter = 0;
        fetch_counter =0;
        set_max_Issue =0;
        function_end = 0;
    }
};


void pipeline::Make_IQ(){ //Create IQ table
    for(unsigned long int l = 0; l < iq_size; l++){
        IQ.push_back(0);
    }
}

void pipeline::Make_ROB(){
    for(unsigned long int k = 0; k < rob_size; k++){ //67 is the specified number of registors for the program.
        ROB.push_back(0);
    }
}
void pipeline::Make_RMT(){ //Create Registor Map Table
    for(int k = 0; k < 67; k++){
        RMT.push_back(0);
    }
}

void pipeline::Make_execute_list(){ //create execute list
    for(unsigned long int j = 0; j < width * 5; j++){
        execute_list.push_back(0);
    }
}

void pipeline::wakeup(int rob_tag_to_wake, unsigned long int iqSize,unsigned long int width ){
    //wake up tag in RR, DI, IS in here
    for(unsigned long int h = 0; h < width; h++){
        //Check RR to wakeup
        if(RR[h].src1_ready == 0){ //check for source 1
            if(RR[h].src1 == rob_tag_to_wake){
                RR[h].src1_ready = 1; //set valid to src if the rob tag matches
            }
        }
        
        
        if(RR[h].src2_ready == 0){ //check for source 2
            if(RR[h].src2 == rob_tag_to_wake){
                RR[h].src2_ready = 1; //set valid to src if the rob tag matches
            }
        }
        
        //check DI to wakeup
        if(DI[h].src1_ready == 0){ //check soure 1
            if(DI[h].src1 == rob_tag_to_wake){
                DI[h].src1_ready = 1; //set valid to src if the rob tag matches
            }
        }
        
        //check for source 2
        if(DI[h].src2_ready == 0){
            if(DI[h].src2 == rob_tag_to_wake){
                DI[h].src2_ready = 1; //set valid to src if the rob tag matches
            }
        }
    }
    
    //check Iq to wakeup
    for(unsigned long int j = 0; j < iqSize; j++){
        //check for source 1
        
        if(IQ[j].IQ_src1_ready == 0){
            if(IQ[j].IQ_instructions.src1 == rob_tag_to_wake){
                IQ[j].IQ_src1_ready = 1; //set valid to src if the rob tag matches
            }
        }
        
        //check for source 2
        if(IQ[j].IQ_src2_ready == 0){
            if(IQ[j].IQ_instructions.src2 == rob_tag_to_wake){
                IQ[j].IQ_src2_ready = 1; //set valid to src if the rob tag matches
            }
        }
    }
}


//counts how many free IQ spots are available
unsigned long int pipeline::check_iq_free(unsigned long int iq_SIZE){
    unsigned long int iq_free = 0;
    for(unsigned long int n = 0; n < iq_SIZE; n++){
        //&& IQ[n].IQ_src1_ready == 0 && IQ[n].IQ_src2_ready == 0
        if(IQ[n].IQ_valid == 0){
            iq_free++;
        }
    }
    return iq_free;
}

//Get the next free spot top down from the IQ
unsigned long int pipeline::free_IQ_spot(unsigned long int iq_SIZE){
    unsigned long int free_spot = -1; //-1 represents none
    for(unsigned long int n = 0; n < iq_SIZE; n++){
        if(IQ[n].IQ_valid == 0){
            free_spot = n;
            break;
        }
    }
    return free_spot;
}

void pipeline::IQ_clear(int index){ //clear values of IQ once moved to ex stage
    IQ[index] = IQ_type(0);
}

unsigned long int pipeline::free_EX_spot(unsigned long int ex_size){
    unsigned long int free_spot_ex = -1;
    for(unsigned long int x = 0; x <ex_size; x++){
        if(execute_list[x].execute_valid == 0){
            return x;
        }
    }
    return free_spot_ex;
}

bool pipeline::Advance_Cycle() {
    totalCycle++;
    //keep running until ROB EOF or all registores are clear
    if(DE.size()==0 && RN.size()==0 && RR.size()==0 && DI.size()==0 && WB.size()==0 && function_end == 1){
        return false;
    }
    return true;
}

//Gets the instructions and pushes them to Decode
void pipeline::Fetch(){
    unsigned long int pc;
    instructions *temp = 0;
    int op_type, dest, src1, src2;
    if(DE.size() == 0) {
        for(unsigned long int i = 0; i < width; i++){
            if(fscanf(pointer, "%lx %d %d %d %d", &pc, &op_type, &dest, &src1, &src2) != EOF) {
                //printf("%lx %d %d %d %d\n", pc, op_type, dest, src1, src2);
                temp = new instructions(pc, op_type, dest, src1, src1, src2 ,src2, age_counter);
                temp->FE_entered = totalCycle;
                age_counter++; //increment age counter;
                temp->FE_exit = totalCycle + 1;
                temp->DE_entered = temp->FE_exit;
                DE.push_back(*temp);
                
            } else break;
        }
        fetch_counter++;
    }
}

void pipeline::Decode(){
    if(DE.size() != 0){
        if(RN.size() == 0){
            for(unsigned long int j = 0; j < width; j++){
                DE[j].DE_exit = totalCycle + 1; //Decode exited
                DE[j].RN_entered = totalCycle + 1; // Rename exited so update counter
                RN.push_back(DE[j]); //push back to Rename
            }
            DE.clear(); //clear Decode Registor
        }
    }
}

void pipeline::Rename(){
    //if RegRead is not empty return
    //printf("entrered rename\n");
    int *temp;
    bool *temp_src_ready = 0;
    int count;
    unsigned long int ROB_fit_size = ROB_counter + width;
    if(RN.size() != 0){
        if(RR.size() == 0){ //or rob is empty
//            if(RN[0].pc == 0x2b69e4){
//                printf("hello");
//            }
            if(ROB_fit_size <= rob_size){
                for(unsigned long int i = 0; i < width; i++){
                    count = 0;
                    for(int j = 0; j < 3; j++) { //break down the instructions
                        if(count == 0) { //for src1
                            temp = &RN[i].src1;
                            temp_src_ready = &RN[i].src1_in_rob;
                            count++;
                            
                        } else if(count == 1) { //scr2
                            temp = &RN[i].src2;
                            temp_src_ready = &RN[i].src2_in_rob;
                            count++;
                        } else {                //dest registor
                            //printf("count: 2\n");
                            temp = &RN[i].dest;
                            count++;
                        }
                        
                        //this only happens when it is checking for the destination registors
                        if(count > 2 ) { //check dest
                            //Allocate a space in ROB
                            ROB[ROB_tail].ROB_dest = *temp;
                            ROB[ROB_tail].ROB_pc = RN[i].pc; //store pc in ROB reg of reg = tail value
                            if(*temp != -1){  //change RMT only if destination is available
                                RMT[*temp].ROB_tag = ROB_tail; //store ROB_tag in RMT
                                RMT[*temp].valid = 1;
                                RN[i].dest = ROB_tail;
                             }
                            //make RMT valid
                            ROB[ROB_tail].ROB_instructions = RN[i];
                            ROB_tail++; //increment tail
                            if(ROB_tail == rob_size) {ROB_tail = 0; }
                            ROB_counter++; //increment rob counter to keep track if ROB is full
                            break;
                        }
                
                        if(RMT[*temp].valid){           //check RMT is valid
                            *temp = RMT[*temp].ROB_tag; //if valid change the destination regitor to rob value
                            *temp_src_ready = true;     //make source value true if taken from ROB
                        } //else *temp_src_ready = false;
                    }
                    RN[i].RN_exit = totalCycle + 1;
                    RN[i].RR_entered = totalCycle + 1;
                    RR.push_back(RN[i]); //push to RR registors
                    RN.clear();
                }
            }
      }
    }
}
void pipeline::RegRead(){
    if(RR.size() != 0){
        if(DI.size() == 0){
            for(unsigned long int k = 0; k < width; k++){
                
                if(!RR[k].src1_in_rob){ //if src1 reg_tag is not from rob, got it from ARF so its valid
                    RR[k].src1_ready = true;
                } else {
                    if(ROB[RR[k].src1].ROB_ready){ //if tag ready in ROB, set SRC_ready to valid else false
                        RR[k].src1_ready = true;
                    }
                }
                if(!RR[k].src2_in_rob){ //if src2 reg_tag is not from rob, got it from ARF so its valid
                    RR[k].src2_ready = true;
                } else {
                    if(ROB.at(RR[k].src2).ROB_ready){ //if tag ready in ROB, set SRC_ready to valid else false
                        RR[k].src2_ready = true; //set source 2 to ready
                    }
                }
                
                RR[k].RR_exit = totalCycle + 1;
                RR[k].DI_entered = totalCycle + 1;
                DI.push_back(RR[k]); //Push back to Registor Read
                RR.clear(); //Clear out Regitpr read
            }
        }
    }
}
void pipeline::Dispatch(){
    if(DI.size() != 0){ //if DI has a bundle
        unsigned long int free_IQ_spots = check_iq_free(iq_size); //returns free IQ spots
        unsigned long int free_IQ_index = -1; //get the next available index
        if(free_IQ_spots >= width) { //if IQ has more equal to or more than the DI then add to queue
            for(unsigned long int i = 0; i < width; i++) { //interate upto the width
                free_IQ_index = free_IQ_spot(iq_size);
                IQ[free_IQ_index].IQ_valid = 1; //make IQ valid
                IQ[free_IQ_index].IQ_dest_reg = DI[i].dest; // copy DI's destination to IQ
                IQ[free_IQ_index].IQ_src1_ready = DI[i].src1_ready; //Copy over DI scr1_ready to IQ
                IQ[free_IQ_index].IQ_src2_ready = DI[i].src2_ready; //Copy over DI scr2_ready to IQ
                DI[i].DI_exit = totalCycle + 1; //DI exited IQ
                DI[i].IS_entered = totalCycle + 1; //instruction entered IQ at this cycle
                IQ[free_IQ_index].IQ_instructions = DI[i]; //copy over instruction class to IQ from DI
                IQ_counter++; //increment counter
                
            }
            DI.clear();
        }
    }
}
void pipeline::Issue(){
    //unsigned long int get_oldest = check_oldest(iq_size);
    unsigned long int execution_list_size = width*5;
    unsigned  long int open_execution_index;
    int current_age = 0;
    int oldest = 0; //holds the oldest value
    unsigned long int set_max = 0;
    //if(set_max < width) {
        for(int n = 0; n < age_counter; n++){ //interate through the age counter max ages
            for(unsigned long int j = 0; j < iq_size; j++){ //interate through IQ
                current_age = IQ[j].IQ_instructions.age;  //get the age of the current inst. in IQ
                if(current_age == oldest){ //checks to find the oldest instruction
                    if(set_max < width) { //max number of instructions per cycle
                        if(IQ[j].IQ_valid == 1 && IQ[j].IQ_src1_ready ==1 && IQ[j].IQ_src2_ready == 1){
                            open_execution_index = free_EX_spot(execution_list_size); //find free spot in execution
                            IQ[j].IQ_instructions.IS_exit = totalCycle + 1;
                            IQ[j].IQ_instructions.EX_entered = totalCycle + 1;
                            execute_list[open_execution_index] = IQ[j].IQ_instructions;//copy over instruction class to ex
                            execute_list[open_execution_index].execute_valid = 1; //make valid once added to execute
                            IQ[j].IQ_valid = 0; //IQ_clear(j);
                            set_max++; //decrement IQ counter when its pussed to ex /IQ_counter--; //decrement IQ counter
                        }
                    }
                }
            }
        oldest++; //increment min value
        }
}

void pipeline::Execute(){
    unsigned long int size = width*5;
    int rob_tag;
    // decrement cycles
    for(unsigned long int y = 0; y < size; y++){
        if(execute_list[y].execute_valid) {
            execute_list[y].ins_cycle--;
        }
    }
    
    for(unsigned long int y = 0; y < size; y++){
        if(execute_list[y].execute_valid) { //check only valid index of execute_list
            if(execute_list[y].ins_cycle == 0) {
                
                if(execute_list[y].dest != -1) {  //if rob tag is not -1 then broadcast destination
                    rob_tag = execute_list[y].dest; //gets the value of destination
                    wakeup(rob_tag, iq_size, width); //wakeup functions
                    
                }
                
                execute_list[y].EX_exit = totalCycle + 1; //exicute exited
                execute_list[y].WB_entered = totalCycle + 1; //WB entered
                execute_list[y].execute_valid = 0; //set flag to 0 once writeback is initiated
                WB.push_back(execute_list[y]); //push to writeback
            }
        }
    }
}


void pipeline::Writeback(){
    if(WB.size() != 0){
        for(unsigned long int y = 0; y < WB.size(); y++){ //interate through WB
            for(unsigned long int z = 0; z < rob_size; z++) { //interate through rob
                if(WB[y].pc == ROB[z].ROB_pc && WB[y].age == ROB[z].ROB_instructions.age){ //only is PC and ages match
                    ROB[z].ROB_ready = 1; //set ROB to eady
                    WB[y].WB_exit = totalCycle + 1; //Writeback exited
                    WB[y].RT_entered = totalCycle + 1; //WB entered
                    ROB[z].ROB_instructions = WB[y]; //cop over to rob
                }
            }
        }
        WB.clear(); //clear after all writebacks done
    }
}

//this function compares Rob_tags from ROB to RMT and sets them to 0 if true
void pipeline::comp_RMT_to_ROB(unsigned long int RobTag){
    for(int y = 0; y < 67; y++){ //interate through RMT
        if(RMT[y].valid == 1){ //only check RMT valid Tags
            if(RMT[y].ROB_tag == RobTag){ //if the RMT_tag == ROB's valid tag
                RMT[y].valid = 0; //Make RMT.valid tag invaldi
            }
        }
    }
}

//This function increments the head and writesback to ARF and compares RMT
void pipeline::Retire(){
    unsigned long int ins_retire_max = 0;
    for(unsigned long int l = 0; l < rob_size; l++){
        if(ins_retire_max < width){
            if(ROB[ROB_head].ROB_ready == 1){ //if Head of rob is valid start comiting to ARF
                //check in RMT for valid tags
                if(ROB[ROB_head].ROB_dest > -1){
                    comp_RMT_to_ROB(ROB_head); //ROB[ROB_head].ROB_dest
                }
                ROB[ROB_head].ROB_instructions.RT_exit = totalCycle + 1;
                print_from_retire(ROB_head);
                wakeup(ROB[ROB_head].ROB_instructions.dest, iq_size, width);
                if(ROB[ROB_head].ROB_instructions.age == age_counter-1){
                    function_end = 1; //ROB is empty so end the file
                }
                ROB[ROB_head] = ROB_type(0); //clear ROB to 0
                ROB_head++; //increment head
                ROB_counter--; //decrement rob counter to check for full
                
            }
            if(ROB_head == rob_size){ //if rob reaches max
                ROB_head = 0;        //reset ROB_Head back to ROB 0;
            }
            ins_retire_max++; //max of width instructions that can retire/
        }
        
    }
    
}

//this just prints all the timing of each instruction in each stage
void pipeline::print_from_retire(unsigned long int ROB_index_to_print){
    instructions temp = ROB[ROB_index_to_print].ROB_instructions;
    printf("%d f{%d} src{%d,%d} dst{%d} FE{%d,%d} DE{%d,%d} RN{%d,%d} RR{%d,%d} DI{%d,%d} IS{%d,%d} EX{%d,%d} WB{%d,%d} RT{%d,%d}\n",
           temp.age, //age
           temp.ins_op_type, //op_type
           temp.orig_src1, temp.orig_src2, //sources
           ROB[ROB_index_to_print].ROB_dest, //destination
           temp.FE_entered, //FE entered stage
           temp.FE_exit - temp.FE_entered, //duration for FE_exit
           temp.FE_exit, //DE_entered
           temp.DE_exit - temp.DE_entered, //DE_duration    temp.RN_entered - temp.DE_entered
           temp.DE_exit,//RN_entered
           temp.RN_exit - temp.RN_entered, //RN_duration
           temp.RN_exit,
           temp.RR_exit - temp.RR_entered, //RR_duration
           temp.RR_exit,
           temp.DI_exit - temp.DI_entered, //DI_duration
           temp.DI_exit,
           temp.IS_exit - temp.IS_entered, //IS_duration
           temp.IS_exit,
           temp.EX_exit - temp.EX_entered, //EX_duration
           temp.EX_exit, //WB_entered
           temp.WB_exit - temp.WB_entered, //WB_duration
           temp.WB_exit, //RT_entered
           temp.RT_exit - temp.RT_entered); //RT_duration
}

//print the final results
void pipeline::print_results(){
    printf("# Dynamic Instruction Count    = %d\n", age_counter);
    printf("# Cycles                       = %d\n", totalCycle);
    printf("# Instructions Per Cycle (IPC) = %.2f\n", float(age_counter)/(totalCycle));
}

#endif
