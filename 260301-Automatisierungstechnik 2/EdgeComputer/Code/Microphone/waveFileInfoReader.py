import wave

# Specify the file path to the WAV file
dateipfad = "beispiel.wav"  

# Open the WAV file
with wave.open(dateipfad, 'r') as wave_datei:  # Opens the WAV file in read mode ('r')

    # Output information about the WAV file
    print("Number of channels:", wave_datei.getnchannels())       # Outputs the number of audio channels (e.g., 1 = mono, 2 = stereo)
    print("Sample rate:", wave_datei.getframerate())              # Outputs the sampling rate (e.g., 44100 Hz)
    print("Number of frames:", wave_datei.getnframes())           # Outputs the total number of frames in the file
    print("Sample width in bytes:", wave_datei.getsampwidth())    # Outputs the number of bytes per sample
