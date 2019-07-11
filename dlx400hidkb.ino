
// Mapping Connector vs teensy's pins
int KB_PIN1 = 23; // Feedback line (input)
int KB_PIN2 = 22; // Feedback line (input)
int KB_PIN3 = 21; // Feedback line (input)
int KB_PIN4 = 20; // Feedback line (input)
int KB_PIN5 = 19; // Feedback line (input)
int KB_PIN6 = 18; // Feedback line (input)
int KB_PIN7 = 17; // Feedback line (input)
int KB_PIN8 = 16; // GND LED
int KB_PIN9 = 15; // PWR LED
int KB_PIN10 = 14; // Sanning line (output)
int KB_PIN11 = 13; // Sanning line (output)
int KB_PIN12 = 1; // Sanning line (output)
int KB_PIN13 = 2; // Sanning line (output)
int KB_PIN14 = 3; // Sanning line (output)
int KB_PIN15 = 4; // Sanning line (output)
int KB_PIN16 = 5; // Feedback line (input)
int KB_PIN17 = 6; // Sanning line (output)

// pins setup
int SCANNING_PINS[] = {KB_PIN10, KB_PIN11, KB_PIN12, KB_PIN13, KB_PIN14, KB_PIN15, KB_PIN17};
int FEEDBACK_PINS[] = {KB_PIN1, KB_PIN2, KB_PIN3, KB_PIN4, KB_PIN5, KB_PIN6, KB_PIN7, KB_PIN16};
int POWER_LED_PIN = KB_PIN9;
int GND_LED_PIN = KB_PIN8;

int key_pressed = 0;
int key_pressed_arr[5];

// struct used for all keys
struct key
{
   int key_code;
   int feed_pin;
   int scan_pin;
};

// Keys, per scanning line
// note: caps lock was remaped to esc and esc to tilde
struct key keys_10[] = {{KEY_K, KB_PIN1, KB_PIN10}, {KEY_U, KB_PIN2, KB_PIN10}, {KEY_7, KB_PIN3, KB_PIN10}, {KEY_TILDE, KB_PIN4, KB_PIN10}, {KEY_TAB, KB_PIN5, KB_PIN10}, {KEY_ESC, KB_PIN6, KB_PIN10}, {MODIFIERKEY_SHIFT, KB_PIN7, KB_PIN10}, {MODIFIERKEY_RIGHT_SHIFT, KB_PIN16, KB_PIN10}};
struct key keys_11[] = {{KEY_L, KB_PIN1, KB_PIN11}, {KEY_I, KB_PIN2, KB_PIN11}, {KEY_8, KB_PIN3, KB_PIN11}, {KEY_1, KB_PIN4, KB_PIN11}, {KEY_Q, KB_PIN5, KB_PIN11}, {KEY_A, KB_PIN6, KB_PIN11}, {KEY_Z, KB_PIN7, KB_PIN11}, {KEY_SLASH, KB_PIN16, KB_PIN11}};
struct key keys_12[] = {{KEY_SEMICOLON, KB_PIN1, KB_PIN12}, {KEY_O, KB_PIN2, KB_PIN12}, {KEY_9, KB_PIN3, KB_PIN12}, {KEY_2, KB_PIN4, KB_PIN12}, {KEY_W, KB_PIN5, KB_PIN12}, {KEY_S, KB_PIN6, KB_PIN12}, {KEY_X, KB_PIN7, KB_PIN12}, {MODIFIERKEY_CTRL, KB_PIN16, KB_PIN12}};
struct key keys_13[] = {{KEY_QUOTE, KB_PIN1, KB_PIN13}, {KEY_P, KB_PIN2, KB_PIN13}, {KEY_0, KB_PIN3, KB_PIN13}, {KEY_3, KB_PIN4, KB_PIN13}, {KEY_E, KB_PIN5, KB_PIN13}, {KEY_D, KB_PIN6, KB_PIN13}, {KEY_C, KB_PIN7, KB_PIN13}, {MODIFIERKEY_ALT, KB_PIN16, KB_PIN13}};
struct key keys_14[] = {{KEY_M, KB_PIN1, KB_PIN14}, {KEY_BACKSLASH, KB_PIN2, KB_PIN14}, {KEY_MINUS, KB_PIN3, KB_PIN14}, {KEY_4, KB_PIN4, KB_PIN14}, {KEY_R, KB_PIN5, KB_PIN14}, {KEY_F, KB_PIN6, KB_PIN14}, {KEY_V, KB_PIN7, KB_PIN14}, {KEY_SPACE, KB_PIN16, KB_PIN14}};
struct key keys_15[] = {{KEY_COMMA, KB_PIN1, KB_PIN15}, {KEY_ENTER, KB_PIN2, KB_PIN15}, {KEY_EQUAL, KB_PIN3, KB_PIN15}, {KEY_5, KB_PIN4, KB_PIN15}, {KEY_T, KB_PIN5, KB_PIN15}, {KEY_G, KB_PIN6, KB_PIN15}, {KEY_B, KB_PIN7, KB_PIN15}, {MODIFIERKEY_RIGHT_ALT, KB_PIN16, KB_PIN15}};
struct key keys_17[] = {{KEY_PERIOD, KB_PIN1, KB_PIN17}, {KEY_J, KB_PIN2, KB_PIN17}, {KEY_BACKSPACE, KB_PIN3, KB_PIN17}, {KEY_6, KB_PIN4, KB_PIN17}, {KEY_Y, KB_PIN5, KB_PIN17}, {KEY_H, KB_PIN6, KB_PIN17}, {KEY_N, KB_PIN7, KB_PIN17}};

void setup() {

    for (int i = 0; i < sizeof(SCANNING_PINS) / sizeof(SCANNING_PINS[0]); i++) {
        pinMode(SCANNING_PINS[i], OUTPUT);
    }

    for (int i = 0; i < sizeof(FEEDBACK_PINS) / sizeof(FEEDBACK_PINS[0]); i++) {
        pinMode(FEEDBACK_PINS[i], INPUT_PULLDOWN);
    }
}
void loop() {
    // set each output pins to HIGH one after the others
    for (int i = 0; i < sizeof(SCANNING_PINS) / sizeof(SCANNING_PINS[0]); i++) {
        digitalWrite(SCANNING_PINS[i], HIGH);
        for (int j = 0; j < sizeof(FEEDBACK_PINS) / sizeof(FEEDBACK_PINS[0]); j++) {
            // Check if a feedback pin is HIGH
            if(digitalRead(FEEDBACK_PINS[j])) {
                // TODO, send key to host
                press_key(get_key(SCANNING_PINS[i], FEEDBACK_PINS[j]));
            }
            else {
                // If key is pressed release
                release_key(get_key(SCANNING_PINS[i], FEEDBACK_PINS[j]));
            }
        }
        digitalWrite(SCANNING_PINS[i], LOW);
    }
    delay(10);
}

// returns the key pressed, given a scanning and feedback pin
int get_key(int scanning_pin, int feedback_pin) {
    if (scanning_pin == KB_PIN10){
        return get_feed_key(keys_10, 8, feedback_pin);
    }
    else if (scanning_pin == KB_PIN11){
        return get_feed_key(keys_11, 8, feedback_pin);
    }
    else if (scanning_pin == KB_PIN12){
        return get_feed_key(keys_12, 8, feedback_pin);
    }
    else if (scanning_pin == KB_PIN13){
        return get_feed_key(keys_13, 8, feedback_pin);
    }
    else if (scanning_pin == KB_PIN14){
        return get_feed_key(keys_14, 8, feedback_pin);
    }
    else if (scanning_pin == KB_PIN15){
        return get_feed_key(keys_15, 8, feedback_pin);
    }
    else if (scanning_pin == KB_PIN17){
        return get_feed_key(keys_17, 7, feedback_pin);
    }
}

// Search an array of key struct and return the key at the specified feedback pin
int get_feed_key(key key_array[], int array_size, int feedback_pin) {
    for (int i = 0; i < array_size; i++) {
        if (key_array[i].feed_pin == feedback_pin) {
            return key_array[i].key_code;}
    }
}

// Removed the key at specified position from key pressed array
void remove_from_key_pressed(int position) {
    for (int i = 0 - 1; i < key_pressed - 1; i++) {
         key_pressed_arr[i] = key_pressed_arr[i+1];
    }
    key_pressed--;
}

// send keyboard press event to host for specified key
void press_key(int key) {
    if (key_pressed < 5) {
    for (int i = 0; i < key_pressed; i++) {
        if (key == key_pressed_arr[i]) {
            return;
        }
    }
    Keyboard.press(key);
    key_pressed_arr[key_pressed++] = key;
    }
}

// send keyboard release event to host for specified key
void release_key(int key) {
    for (int i = 0; i < key_pressed; i++) {
        if (key == key_pressed_arr[i]) {
            Keyboard.release(key);
            remove_from_key_pressed(i);
        }
    }

}
