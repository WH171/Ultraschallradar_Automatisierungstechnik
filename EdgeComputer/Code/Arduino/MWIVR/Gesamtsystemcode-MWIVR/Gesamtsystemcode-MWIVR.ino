/**
 * @file Gesamtsystemcode-MWIVR.ino
 * @brief Edge Impulse ingestion SDK example customized for keyword recognition.
 *
 * This standalone implementation uses PDM microphone input with LED and OLED
 * feedback for detected keywords. It avoids reliance on Serial Monitor and is
 * designed for embedded use with real-time inference.
 */

#include <PDM.h> // Required for Pulse Density Modulation (PDM) microphone
#include <Wire.h> // Required for I2C communication with the OLED display
#include <Adafruit_SSD1306.h> // Required for controlling the SSD1306 OLED display
#include <MWIVR_inferencing.h> // Edge Impulse generated header for keyword spotting model

/**
 * @brief Namespace containing all configuration parameters for the application.
 *
 */
namespace Config {
    // Display settings
    const uint8_t SCREEN_WIDTH = 128; /< Width of the OLED display in pixels. */
    const uint8_t SCREEN_HEIGHT = 64; /< Height of the OLED display in pixels. */
    const int8_t OLED_RESET = -1; /< OLED reset pin (-1 indicates no dedicated reset pin). */
    const uint8_t OLED_ADDRESS = 0x3C; /< I2C address of the OLED display. */
    
    // LED Pins
    const uint8_t RED_LED_PIN = 11; /< Digital pin connected to the red LED. */
    const uint8_t GREEN_LED_PIN = 12; /< Digital pin connected to the green LED. */
    
    // Audio Einstellungen
    const int AUDIO_ACTIVITY_THRESHOLD = 300; /< Amplitude threshold for detecting audio activity. */
    const float CONFIDENCE_THRESHOLD = 0.85f; /< Minimum confidence score required for a keyword to be considered recognized. */
    const int PDM_GAIN = 60; /< Gain setting for the PDM microphone. */
    const uint16_t PDM_BUFFER_SIZE = 4096; /< Size of the PDM microphone buffer in bytes. */
    
    // Timing Constants
    const unsigned long COMMAND_DURATION_MS = 10000;      /< Duration in milliseconds a recognized command remains active. */
    const unsigned long BLINK_INTERVAL_MS = 500;          /< Interval in milliseconds for LED blinking when 'blinken' command is active. */
    const unsigned long DISPLAY_UPDATE_INTERVAL_MS = 200; /< Interval in milliseconds for updating the OLED display during command execution. */
    const unsigned long READY_DELAY_MS = 500;             /< Delay in milliseconds after displaying "bereit" (ready) message. */
    const unsigned long ERROR_DELAY_MS = 1000;            /< Delay in milliseconds after displaying an error message. */
    const unsigned long WELCOME_DELAY_MS = 5000;          /< Duration in milliseconds the welcome message is displayed. */
    const unsigned long REPEAT_MESSAGE_DELAY_MS = 1000;   /< Duration in milliseconds the "bitte wiederholen" message is displayed. */
    const unsigned long INFERENCE_TIMEOUT_MS = 5000;      /< Timeout in milliseconds for audio inference to complete. */
    
    // Valid and ignored labels
    const char* VALID_LABELS[] = { "rot", "gruen", "beide", "blinken" }; /< Array of recognized valid command labels. */
    const char* IGNORED_LABELS[] = { "noise", "background", "Noise" }; /< Array of labels to be ignored (e.g., background noise). */
    const uint8_t VALID_LABELS_COUNT = sizeof(VALID_LABELS) / sizeof(VALID_LABELS[0]); /< Number of valid labels. */
    const uint8_t IGNORED_LABELS_COUNT = sizeof(IGNORED_LABELS) / sizeof(IGNORED_LABELS[0]); /< Number of ignored labels. */
}

Adafruit_SSD1306 display(Config::SCREEN_WIDTH, Config::SCREEN_HEIGHT, &Wire, Config::OLED_RESET); /< OLED display object. */

/**
 * @brief Structure to hold audio inference-related buffers and state.
 */
typedef struct {
    int16_t buffer; /*< Pointer to the audio sample buffer. */
    uint8_t buf_ready; /< Flag indicating if the buffer is ready for inference (1 = ready, 0 = not ready). */
    uint32_t buf_count; /< Current number of samples in the buffer. */
    uint32_t n_samples; /< Total number of samples required for inference. */
    bool is_initialized; /< Flag indicating if the inference buffer has been initialized. */
} inference_t;

static inference_t inference = {nullptr, 0, 0, 0, false}; /< Global instance of the inference structure. */
static signed short sampleBuffer[2048]; /< Buffer for raw PDM audio samples. */
static bool debug_nn = false; /< Flag for enabling neural network debugging. */
static bool command_active = false; /< Flag indicating if a command is currently active. */
static unsigned long command_end_time = 0; /< Timestamp when the current command should end. */
static const char* top_label_global = ""; /< Stores the label of the most recently recognized keyword. */

/**
 * @brief Safely frees the memory allocated for the inference buffer.
 *
 * Prevents double-free issues and sets the buffer pointer to nullptr.
 */
void safe_free_inference_buffer() {
    if (inference.buffer != nullptr) {
        free(inference.buffer);
        inference.buffer = nullptr;
        inference.is_initialized = false;
    }
}

/**
 * @brief Displays a message on the OLED display, dynamically adjusting text size for best fit.
 *
 * The function tries to fit the message into a single line by reducing text size
 * and centers the text both horizontally and vertically.
 * @param msg The message string to be displayed.
 */
void show_dynamic_message(const char* msg) {
    if (msg == nullptr) return; // Input validation
    
    display.clearDisplay(); // Clear the display buffer
    display.setTextColor(SSD1306_WHITE); // Set text color to white

    int16_t x1, y1; // Variables for text bounds calculation
    uint16_t w, h; // Variables for text width and height
    int current_text_size = 2; // Start with the largest text size

    // Attempt to find the largest text size that fits on one line
    while (current_text_size >= 1) {
        display.setTextSize(current_text_size); // Set current text size
        display.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h); // Get text bounds without drawing
        if (w <= Config::SCREEN_WIDTH) { // Check if text width fits screen
            break; // If it fits, optimal size was found
        }
        current_text_size--; // Otherwise, try a smaller size
    }
    
    if (current_text_size < 1) current_text_size = 1; // Ensure text size is at least 1

    // Recalculate with the final text size for accurate centering
    display.setTextSize(current_text_size); // Set the final text size
    display.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h); // Get text bounds again

    // Calculate x and y coordinates for centering
    int16_t x = (Config::SCREEN_WIDTH - w) / 2; // Horizontal centering
    int16_t y = (Config::SCREEN_HEIGHT - h) / 2; // Vertical centering

    display.setCursor(x, y); // Set the cursor to the calculated position
    display.println(msg); // Print the message to the display buffer
    display.display(); // Push the buffer content to the OLED display
}

/**
 * @brief Checks for audio activity in a given buffer based on an amplitude threshold.
 *
 * This function calculates the average absolute amplitude of the audio samples
 * and compares it against a predefined threshold to detect if sound is present.
 * @param audio_buffer Pointer to the buffer containing audio samples.
 * @param num_samples The number of samples in the audio buffer.
 * @param threshold The amplitude threshold to exceed for activity detection.
 * @return True if audio activity is detected (average absolute amplitude > threshold), false otherwise.
 */
bool check_audio_activity(int16_t* audio_buffer, uint32_t num_samples, int threshold) {
    if (audio_buffer == nullptr || num_samples == 0 || threshold < 0) {
        return false; // Invalid parameters
    }
    
    long sum_abs_samples = 0; // Accumulator for the sum of absolute sample values
    uint32_t samples_to_check = num_samples; 
    
    for (uint32_t i = 0; i < samples_to_check; i++) {
        sum_abs_samples += abs(audio_buffer[i]); // Sum up the absolute values of samples
    }
    
    long average_abs_amplitude = sum_abs_samples / samples_to_check; // Calculate the average
    return average_abs_amplitude > threshold; // Return true if average exceeds threshold
}

/**
 * @brief Checks if a given label is present in the list of valid command labels.
 * @param label The label string to check.
 * @return True if the label is a valid command, false otherwise.
 */
bool is_valid_label(const char* label) {
    if (label == nullptr) return false; // Input validation
    
    for (uint8_t i = 0; i < Config::VALID_LABELS_COUNT; i++) {
        if (strcmp(label, Config::VALID_LABELS[i]) == 0) { // Compare with each valid label
            return true; // Found a match
        }
    }
    return false; // No match found
}

/**
 * @brief Checks if a given label should be ignored (e.g., "noise", "background").
 * @param label The label string to check.
 * @return True if the label is in the ignored list, false otherwise.
 */
bool is_ignored_label(const char* label) {
    if (label == nullptr) return false; // Input validation
    
    for (uint8_t i = 0; i < Config::IGNORED_LABELS_COUNT; i++) {
        if (strcmp(label, Config::IGNORED_LABELS[i]) == 0) { // Compare with each ignored label
            return true; // Found a match
        }
    }
    return false; // No match found
}

/**
 * @brief Executes LED actions based on a recognized command.
 * @param command The recognized command string ("rot", "gruen", "beide", "blinken").
 */
void execute_led_command(const char* command) {
    if (command == nullptr) return; // Input validation
    
    if (strcmp(command, "rot") == 0) {
        digitalWrite(Config::RED_LED_PIN, HIGH); // Turn on red LED
        digitalWrite(Config::GREEN_LED_PIN, LOW); // Turn off green LED
    } else if (strcmp(command, "gruen") == 0) {
        digitalWrite(Config::RED_LED_PIN, LOW); // Turn off red LED
        digitalWrite(Config::GREEN_LED_PIN, HIGH); // Turn on green LED
    } else if (strcmp(command, "beide") == 0) {
        digitalWrite(Config::RED_LED_PIN, HIGH); // Turn on red LED
        digitalWrite(Config::GREEN_LED_PIN, HIGH); // Turn on green LED
    } else if (strcmp(command, "blinken") == 0) {
        // 'blinken' command is handled by handle_blinking() in the loop,
        // so initially, just set a state that handle_blinking() can use.
        // For now, let's turn off red and turn on green as an initial state.
        digitalWrite(Config::RED_LED_PIN, LOW);
        digitalWrite(Config::GREEN_LED_PIN, HIGH); 
    }
}

/**
 * @brief Handles non-blocking LED blinking functionality.
 *
 * This function should be called repeatedly in the main loop to achieve
 * blinking without blocking the program. It toggles the LEDs at a set interval.
 */
void handle_blinking() {
    static unsigned long last_blink_time = 0; // Static variable to store the last blink time
    if (millis() - last_blink_time > Config::BLINK_INTERVAL_MS) { // Check if blink interval has passed
        digitalWrite(Config::RED_LED_PIN, !digitalRead(Config::RED_LED_PIN)); // Toggle red LED
        digitalWrite(Config::GREEN_LED_PIN, !digitalRead(Config::GREEN_LED_PIN)); // Toggle green LED
        last_blink_time = millis(); // Update last blink time
    }
}

/**
 * @brief Turns off both red and green LEDs.
 */
void turn_off_all_leds() {
    digitalWrite(Config::RED_LED_PIN, LOW); // Turn off red LED
    digitalWrite(Config::GREEN_LED_PIN, LOW); // Turn off green LED
}

static void pdm_data_ready_inference_callback(void); /< Callback function for PDM data readiness. */
static bool microphone_inference_start(uint32_t n_samples); /< Initializes the microphone for inference. */
static bool microphone_inference_record(void); /< Records audio samples for inference. */
static int microphone_audio_signal_get_data(size_t offset, size_t length, float out_ptr); /*< Retrieves audio data for inference. */
static void microphone_inference_end(void); /< Stops the microphone and frees resources. */

/**
 * @brief PDM data ready callback function for inference.
 *
 * This function is called by the PDM library when new audio data is available.
 * It reads the available data and fills the inference buffer.
 */
static void pdm_data_ready_inference_callback(void) {
    int bytesAvailable = PDM.available(); // Get number of bytes available from PDM
    int bytesRead = PDM.read((char *)&sampleBuffer[0], bytesAvailable); // Read PDM data into sample buffer

    if (inference.buf_ready == 0) { // Only process if the inference buffer is not yet ready
        for(int i = 0; i < bytesRead>>1; i++) { // Iterate through samples (bytesRead is in bytes, >>1 for 16-bit samples)
            inference.buffer[inference.buf_count++] = sampleBuffer[i]; // Copy sample to inference buffer
            if(inference.buf_count >= inference.n_samples) { // Check if inference buffer is full
                inference.buf_count = 0; // Reset buffer count
                inference.buf_ready = 1; // Mark buffer as ready for inference
                break; // Exit loop
            }
        }
    }
}

/**
 * @brief Initializes the microphone and allocates memory for audio inference.
 * @param n_samples The total number of samples required for a single inference.
 * @return True if microphone initialization and memory allocation were successful, false otherwise.
 */
static bool microphone_inference_start(uint32_t n_samples) {
    // Ensure old buffer is freed to prevent memory leaks
    safe_free_inference_buffer();
    
    // Allocate memory for the inference buffer
    inference.buffer = (int16_t *)malloc(n_samples * sizeof(int16_t));
    if(inference.buffer == nullptr) {
        show_dynamic_message("Speicherfehler!"); // Display memory error
        return false;
    }

    inference.buf_count = 0; // Initialize buffer count
    inference.n_samples = n_samples; // Set total samples for inference
    inference.buf_ready = 0; // Mark buffer as not ready
    inference.is_initialized = true; // Mark inference as initialized

    PDM.onReceive(&pdm_data_ready_inference_callback); // Register PDM data ready callback
    PDM.setBufferSize(Config::PDM_BUFFER_SIZE); // Set PDM internal buffer size

    // Start PDM microphone
    if (!PDM.begin(1, EI_CLASSIFIER_FREQUENCY)) { // 1 channel, target frequency
        show_dynamic_message("PDM Init Fehler"); // Display PDM init error
        safe_free_inference_buffer(); // Free allocated memory on error
        return false;
    }

    PDM.setGain(Config::PDM_GAIN); // Set PDM microphone gain
    return true;
}

/**
 * @brief Records a block of audio samples for inference.
 *
 * This function waits until the inference buffer is filled by the PDM callback,
 * with a timeout to prevent infinite blocking.
 * @return True if audio recording was successful and the buffer is ready, false on timeout or if not initialized.
 */
static bool microphone_inference_record(void) {
    if (!inference.is_initialized) { // Check if inference system is initialized
        return false;
    }
    
    inference.buf_ready = 0; // Reset buffer ready flag
    inference.buf_count = 0; // Reset buffer count
    
    // Timeout-based waiting logic instead of an endless loop
    unsigned long timeout = millis() + Config::INFERENCE_TIMEOUT_MS; // Calculate timeout time
    while(inference.buf_ready == 0 && millis() < timeout) { // Wait until buffer is ready or timeout occurs
        delay(10); // Small delay to yield to other tasks and avoid busy-waiting
    }
    
    if (inference.buf_ready == 0) { // Check if timeout occurred
        show_dynamic_message("Audio Timeout!"); // Display audio timeout message
        return false;
    }
    
    return true; // Buffer is ready
}

/**
 * @brief Retrieves a portion of the recorded audio signal for inference.
 *
 * This function converts signed 16-bit audio samples to float for the Edge Impulse classifier.
 * @param offset Starting offset in the audio buffer.
 * @param length Number of samples to retrieve.
 * @param out_ptr Pointer to the output float array where converted samples will be stored.
 * @return 0 on success, -1 if inference is not initialized or buffer is null.
 */
static int microphone_audio_signal_get_data(size_t offset, size_t length, float *out_ptr) {
    if (!inference.is_initialized || inference.buffer == nullptr) { // Check if initialized and buffer exists
        return -1; // Error state
    }
    // Convert 16-bit signed integers to floats and store in out_ptr
    numpy::int16_to_float(&inference.buffer[offset], out_ptr, length);
    return 0; // Success
}

/**
 * @brief Stops the microphone and cleans up allocated resources.
 */
static void microphone_inference_end(void) {
    PDM.end(); // Stop the PDM microphone
    safe_free_inference_buffer(); // Free the inference buffer memory
}

/**
 * @brief Arduino setup function.
 *
 * Initializes LEDs, OLED display, and the PDM microphone.
 * Handles initial display messages and error conditions.
 */
void setup() {
    // initialize pin
    pinMode(Config::RED_LED_PIN, OUTPUT); // Set red LED pin as output
    pinMode(Config::GREEN_LED_PIN, OUTPUT); // Set green LED pin as output
    turn_off_all_leds(); // Ensure all LEDs are off initially

    // initialize display
    if (!display.begin(SSD1306_SWITCHCAPVCC, Config::OLED_ADDRESS)) {
        // Fallback: LEDs blinken lassen wenn Display nicht funktioniert
        for (int i = 0; i < 10; i++) { // Blink red LED 10 times to indicate display error
            digitalWrite(Config::RED_LED_PIN, HIGH);
            delay(200);
            digitalWrite(Config::RED_LED_PIN, LOW);
            delay(200);
        }
        while (1); // Stop the system if display fails
    }
    
    show_dynamic_message("Will-     kommen!"); // Display welcome message
    delay(Config::WELCOME_DELAY_MS); // Wait for welcome message duration
    show_dynamic_message("bereit"); // Display ready message

    // Mikrofon-Initialisierung
    if (microphone_inference_start(EI_CLASSIFIER_RAW_SAMPLE_COUNT) == false) { // Initialize microphone for inference
        show_dynamic_message("Mic Init Fehler"); // Display microphone initialization error
        while (1); // Stop the system on microphone error
    }
}

/**
 * @brief Arduino loop function.
 *
 * Continuously monitors for voice commands, performs inference, and controls LEDs and OLED display.
 * Manages command active states and timeouts.
 */
void loop() {
    // Command execution phase
    if (command_active) { // Check if a command is currently active
        if (millis() < command_end_time) { // Check if command duration has not expired
            // Display countdown
            unsigned long remaining_time_ms = command_end_time - millis(); // Calculate remaining time
            int remaining_seconds = (remaining_time_ms + 999) / 1000; // Convert to seconds, rounding up
            char msg_buffer[32]; 
            snprintf(msg_buffer, sizeof(msg_buffer), "%s: %ds", top_label_global, remaining_seconds); // Format message
            show_dynamic_message(msg_buffer); // Display command and countdown

            // Special handling for "blinken"
            if (strcmp(top_label_global, "blinken") == 0) { // Check if the command is "blinken"
                handle_blinking(); // Call non-blocking blinking function
            }

            delay(Config::DISPLAY_UPDATE_INTERVAL_MS); // Small delay for display update
            return; // Return to loop start to continue command execution
        } else {
            // Command ended - reset system
            turn_off_all_leds(); // Turn off all LEDs
            command_active = false; // Deactivate command
            show_dynamic_message("bereit"); // Display ready message
            return; // Return to loop start
        }
    }

    // Audio acquisition with error handling
    if (!microphone_inference_record()) { // Record audio for inference
        show_dynamic_message("Audiofehler!"); // Display audio error message
        delay(Config::ERROR_DELAY_MS); // Wait for error message duration
        return; // Return to loop start
    }

    // Voice Activity Detection - save CPU by not running inference on silence
    if (!check_audio_activity(inference.buffer, inference.n_samples, Config::AUDIO_ACTIVITY_THRESHOLD)) {
        show_dynamic_message("bereit"); // Display ready message if no audio activity
        delay(Config::READY_DELAY_MS); // Wait before checking again
        return; // Return to loop start
    }

    // Perform ML inference
    signal_t signal; // Structure for audio signal
    signal.total_length = EI_CLASSIFIER_RAW_SAMPLE_COUNT; // Set total length of signal
    signal.get_data = &microphone_audio_signal_get_data; // Assign callback to get data
    ei_impulse_result_t result = { 0 }; // Initialize result structure

    EI_IMPULSE_ERROR r = run_classifier(&signal, &result, debug_nn); // Run the Edge Impulse classifier
    if (r != EI_IMPULSE_OK) { // Check for classifier error
        show_dynamic_message("ML Fehler"); // Display ML error message
        delay(Config::ERROR_DELAY_MS); // Wait for error message duration
        return; // Return to loop start
    }

    // Find the best recognition result
    float max_value = 0.0f; // Stores the maximum confidence value
    top_label_global = ""; // Stores the label with the highest confidence
    
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) { // Iterate through all classification labels
        if (result.classification[ix].value > max_value) { // If current label's confidence is higher
            max_value = result.classification[ix].value; // Update max confidence
            top_label_global = result.classification[ix].label; // Update top label
        }
    }

    // Recognition logic - Three scenarios
    if (is_valid_label(top_label_global) && max_value >= Config::CONFIDENCE_THRESHOLD) {
        // Scenario 1: Valid command with high confidence
        command_active = true; // Activate command state
        command_end_time = millis() + Config::COMMAND_DURATION_MS; // Set command end time
        
        execute_led_command(top_label_global); // Execute LED action for the recognized command
        
        char initial_msg_buffer[32]; // Buffer for initial command message
        snprintf(initial_msg_buffer, sizeof(initial_msg_buffer), "%s: %ds", top_label_global, (int)(Config::COMMAND_DURATION_MS / 1000)); // Format message
        show_dynamic_message(initial_msg_buffer); // Display initial command and duration
        return; // Return to loop start
        
    } else if (is_ignored_label(top_label_global) && max_value >= Config::CONFIDENCE_THRESHOLD) {
        // Scenario 2: Ignored label (noise, background etc.) with high confidence
        show_dynamic_message("bereit"); // Display ready message
        delay(Config::READY_DELAY_MS); // Wait before checking again
        return; // Return to loop start
        
    } else {
        // Scenario 3: Unclear recognition - ask user to repeat
        show_dynamic_message("bitte wie- derholen!"); // Display "please repeat" message
        delay(Config::REPEAT_MESSAGE_DELAY_MS); // Wait for message duration
        show_dynamic_message("bereit"); // Display ready message
        delay(Config::READY_DELAY_MS); // Wait before checking again
        return; // Return to loop start
    }
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_MICROPHONE
#error "Invalid model for current sensor. This model requires a microphone." // Compiler error if the model is not trained for a microphone sensor
#endif