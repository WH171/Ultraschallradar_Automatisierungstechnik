import numpy as np
import wave

# Define the audio sample rate (samples per second)
FRAMES_PER_SECOND = 44100

# Function to generate a sine wave for a given frequency and duration
def sound_wave(frequency, num_seconds):
    time = np.arange(0, num_seconds, 1 / FRAMES_PER_SECOND)  # Create time steps
    amplitude = np.sin(2 * np.pi * frequency * time)         # Generate sine wave
    return np.clip(
        np.round(amplitude * 32768),                         # Scale to 16-bit range
        -32768,
        32767,
    ).astype("<h")                                           # Convert to 16-bit PCM format

# Generate left and right audio channels with different frequencies
left_channel = sound_wave(440, 2.5)     # 440 Hz (A4 note)
right_channel = sound_wave(480, 2.5)    # 480 Hz

# Interleave left and right channels to form stereo audio
stereo_frames = np.dstack((left_channel, right_channel)).flatten()

# Write the stereo audio to a WAV file
with wave.open("output.wav", mode="wb") as wav_file:
    wav_file.setnchannels(2)                     # Stereo: 2 channels
    wav_file.setsampwidth(2)                     # 2 bytes (16-bit)
    wav_file.setframerate(FRAMES_PER_SECOND)     # Set the sample rate
    wav_file.writeframes(stereo_frames)          # Write audio frames to file
