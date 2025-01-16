import tkinter as tk
from PIL import Image, ImageTk
import time
import os

import threading
import json




def write_to_pipe(data):
    with open(pipe_name, 'w') as pipe:
        pipe.write(json.dumps(data))


# Function triggered by keystroke
def on_key(event):
    # Example data to send
    data = {
        "name": "Alice",
        "age": 30,
        "city": "Wonderland"
    }

    # Start a thread to write to the pipe
    threading.Thread(target=write_to_pipe, args=(data,)).start()




class ImageRefreshApp:
    def __init__(self, root, image_path, refresh_interval):
        self.root = root
        self.image_path = image_path
        self.image_label = tk.Label(root)
        self.image_label.pack()
        threading.Thread(target=self.listen_to_pipe, daemon=True).start()


    def load_image(self):
        try:
            if self.image_path.endswith("2.png"):
                self.image_path = self.image_path.replace("2.png", ".png")
            else:
                self.image_path = self.image_path.replace(".png", "2.png")
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
            with open(pipe_name, 'r') as pipe:
                # Blocking call - waits for data from the pipe
                data = pipe.read().strip()
                if data:
                    # Trigger game refresh on receiving data
                    self.refresh_image()


def main():
    image_path = "test.png"  # Update with the actual path
    refresh_interval = 1  # Refresh every 5 seconds

    root = tk.Tk()
    root.title("Image Refresh App")

    # Register keystoke event
    root.bind('<s>', on_key)  # Press 's' to trigger data sending
    

    app = ImageRefreshApp(root, image_path, refresh_interval)
    app.refresh_image()

    def on_closing():
        root.destroy()

    root.protocol("WM_DELETE_WINDOW", on_closing)
    root.mainloop()

if __name__ == "__main__":

    # set up named pipe connection
    pipe_name = "my_pipe2"


    if not os.path.exists(pipe_name):
        os.mkfifo(pipe_name)


    main()


