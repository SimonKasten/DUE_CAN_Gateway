
#include "main.h"
#include <DueTimer.h>


using namespace std;

uint32_t timer1;
uint32_t timer2;

float mVelocity;
float mTorque;
float mPosition;


Gateway gateway;

CAN_FRAME sync_msg;

CAN_FRAME can0_req;
CAN_FRAME can0_res;
CAN_FRAME can1_req;
CAN_FRAME can1_res;



void canopen_sync(){
    Can0.sendFrame(sync_msg);
}
void device_states(){
    gateway.fetch_states();
}

void setup()
{ 
    pinMode(LED_SHIELD, OUTPUT);
    digitalWrite(LED_SHIELD, HIGH);

    Serial.begin(115200);

    init_canbus();


	Timer3.attachInterrupt(canopen_sync).start(500000);  // Calls every 500ms
    Timer4.attachInterrupt(device_states).start(500000);

    Serial.println("------- DUE-CAN-SETUP --------");

    gateway.servo.set_nmt_state(NMT_RESET_NODE);
    delay(10); 
    gateway.servo.sdoInit();
    delay(10); 
    gateway.servo.set_nmt_state(NMT_OP);
    delay(10); 
    gateway.servo.motorInit();
    delay(10); 
    gateway.servo.relais();
}


void loop()
{
    // CANOPEN
    while (Can0.available())
    { 
        digitalWrite(LED_SHIELD, HIGH);
        Can0.read(can0_req);

        can0_res = gateway.process_canopen(can0_req);

        Serial.println(can0_res.id);

        //Can1.sendFrame(can0_res);
    }

    // CAN
    while (Can1.available())
    { 
        digitalWrite(LED_SHIELD, HIGH);
        Can1.read(can1_req);

        can1_res = gateway.process_can(can1_req);

        Serial.println(can1_res.id);
        //Can1.sendFrame(can1_res);
    }


    if(digitalRead(LED_SHIELD) == HIGH)
        digitalWrite(LED_SHIELD, LOW);



    // servo.checkForCANMsg(&mVelocity, &mTorque, &mPosition);

    // digitalWrite(LED_SHIELD, HIGH);

    // while (Serial.available())
    // {
    //   char c = Serial.read();
    //   Serial.print("Eingabe: ");
    //   Serial.print(c);
    //   Serial.print("    ");

    //   if (c == 'd')
    //     servo.drehzahlMode(0, 0);
    //   if (c == 'z')
    //     servo.motorstop();
    //   if (c == 'q')
    //     servo.quit();
    //   if (c == 'l')
    //   {
    //     uint16_t testv = Serial.parseInt();
    //     Serial.println(testv);

    //     servo.solldrehzahl(testv);
    //   }
    //   if (c == 'v')
    //   {
    //     uint16_t testv = Serial.parseInt();
    //     Serial.println(testv);
    //     servo.solldrehmoment(testv);
    //   }
    //   if (c == 'p')
    //     servo.positionMode(2, 2);

    //   Serial.println();
    //   while (Serial.available())
    //     Serial.read();
    // }


    // char bufff[13];
    // if (millis() - timer2 > 500)
    // {
    //   Serial.print("velocity: ");
    //   dtostrf(mVelocity, 8, 3, bufff);
    //   Serial.print(bufff);

    //   Serial.print("  |  ");
    //   Serial.print("torque: ");
    //   Serial.print("  ");
    //   dtostrf(mTorque, 2, 2, bufff);
    //   Serial.print(bufff);

    //   Serial.print("  |  ");
    //   Serial.print("pos: ");
    //   Serial.print("  ");
    //   dtostrf(mPosition, 8, 3, bufff);
    //   Serial.println(bufff);
    //   timer2 = millis();
    // }
}

// void pos_geben()
// {
//   Serial.print("\n\nBitte Pos eingeben:  ");
//   while (Serial.available())
//     Serial.read();
//   while (!Serial.available())
//     ;
//   int16_t pos = Serial.parseInt();
//   int32_t pos1 = int32_t(pos);
//   Serial.println(pos1, HEX);

//   Serial.print("Bitte Vmax eingeben: ");
//   while (Serial.available())
//     Serial.read();
//   while (!Serial.available())
//     ;
//   int16_t vmax = Serial.parseInt();
//   int32_t vmax1 = int32_t(vmax);
//   Serial.println(vmax1, HEX);

//   servo.sendPDO(0x302, 0x8, pos1 * int32_t(servo.const_scale_num), vmax1 * int32_t(servo.const_scale_velo));
//   servo.heartbeat();
// }





void init_canbus(){
    sync_msg.id = 0x80;
    sync_msg.length = 0;
    sync_msg.data.byte[0] = 0;


    Can0.setRxBufferSize(128);
    Can0.setTxBufferSize(32);
    
    Can1.setRxBufferSize(128);
    Can1.setTxBufferSize(32);

    Can0.begin(CAN_BPS_500K);
    Can1.begin(CAN_BPS_500K);

    int filter;
    //extended
    for (filter = 0; filter < 3; filter++)
    {
        Can0.setRXFilter(filter, 0, 0, true);
        Can1.setRXFilter(filter, 0, 0, true);
    }
    //standard
    for (filter = 3; filter < 7; filter++)
    {
        Can0.setRXFilter(filter, 0, 0, false);
        Can1.setRXFilter(filter, 0, 0, false);
    }
}