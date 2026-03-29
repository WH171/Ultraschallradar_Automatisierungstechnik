import sensor # Import the module for sensor related functions
import image # Import module containing machine vision algorithms
import time # Import module for tracking elapsed time

sensor.reset() # Resets the sensor
sensor.set_contrast(3) # Sets the contrast to the highest level (min -3, max 3)
sensor.set_gainceiling(16) # Sets the amplification of camera sensor signal

# HQVGA and GRAYSCALE are the best for face tracking.
sensor.set_framesize(sensor.HQVGA)
sensor.set_pixformat(sensor.GRAYSCALE)

# Load the built-in frontal face Haar Cascade
# By default this will use all stages, lower stages is faster but less accurate.
face_cascade = image.HaarCascade("frontalface", stages=25)
print(face_cascade) # Prints the Haar Cascade configuration

faceImage = image.Image("/face.pbm", copy_to_fb=False) # Loads a bitmap file from the flash storage
clock = time.clock() # Instantiates a clock object to calculate the FPS

while (True):
    clock.tick() # Advances the clock
    cameraImage = sensor.snapshot() # Takes a snapshot and saves it in memory

    # Find objects.
    # Note: Lower scale factor scales-down the image more and detects smaller objects.
    # Higher threshold results in a higher detection rate, with more false positives.
    boundingBoxes = cameraImage.find_features(face_cascade, threshold=1, scale_factor=1.5)

    # Draw objects
    for boundingBox in boundingBoxes:
        faceX = boundingBox[0]
        faceY = boundingBox[1]
        faceWidth = boundingBox[2]

        # Calculates the scale ratio to scale the bitmap image to match the bounding box
        scale_ratio = faceWidth / faceImage.width()
        # Draws the bitmap on top of the camera stream
        cameraImage.draw_image(faceImage, faceX, faceY, x_scale=scale_ratio, y_scale=scale_ratio)


    # Print FPS.
    # Note: The actual FPS is higher when not displaying a frame buffer preview in the IDE
    print(clock.fps())
