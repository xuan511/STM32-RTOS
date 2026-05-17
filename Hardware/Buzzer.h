#define __BUZZER_H
#define __BUZZER_H
#include "main.h"
#include "cmsis_os.h"

void Buzzer_Init(void);
void Buzzer_On(void);
void Buzzer_Off(void);
void Buzzer_StartAlarm(void);
void Buzzer_StopAlarm(void);

#endif