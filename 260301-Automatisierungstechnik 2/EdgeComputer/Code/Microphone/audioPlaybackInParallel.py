import wave
import pyaudio
import threading

# Function to play sound from a given file
def play_sound(file):
    chunk = 1024  # Size of each audio chunk to read and play
    wf = wave.open(file, 'rb')  # Open the audio file in read-binary mode
    p = pyaudio.PyAudio()  # Initialize the PyAudio object for playback
    stream = p.open(format=p.get_format_from_width  # Open the stream with the necessary audio parameters
                    (wf.getsampwidth()),  # Sample width (bit depth)
                    channels=wf.getnchannels(),  # Number of channels (mono or stereo)
                    rate=wf.getframerate(),  # Frame rate (sample rate)
                    output=True)  # Set to output audio
    data = wf.readframes(chunk)  # Read the first chunk of audio data
    
    # Loop through the audio data and play it in chunks
    while data:
        stream.write(data)  # Play the chunk
        data = wf.readframes(chunk)  # Read the next chunk
    
    stream.stop_stream()  # Stop the stream after all data has been played
    stream.close()  # Close the stream
    p.terminate()  # Terminate the PyAudio instance

# List of audio files to play
files = ['sound1.wav', 'sound2.wav', 'sound3.wav']

# List to store threads
threads = []

# Create and start a thread for each audio file
for file in files:
    thread = threading.Thread(target=play_sound, args=(file,))
    threads.append(thread)  # Add the thread to the list
    thread.start()  # Start the thread

# Wait for all threads to complete
for thread in threads:
    thread.join()  # Ensure the main program waits for the threads to finish
