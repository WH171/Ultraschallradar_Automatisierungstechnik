import pyaudio
import wave

# Set desired values
start = 4  # Start time in seconds
ende = 9   # End time in seconds
dauer = ende - start  # Duration of the segment in seconds

# Open the WAV file
# Adjust the file path to change the wave file being used
wave_datei = wave.open('asdf.wav', 'rb')

# Create a PyAudio object
p = pyaudio.PyAudio()

# Open the stream based on the wave object
stream = p.open(
    format=p.get_format_from_width(wave_datei.getsampwidth()),
    channels=wave_datei.getnchannels(),
    rate=wave_datei.getframerate(),
    output=True
)

# Read the data (skip to the start position)
wave_datei.readframes(start * wave_datei.getframerate())
data = wave_datei.readframes(dauer * wave_datei.getframerate())

# Play the stream (from start to end)
stream.write(data)

# Save to a new file
# File path of the clipped audio segment
neue_wave_datei = wave.open('ausschnitt.wav', 'wb')
neue_wave_datei.setparams(wave_datei.getparams())
neue_wave_datei.writeframes(data)
neue_wave_datei.close()

# Clean up
wave_datei.close()
stream.close()
p.terminate()
