import matplotlib.pyplot as plt
import numpy as np
import wave

# Load the audio files
with wave.open('audio1.wav', 'rb') as wf1:
    y1 = wf1.readframes(-1)
    sr1 = wf1.getframerate()

with wave.open('inputAudio2.wav', 'rb') as wf2:
    y2 = wf2.readframes(-1)
    sr2 = wf2.getframerate()

# Convert audio files to numpy arrays
y1 = np.frombuffer(y1, dtype=np.int16)
y2 = np.frombuffer(y2, dtype=np.int16)

# Determine axes for the plot
x1 = np.linspace(0, len(y1) / sr1, len(y1))
x2 = np.linspace(0, len(y2) / sr2, len(y2))

# Plot the data
plt.figure(figsize=(10, 4))
plt.plot(x1, y1, label='Audio 1')
plt.plot(x2, y2, label='Audio 2')
plt.xlabel('Time (seconds)')
plt.ylabel('Amplitude')
plt.title('Audio Waveforms')
plt.legend()
plt.grid(True)
plt.show()
