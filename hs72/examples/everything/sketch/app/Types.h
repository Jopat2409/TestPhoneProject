/*
Custom enums and types
*/
#ifndef _TYPES_H
#define _TYPES_H

enum button_t{
    b_one = 0,
    b_two = 1,
    b_three = 2
};

enum b_status_t{
    s_pressed = 0,
    s_held = 1,
    s_released = 2
};

enum page_t{
    p_menu,
    p_balance,
    p_paymerchant,
    p_payuser
};



#endif
