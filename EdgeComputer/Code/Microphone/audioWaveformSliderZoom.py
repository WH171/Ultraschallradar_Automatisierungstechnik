import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider

# Generate example data (sine wave as audio signal)
sampling_rate = 44100  # Sampling rate in Hz
duration = 5  # Duration in seconds

# Time vector for the duration of the signal
t = np.linspace(0, duration, int(sampling_rate * duration), endpoint=False)

frequency = 440  # Frequency in Hz (A4 note)
signal = 0.5 * np.sin(2 * np.pi * frequency * t)  # Sine wave signal

# Initialize plot
fig, ax = plt.subplots()
plt.subplots_adjust(bottom=0.25)  # Make space for slider
l, = plt.plot(t, signal, lw=1)  # Plot the signal
ax.set_title('Audio Signal Display')
ax.set_xlabel('Time [s]')
ax.set_ylabel('Amplitude')

# Slider axis setup (position slider below the plot)
axcolor = 'lightgoldenrodyellow'
axpos = plt.axes([0.1, 0.1, 0.65, 0.03], facecolor=axcolor)
spos = Slider(axpos, 'Pos', 0.0, duration, valinit=0.0)  # Time position slider

# Zoom and scroll function using mouse wheel
def zoom(event):
    cur_xlim = ax.get_xlim()  # Current x-axis limits
    cur_ylim = ax.get_ylim()  # Current y-axis limits
    xdata = event.xdata  # Mouse x-position
    ydata = event.ydata  # Mouse y-position

    if event.button == 'up':
        # Zoom in
        scale_factor = 1 / 1.5
    elif event.button == 'down':
        # Zoom out
        scale_factor = 1.5
    else:
        # Do not zoom
        scale_factor = 1
        return

    # Calculate new zoomed limits
    new_width = (cur_xlim[1] - cur_xlim[0]) * scale_factor
    new_height = (cur_ylim[1] - cur_ylim[0]) * scale_factor
    relx = (cur_xlim[1] - xdata) / (cur_xlim[1] - cur_xlim[0])
    rely = (cur_ylim[1] - ydata) / (cur_ylim[1] - cur_ylim[0])

    # Apply new axis limits centered around mouse
    ax.set_xlim([xdata - new_width * (1 - relx), xdata + new_width * relx])
    ax.set_ylim([ydata - new_height * (1 - rely), ydata + new_height * rely])
    fig.canvas.draw()

# Update function for slider value change
def update(val):
    pos = spos.val  # Slider value
    ax.set_xlim([pos, pos + (cur_xlim[1] - cur_xlim[0])])  # Scroll horizontally
    fig.canvas.draw_idle()

# Connect slider and zoom functionality
spos.on_changed(update)
fig.canvas.mpl_connect('scroll_event', zoom)

# Show interactive plot
plt.show()
