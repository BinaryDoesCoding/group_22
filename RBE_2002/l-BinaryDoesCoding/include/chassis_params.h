/**
 * WARNING: THE DEFAULTS ARE GROSSLY WRONG!!!
 */

/**
 * TODO: Set to reasonable values
 */

const float ROBOT_RADIUS = 6.7f;   // cm (112 mm wheel separation / 2)
const float LEFT_TICKS_PER_CM  = 67.0f;
const float RIGHT_TICKS_PER_CM = 67.0f;
const float DISTANCE_PER_ENCODER_TICK = (7.0f * PI) / 1440.0f;   // cm (7 cm wheel diameter, 360 ticks per revolution)