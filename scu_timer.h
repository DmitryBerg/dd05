#ifndef SCUTIMER_H
#define SCUTIMER_H

#define SCU_TIMER_PERIOD 300000000

void set_scutimer(void);
int scutime(void);
int scutimer_tick(void);

void setio(void);

int scutime(void);

void scutimer_sleep(int t_ms);
#endif
