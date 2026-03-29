from guizero import App, FileSaveDialog
import wave

def save_wave_file():
    # Create an App instance (GUI backend, invisible)
    app = App(visible=False)  # Start app in invisible mode

    # Open the file explorer to select the save location
    file_path = FileSaveDialog(
        app, title="Save As", filetypes=[("Wave files", "*.wav")], initialfilename="*.wav"
    ).value

    if file_path:
        # Open the selected file in write mode
        with wave.open(file_path, 'w') as wf:
            wf.setnchannels(2)       # Two channels (Stereo)
            wf.setsampwidth(2)       # 2 bytes per sample
            wf.setframerate(44100)   # Sampling rate of 44100 Hz
            wf.setnframes(0)         # Initially zero frames

            # Write data to the file 
            # (Audio data should be inserted here)
            # Example: wf.writeframes(b'audiodata')

        print("File was successfully saved to:", file_path)
    else:
        print("Save cancelled.")

    # Close the app
    app.destroy()

save_wave_file()
