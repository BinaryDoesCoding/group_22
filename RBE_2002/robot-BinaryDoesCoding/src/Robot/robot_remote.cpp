#include <ir_codes.h>
#include <IRdecoder.h>
#include "robot.h"

/**
 * robot_remote.cpp contains the code for handling IR remote keypresses.
 * It also contains the global declaration of the IRDecoder, which is needed
 * for the ISR. Note that the decoder is declared extern in IRdecoder.h,
 * so we _must_ name it decoder here.
 */

String keyString;

/**
 * IRDecoder decoder is declared extern in IRdecoder.h (for ISR purposes),
 * so we _must_ name it decoder here.
 */
#define IR_PIN 17
IRDecoder decoder(IR_PIN);

void Robot::InitializeDecoder(void)
{
    decoder.init();
    keyString.reserve(20);
}

void Robot::HandleKeyCode(int16_t keyCode)
{
    Serial.print("key: ");
    Serial.println(keyCode);

    switch (keyCode)
    {
        case ENTER_SAVE:   // kill switch / stop
            EnterIdleState();
            keyString = "";
            break;

        case NUM_1:        // start stand-off mode
            //EnterStandoffState();
            //keyString = "";
            break;

        case UP_ARROW:     // manual forward test
            EnterStandoffState();
            keyString = "";;   // 10 cm/sec
            break;

        case LEFT_ARROW:   // manual turn test
            chassis.SetTwist(0, 0.785);   // 45 deg/sec
            break;

        case REWIND:
            EnterLineFollowingState(keyString.toInt());
            keyString = "";
            break;

        case NUM_2:
        case NUM_3:
            keyString += (char)(keyCode + 33);
            break;

        case NUM_4:
        case NUM_5:
        case NUM_6:
            keyString += (char)(keyCode + 32);
            break;

        case NUM_7:
        case NUM_8:
        case NUM_9:
            keyString += (char)(keyCode + 31);
            break;

        case NUM_0_10:
            keyString += '0';
            break;
    }
}