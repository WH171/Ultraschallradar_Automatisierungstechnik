# Load an example audio file
y, sr = librosa.load(librosa.ex('choice'))

# Shift the pitch up by a major third (4 semitones; assuming 12 bins per octave)
y_third = librosa.effects.pitch_shift(y, sr=sr, n_steps=4)

# Shift the pitch down by a tritone (6 semitones; also assuming 12 bins per octave)
y_tritone = librosa.effects.pitch_shift(y, sr=sr, n_steps=-6)

# Shift the pitch up by 3 quarter-tones (requires higher resolution: 24 bins per octave)
y_three_qt = librosa.effects.pitch_shift(y, sr=sr, n_steps=3, bins_per_octave=24)
