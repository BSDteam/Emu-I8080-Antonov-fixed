#ifndef OUTPUT_H
#define OUTPUT_H

extern void put_inst(void);
extern void show_prog(void);
extern void show_regis(void);
extern void show_virtual_screen(void);

extern int find_1(void);
extern int find_2(void);
extern int find_3(void);

extern void put_hex4(unsigned int dig);
extern void put_hex2(unsigned int dig);

#endif // OUTPUT_H
