#include "gateway.h"



Gateway::Gateway(){
}

bool Gateway::fetch_states(){
    return servo.fetch_state(); // && alle weiteren Devices
}



CAN_FRAME Gateway::process_canopen(CAN_FRAME &f){
    if ( f.id > 0x180 && f.id < 0x580) {
        servo.update_tpdos(f);
    }

    return f;
}

CAN_FRAME Gateway::process_can(CAN_FRAME &f){


    return f;
}
