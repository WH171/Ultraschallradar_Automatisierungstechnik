from guizero import App, MenuBar

# Function called when a File menu option is selected
def file_function():
    print("File option")

# Function called when an Edit menu option is selected
def edit_function():
    print("Edit option")

# Create the main application window
app = App()

# Define the menu bar with two top-level menus: File and Edit
menubar = MenuBar(
    app,
    toplevel=["File", "Edit"],  # Top-level menu labels
    options=[
        [["File option 1", file_function], ["File option 2", file_function]],  # File menu options
        [["Edit option 1", edit_function], ["Edit option 2", edit_function]]   # Edit menu options
    ]
)

# Display the GUI window and start the event loop
app.display()
