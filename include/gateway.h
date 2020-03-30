#pragma once

#include "variant.h"
#include <due_can.h>
#include <string.h>

#include "cdhd.h"
#include "elrest.h"
#include "dSpace.h"


class Gateway
{ 
public:
    Gateway();

    CDHD_Drive servo = {1}; // Create an object of CDHD_Drive

    // canopen
    bool Gateway::fetch_states();
    CAN_FRAME process_canopen(CAN_FRAME &f);


    CAN_FRAME process_can(CAN_FRAME &f);
   
};
