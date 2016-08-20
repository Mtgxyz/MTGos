#pragma once
namespace MTGos {
enum class IntType {
    QUIT, //Quit from Keyboard
    ILL, //Illegal Instruction
    TRAP, // Breakpoint
    ABRT, //Abort
    BUS, //Data Abort
    FPE, //Division by zero et al
    KILL, //Shutdown 
    SEGV, //Segmentation fault
    CONT, //Clock signal (called a multiple of 60 or 50Hz)
    SYS //Syscall
};

}