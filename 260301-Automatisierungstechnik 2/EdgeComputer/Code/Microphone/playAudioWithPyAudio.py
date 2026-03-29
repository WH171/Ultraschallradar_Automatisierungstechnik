import wave
import pyaudio

# Open the wave file
wf = wave.open('audio.wav', 'rb')

# Initialize the PyAudio object
p = pyaudio.PyAudio()

# Open the audio stream with format, channels, and rate matching the wave file
stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                channels=wf.getnchannels(),
                rate=wf.getframerate(),
                output=True)

# Read data from the wave file in chunks
data = wf.readframes(1024)

# Play the wave file by writing chunks to the audio stream
while data:
    stream.write(data)
    data = wf.readframes(1024)

# Stop and close the stream, then terminate the PyAudio object
stream.stop_stream()
stream.close()
p.terminate()
