import wave 

input_file = 'input.wav'
wav_file = wave.open(input_file, 'r')

nchannels = wav_file.getnchannels()
sample_width = wav_file.getsampwidth()
framerate = wav_file.getframerate()
nframes = wav_file.getnframes()

audio_data = wav_file.readframes(nframes)

# create the new wave file in write mode 
# set parameters based on the values read
output_file = 'output.wav'
wav_output = wave.open(output_file, 'w')
wav_output.setparams((nchannels, sample_width, framerate, nframes,
'NONE', 'not compressed'))

# write the read audio data into the new wave file
wav_output.writeframes(audio_data)

# close both the input and output files
wav_file.close()
wav_output.close()
