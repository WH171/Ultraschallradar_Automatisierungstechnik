from time import sleep
import RPi.GPIO as GPIO

# Set the pin numbering mode to BOARD (uses physical pin numbers)
GPIO.setmode(GPIO.BOARD)

# Define the signal pin number
Signal_Pin = 16

# Set the signal pin as an output
GPIO.setup(Signal_Pin, GPIO.OUT)

try:
    # Infinite loop to toggle the output
    while True:
        # Turn the output ON (set pin HIGH)
        GPIO.output(Signal_Pin, GPIO.HIGH)
        print("Output activated")
        sleep(10)  # Keep output on for 10 seconds

        # Turn the output OFF (set pin LOW)
        GPIO.output(Signal_Pin, GPIO.LOW)
        print("Output deactivated")
        sleep(5)  # Wait 5 seconds before turning it on again

except KeyboardInterrupt:
    # Clean up GPIO settings when the program is interrupted (e.g., with Ctrl+C)
    GPIO.cleanup()