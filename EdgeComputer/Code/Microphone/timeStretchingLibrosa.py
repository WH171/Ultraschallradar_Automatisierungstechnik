# Load an example audio file using librosa
y, sr = librosa.load(librosa.ex('choice'))

# Compress the audio to be 5 times faster (rate = 0.2 means playback is faster)
y_fast = librosa.effects.time_stretch(y, rate=0.2)

# Stretch the audio to be half the original speed (slower playback)
y_slow = librosa.effects.time_stretch(y, rate=0.5)
