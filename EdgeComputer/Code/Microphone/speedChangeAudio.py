import librosa
import matplotlib.pyplot as plt
import soundfile as sf

# Path to the WAV file
wavFile = "input.wav"

# Load the audio file
# sr=None keeps the original sampling rate
# mono=False preserves the stereo channels
signal, sampleRate = librosa.load(wavFile, sr=None, mono=False)

# Speed up the WAV file by a factor of 1.5
speedUpSignal = librosa.effects.time_stretch(signal, rate=1.5)

# Store the modified signal in a variable
data = speedUpSignal

# Output the modified WAV file
# .T transposes the array to match the expected shape (channels x samples)
sf.write('speedChangeOutput.wav', data.T, sampleRate, subtype='PCM_24', format='WAV')
