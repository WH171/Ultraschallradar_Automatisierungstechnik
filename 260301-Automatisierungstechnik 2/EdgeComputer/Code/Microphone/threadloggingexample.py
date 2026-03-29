import logging  # Module for logging informational messages
import threading  # Module to work with threads
import time  # Module to handle time-related tasks

# Function that will be executed in each thread
def thread_function(name):
    logging.info("Thread %s: starting", name)

# Main entry point of the program
if __name__ == "__main__":
    # Set up logging format and level
    format = "%(asctime)s: %(message)s"
    logging.basicConfig(format=format, level=logging.INFO, 
                        datefmt="%H:%M:%S")

    threads = list()  # List to keep track of threads

    # Create and start 3 threads
    for index in range(3):
        logging.info("Main   : create and start thread %d.", index)
        x = threading.Thread(target=thread_function, args=(index,))
        threads.append(x)  # Add thread to the list
        x.start()  # Start the thread

    # Wait for all threads to complete
    for index, thread in enumerate(threads):
        logging.info("Main   : before joining thread %d.", index)
        thread.join()  # Wait for the thread to finish
        logging.info("Main   : thread %d done", index)
