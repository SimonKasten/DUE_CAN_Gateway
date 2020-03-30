#pragma once

#include "variant.h"
#include <due_can.h>
#include <string.h>

#define NMT_OP 0x01
#define NMT_STOP 0x02
#define NMT_PRE_OP 0x80
#define NMT_RESET_NODE 0x81
#define NMT_RESET_COMM 0x82

// NMT
#define STATE_BOOTUP 0x00
#define STATE_STOPPED 0x04
#define STATE_OP 0x05
#define STATE_PRE_OP 0x7F

// Controlword
#define SWITCH_ON 0x06
#define ENABLE    0x07
#define OP_START  0x0F
#define POS_START 0x3F
#define CLR_FAULT 0x80

// OPModes
#define OP_VELO 3
#define OP_TORQ 4
#define OP_POS  1

#define RELAIS 5

class CDHD_Drive
{ 
private:
    CAN_FRAME send_Msg;
    CAN_FRAME init_Msg;
    CAN_FRAME r1, r2, r3, r4;
    char buff[10];

    const float const_scale_num = 262144.0;
    const float const_scale_velo = (const_scale_num / 60.0);
    const float const_scale_torq = 100.0;

public:
    CDHD_Drive(uint32_t _id);

    ~CDHD_Drive();


    uint32_t id; 

    
    struct TPDO1 {
        uint16_t statusword = 0;
        int8_t opmode_display = 0;
        int16_t ist_torq = 0;
    } tpdo1;
    struct TPDO2 {
        int32_t ist_pos = 0;
        int32_t ist_rpm = 0;
    } tpdo2;
    struct TPDO3 {
        int16_t ist_curr = 0;
        int16_t soll_torq = 0;
        int16_t analog1 = 0;
        int16_t analog2 = 0;
    } tpdo3;
    struct TPDO4 {
        uint32_t dig_in = 0;
        int32_t pos_ext = 0;
    } tpdo4;

    struct RPDO1 {
        uint16_t controlword = 0;
        int8_t opmode = 0;
    } rpdo1;
    struct RPDO2 {
        int32_t target_position = 0;
        int32_t profile_velocity = 0;
    } rpdo2;
    struct RPDO3 {
        int32_t target_velocity = 0;
    } rpdo3;
    struct RPDO4 {
        int16_t target_torque = 0;
    } rpdo4;



    void update_tpdos(CAN_FRAME &f);
    void update_rpdos();
    void send_rpdos();



    uint16_t state; 
    void set_nmt_state(uint16_t state);
    void update_nmt_state(CAN_FRAME &f);


    bool fetch_state();





    void motorInit();
    void sdoInit();
    void relais();


    void motorstop();
    void quit();

    void positionMode(int32_t sollPosition, int32_t maxPosDrehzahl);
    void solldrehmoment(int32_t sollTorque);
    void torqueMode(int32_t sollTorque, int32_t maxDrehzahl);
    void solldrehzahl(int32_t sollDrehzahl);
    void drehzahlMode(int32_t sollDrehzahl, int32_t maxTorque);

    void sendPDO(uint32_t s_id, uint32_t s_dlc, uint32_t s_data1, uint32_t s_data2);
    void sendSDO(uint32_t s_id, uint32_t s_cb, uint32_t s_od, uint32_t s_sub, uint32_t s_data);
    void recieveSDO(CAN_FRAME &frame);
    void recievePDO(CAN_FRAME &frame);

};
