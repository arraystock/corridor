#ifndef SERIAL_H
#define SERIAL_H

void serial_initialize();
int serial_received();
char serial_read();
int is_transmit_empty();
void serial_write(char c);
void serial_print(const char *str);

#endif
