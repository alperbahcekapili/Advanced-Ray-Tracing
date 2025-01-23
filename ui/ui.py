import tkinter as tk
from PIL import Image, ImageTk
import time
import os

import threading
import json

from game import Game

pipe_in = '/tmp/pipe_in'
pipe_out = '/tmp/pipe_out'


cursor_pos = [0,0]







class ImageRefreshApp:
    def __init__(self, root, image_path, game):
        self.root = root
        self.image_path = image_path
        self.image_label = tk.Label(root)
        self.image_label.pack()
        self.game = game
        threading.Thread(target=self.listen_to_pipe, args=(self.game, )).start()


    def load_image(self):
        try:
            image = Image.open(self.image_path)
            
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
    def listen_to_pipe(self, obj):
        try:
            while True:
                obj.refresh.wait()
                print("Trying to refres...")
                obj.refresh.clear()
                self.refresh_image()
            
        except Exception as e:
            pass


def main():
    image_path = "/home/alpfischer/raytracer_project/Advanced-Ray-Tracing/build/plane_bilinear.png"  # Update with the actual path
    refresh_interval = 1  # Refresh every 5 seconds

    root = tk.Tk()
    root.title("Image Refresh App")

    


    # Register keystoke event
    game = Game()

    root.bind('<w>', game.handle)
    root.bind('<a>', game.handle)
    root.bind('<s>', game.handle)
    root.bind('<d>', game.handle)
    root.bind('<m>', game.handle)
    

    app = ImageRefreshApp(root, image_path, game)
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


