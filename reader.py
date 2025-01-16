import os

pipe_name = '/home/alpfischer/raytracer_project/Advanced-Ray-Tracing/ui/my_pipe2'

# Ensure the named pipe exists
if not os.path.exists(pipe_name):
    os.mkfifo(pipe_name)

with open(pipe_name, 'r') as pipe:
    while True:
        line = pipe.readline()
        if line:
            print(line.strip())