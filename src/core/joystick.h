#ifndef JOYSTICK_H
#define JOYSTICK_H

/* threshold to distinguish between real joystick movement and noise */
#define JOY_THRESHOLD 100

/* prepare for up to 6 axes and 16 buttons per joystick */
enum joystick_actuator {
       JOY_AXIS0_POS,
       JOY_AXIS0_NEG,
       JOY_AXIS1_POS,
       JOY_AXIS1_NEG,
       JOY_AXIS2_POS,
       JOY_AXIS2_NEG,
       JOY_AXIS3_POS,
       JOY_AXIS3_NEG,
       JOY_AXIS4_POS,
       JOY_AXIS4_NEG,
       JOY_AXIS5_POS,
       JOY_AXIS5_NEG,
       JOY_BUTTON0,
       JOY_BUTTON1,
       JOY_BUTTON2,
       JOY_BUTTON3,
       JOY_BUTTON4,
       JOY_BUTTON5,
       JOY_BUTTON6,
       JOY_BUTTON7,
       JOY_BUTTON8,
       JOY_BUTTON9,
       JOY_BUTTON10,
       JOY_BUTTON11,
       JOY_BUTTON12,
       JOY_BUTTON13,
       JOY_BUTTON14,
       JOY_BUTTON15
};

/* initialize joystick subsystem */
void joystick_setup(void);

/* assign a joystick to a player */
int open_joystick(int joy_no);

/* close a joystick */
void close_joystick(int joy_no);

/* close all joysticks */
void close_joysticks(void);

/* update joystick states */
void update_joysticks(void);

/* query status of joystick */
int joy_active(int joy_no, int actuator, int threshold);

/* stop joystick subsystem */
void joystick_stop(void);

#endif
