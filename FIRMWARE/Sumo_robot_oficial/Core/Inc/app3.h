#ifndef APP3_H_
#define APP3_H_

uint32_t hw_timer_get_tick_ms(void);
void survive(void);
void hunt(void);
void target(void);
void attack(void);
void clearCommands(void);
void startExecutingCommands(void);
void addCommand(int left, int right, int duration);
void executeCommands(void);

void app3_init(void);
void app3_loop(void);

#endif /* APP3_H_ */
