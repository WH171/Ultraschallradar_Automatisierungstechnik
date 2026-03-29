import librosa
import matplotlib.pyplot as plt
import soundfile as sf

# Path to the WAV file
wav_datei = "input.wav"

# Load the audio file
# 'sr=None' preserves the original sampling rate
# 'mono=False' loads the file as stereo if available
signal, abtastrate = librosa.load(wav_datei, sr=None, mono=False)

# Change the pitch by modifying the number of semitone steps
# For example, n_steps=1 corresponds to a pitch shift of one semitone
pitch_shifted = librosa.effects.pitch_shift(signal, 
                                            n_steps=10,
                                            sr=abtastrate,
                                            bins_per_octave=12)

data = pitch_shifted

# Write the output to a new WAV file with 24-bit PCM encoding
sf.write('PitchShifted.wav', data.T, abtastrate, subtype='PCM_24', format='WAV')
