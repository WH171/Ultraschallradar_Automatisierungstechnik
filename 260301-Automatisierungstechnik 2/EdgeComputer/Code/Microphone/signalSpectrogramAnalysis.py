import matplotlib.pyplot as plt
import numpy as np

# Set random seed for reproducibility
np.random.seed(19680801)

# Create time array and signal sample parameters
dt = 0.0005  # Time step
t = np.arange(0.0, 20.5, dt)  # Time array from 0 to 20 seconds with step dt

# Generate two sine wave signals
s1 = np.sin(2 * np.pi * 100 * t)  # 100 Hz sine wave
s2 = 2 * np.sin(2 * np.pi * 400 * t)  # 400 Hz sine wave with double amplitude

# Create "chirp" signal by zeroing out parts of s2
s2[t <= 10] = s2[12 <= t] = 0  # Only keep signal between 10-12 seconds

# Add noise for more realistic scenario
nse = 0.01 * np.random.random(size=len(t))  # Random noise

# Combine signals and noise
x = s1 + s2 + nse  # Composite signal
NFFT = 1024  # Number of points for FFT
Fs = 1/dt  # Sampling frequency

# Create plot with two subplots (signal and spectrogram)
fig, (ax1, ax2) = plt.subplots(nrows=2, sharex=True)
ax1.plot(t, x)  # Plot time domain signal
ax1.set_ylabel('Signal')  # Y-axis label for signal plot

# Compute and plot spectrogram
Pxx, freqs, bins, im = ax2.specgram(x, NFFT=NFFT, Fs=Fs)
# Where:
# - Pxx: Spectral power
# - freqs: Frequency vector
# - bins: Time bins
# - im: Spectrogram image data

# Set axis labels and limits
ax2.set_xlabel('Time (s)')  # X-axis label
ax2.set_ylabel('Frequency (Hz)')  # Y-axis label
ax2.set_xlim(0, 20)  # Set x-axis limits

# Display the plot
plt.show()