import numpy as np
import matplotlib.pyplot as plt
import wave

# Path to the wave file
wave_file_path = 'asdf.wav'

# Open the wave file
wave_file = wave.open(wave_file_path, 'r')

# Sampling rate of the wave file
fs = wave_file.getframerate()

# Number of frames in the wave file
data_size = wave_file.getnframes()

# Read data from the wave file
wave_data = wave_file.readframes(data_size)
signal = np.frombuffer(wave_data, dtype=np.int16)

# Set time range
start_time = 0  # Start time (in seconds)
end_time = 2    # End time (in seconds)

# Indices for the desired time range
start_index = int(start_time * fs)
end_index = int(end_time * fs)

# Crop time vector
cropped_time = np.linspace(start_time, end_time, end_index - start_index)
cropped_signal = signal[start_index:end_index]

# Create plot
plt.figure(figsize=(10, 6))
plt.plot(cropped_time, cropped_signal, 'b')
plt.xlabel('Time (s)')
plt.ylabel('Amplitude')
plt.title('Wave Audio Visualization (Segment)')
plt.grid(True)
plt.show()

# Close the wave file
wave_file.close()
