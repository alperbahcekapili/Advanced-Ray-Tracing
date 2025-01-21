import tkinter as tk
from PIL import Image, ImageTk
import time
import os

import threading
import json



pipe_in = '/tmp/pipe_in'
pipe_out = '/tmp/pipe_out'


cursor_pos = [0,0]

def write_to_pipe(data):
    with open(pipe_out, 'w') as pipe:
        pipe.write(json.dumps(data))


# Function triggered by keystroke
def up(event):
    threading.Thread(target=write_to_pipe, args=('up',)).start()
def down(event):
    threading.Thread(target=write_to_pipe, args=("down",)).start()
def right(event):
    threading.Thread(target=write_to_pipe, args=("right",)).start()
def left(event):
    threading.Thread(target=write_to_pipe, args=("left",)).start()




class ImageRefreshApp:
    def __init__(self, root, image_path, refresh_interval):
        self.root = root
        self.image_path = image_path
        self.image_label = tk.Label(root)
        self.image_label.pack()
        threading.Thread(target=self.listen_to_pipe, daemon=True).start()


    def load_image(self):
        try:
            image = Image.open(self.image_path)
            image = image.resize((1920, 1080), Image.Resampling.LANCZOS)  # Resize for display
            return ImageTk.PhotoImage(image)
        except Exception as e:
            print(f"Error loading image: {e}")
            return None

    def refresh_image(self):
        img = self.load_image()
        if img:
            self.image_label.config(image=img)
            self.image_label.image = img

    # Function to read from the named pipe
    def listen_to_pipe(self,):
        while True:
            with open(pipe_in, 'r') as pipe:
                # Blocking call - waits for data from the pipe
                data = pipe.read().strip()
                if data:
                    # Trigger game refresh on receiving data
                    self.refresh_image()


def main():
    image_path = "/home/alpfischer/raytracer_project/Advanced-Ray-Tracing/build/plane_bilinear.png"  # Update with the actual path
    refresh_interval = 1  # Refresh every 5 seconds

    root = tk.Tk()
    root.title("Image Refresh App")

    # Register keystoke event
    root.bind('<s>', down)
    root.bind('<w>', up)
    root.bind('<d>', right)
    root.bind('<a>', left)
    

    app = ImageRefreshApp(root, image_path, refresh_interval)
    app.refresh_image()

    def on_closing():
        root.destroy()

    root.protocol("WM_DELETE_WINDOW", on_closing)
    root.mainloop()




if __name__ == "__main__":


    if not os.path.exists(pipe_in):
        os.mkfifo(pipe_in)
    if not os.path.exists(pipe_out):
        os.mkfifo(pipe_out)


    main()


