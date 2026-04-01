#include <ir_codes.h>
#include <IRdecoder.h>
#include "robot.h"

/**
 * robot_remote.cpp contains the code for handling IR remote keypresses.
 * It also contains the global declaration of the IRDecoder, which is needed
 * for the ISR. Note that the decoder is declared extern in IRdecoder.h,
 * so we must name it decoder here.
 */

String keyString;

/**
 * IRDecoder decoder is declared extern in IRdecoder.h (for ISR purposes),
 * so we must name it decoder here.
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
        case ENTER_SAVE:
            // kill switch / stop
            EnterIdleState();
            keyString = "";
            Serial.println("Robot -> IDLE");
            break;

        case NUM_1:
            // start alignment mode
            EnterAligningState();
            keyString = "";
            Serial.println("Robot -> ALIGNING");
            break;

        case UP_ARROW:
            // start standoff mode
            EnterStandoffState();
            keyString = "";
            Serial.println("Robot -> STANDOFF");
            break;

        case LEFT_ARROW:
            // manual turn test
            chassis.SetTwist(0.0f, 0.785f);
            keyString = "";
            Serial.println("Manual turn test");
            break;

        case NUM_2:
            // use Sharp IR
            SetSensorMode(SENSOR_SHARP_IR);
            keyString = "";
            Serial.println("Sensor -> Sharp IR");
            break;

        case NUM_3:
            // use ultrasonic
            SetSensorMode(SENSOR_ULTRASONIC);
            keyString = "";
            Serial.println("Sensor -> Ultrasonic");
            break;

        case REWIND:
            // start line following using entered number
            EnterLineFollowingState(keyString.toInt());
            keyString = "";
            Serial.println("Robot -> LINE FOLLOW");
            break;

        case NUM_4:
        case NUM_5:
        case NUM_6:
        case NUM_7:
        case NUM_8:
        case NUM_9:
        case NUM_0_10:
            // build numeric input string
            if (keyCode == NUM_0_10)
            {
                keyString += '0';
            }
            else if (keyCode >= NUM_4 && keyCode <= NUM_6)
            {
                keyString += (char)(keyCode + 32);
            }
            else if (keyCode >= NUM_7 && keyCode <= NUM_9)
            {
                keyString += (char)(keyCode + 31);
            }
            Serial.print("keyString: ");
            Serial.println(keyString);
            break;

        default:
            break;
    }
}