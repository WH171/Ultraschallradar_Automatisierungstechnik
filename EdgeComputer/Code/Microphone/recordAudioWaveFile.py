import wave             # Module for handling WAV files
import sys              # Module to access system-specific parameters
import pyaudio          # PyAudio for capturing audio from microphone

CHUNK = 1024            # Number of frames per buffer
FORMAT = pyaudio.paInt16    # Audio format (16-bit PCM)
CHANNELS = 1 if sys.platform == 'darwin' else 2    # Mono on macOS, stereo otherwise
RATE = 44100            # Sampling rate in Hz
RECORD_SECONDS = 5      # Duration of recording

p = pyaudio.PyAudio()   # Create PyAudio instance

with wave.open('output.wav', 'wb') as wf:          # Open WAV file for writing
    wf.setnchannels(CHANNELS)                      # Set number of audio channels
    wf.setsampwidth(p.get_sample_size(FORMAT))     # Set sample width from audio format
    wf.setframerate(RATE)                          # Set sampling rate

    stream = p.open(format=FORMAT,                 # Open audio input stream
                    channels=CHANNELS,
                    rate=RATE,
                    input=True)

    print('Recording...')                          # Notify user recording started

    for _ in range(0, RATE // CHUNK * RECORD_SECONDS):     # Loop to read and write chunks
        wf.writeframes(stream.read(CHUNK))                 # Read audio chunk and write to file

    print('Done')                                # Notify user recording ended

    stream.close()                               # Close audio stream

p.terminate()                                     # Terminate PyAudio instance
