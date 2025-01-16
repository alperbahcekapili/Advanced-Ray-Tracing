import os
import time

pipe_name = '/home/alpfischer/raytracer_project/Advanced-Ray-Tracing/ui/my_pipe2'

# Create named pipe if it doesn't exist
if not os.path.exists(pipe_name):
    os.mkfifo(pipe_name)

while True:
    with open(pipe_name, 'w') as pipe:
        pipe.write('Ray tracing data\n')
    time.sleep(1)


    