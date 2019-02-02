#ifndef GDB_H
#define GDB_H

#ifdef DEBUG

#define BUFMAX 400

static char initialized; /* boolean flag. != 0 means we've been initialized */

int remote_debug;

void _returnFromException();

static char remcomInBuffer[BUFMAX];
static char remcomOutBuffer[BUFMAX];

unsigned char *getpacket(void);
void putpacket(unsigned char *buffer);

static void (*volatile mem_fault_routine)();

static volatile int mem_err;

void set_mem_err(void);

int get_char(char *addr);
void set_char(char *addr, int val);

int computeSignal(int exceptionVector);

int hexToInt(char **ptr, int *intValue);

void handle_exception(int exceptionVector);
void set_debug_traps(void);

void breakpoint(void);

#endif
#endif
