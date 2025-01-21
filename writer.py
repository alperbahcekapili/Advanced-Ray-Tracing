import os
import time


pipe_in = '/tmp/pipe_in'
pipe_out = '/tmp/pipe_out'



# Create named pipe if it doesn't exist
if not os.path.exists(pipe_out):
    os.mkfifo(pipe_out)

while True:
    with open(pipe_out, 'w') as pipe:
        pipe.write('Baboli\n')
    time.sleep(1)

    # Read a message from the C++ program
    with open(pipe_in, 'r') as read_pipe:
        message = read_pipe.readline().strip()
        print(f"Python received: {message}")


    