import sys
import numpy as np
import pyaudio

# Audio recording parameters
CHUNK = 1024                            # Number of audio frames per buffer
FORMAT = pyaudio.paInt16                # 16-bit audio format
CHANNELS = 1 if sys.platform == 'darwin' else 2  # Mono for macOS, stereo otherwise
RATE = 44100                            # Sampling rate (Hz)
RECORD_SECONDS = 5                      # Duration to record

p = pyaudio.PyAudio()                   # Initialize PyAudio instance

# Open an audio stream for recording
stream = p.open(format=FORMAT,
                channels=CHANNELS,
                rate=RATE,
                input=True)

data = np.array([])                     # Initialize an empty NumPy array to store audio data

print('Recording...')
# Read audio in chunks and store into the data array
for _ in range(0, RATE // CHUNK * RECORD_SECONDS):
    in_data = stream.read(CHUNK)                        # Read raw bytes from audio stream
    in_data = np.frombuffer(in_data, np.int16)          # Convert to NumPy array of int16
    data = np.concatenate((data, in_data))              # Append to existing data
print('Done')

MAX_LENGTH = 1024                         # Max segment length for averaging

data = data[0::2]                         # Downsample if stereo: take every second sample (left channel)

# Trim data to a multiple of 1024 samples
data = data[-data.size // MAX_LENGTH * MAX_LENGTH:]

# Reshape into 1024-sample blocks and average each block
data = data.reshape(-1, data.size // MAX_LENGTH)
data = np.mean(data, axis=1)             # Compute mean of each block

print(data)                               # Output the averaged result

# Clean up the audio stream
stream.close()
p.terminate()
