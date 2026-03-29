import wave
import numpy as np

# Definition of the variable alpha
alpha = 0.01

# Definition of the smoothing function
def exponential_smoothing(data):
    smoothed_data = np.zeros_like(data)
    smoothed_data[0] = data[0]
    for i in range(1, len(data)):
        smoothed_data[i] = alpha * data[i] + (1 - alpha) * smoothed_data[i - 1]
    return smoothed_data

def smooth_wave_file(input_file, output_file):
    with wave.open(input_file, 'rb') as wav_in:
        params = wav_in.getparams()
        data = np.frombuffer(wav_in.readframes(params.nframes), dtype=np.int16)
        smoothed_data = exponential_smoothing(data)
    with wave.open(output_file, 'wb') as wav_out:
        wav_out.setparams(params)
        wav_out.writeframes(smoothed_data.tobytes())

# Example usage:
input_wave_file = 'input.wav'
output_wave_file = 'smoothed_audio.wav'
smooth_wave_file(input_wave_file, output_wave_file)
