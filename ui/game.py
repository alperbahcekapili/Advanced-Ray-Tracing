import tkinter as tk
from PIL import Image, ImageTk
import time
import os

import threading
import json


pipe_in = '/tmp/pipe_in'
pipe_out = '/tmp/pipe_out'



class Game:

    def write_to_pipe(self, data):  
        self.pipeout = open(pipe_out, 'w')  
        self.pipeout.write(data+"\n")
        self.pipeout.flush()

    def handle(self, event):
        print(event)
        # set current tile color if tile movement
        if event.char in ['s', 'w', 'd', 'a']:
            self.send_paint_event(self.cursor[0], self.cursor[1], 1 if (self.cursor[0]*3 + self.cursor[1]) % 2 == 1 else 0)
            self.wait_update()
            print(event.char)
            if event.char == 's':
                self.cursor[0] = (self.cursor[0] + 7) % 8
            if event.char == 'w':
                self.cursor[0] = (self.cursor[0] + 1) % 8
            if event.char == 'd':
                self.cursor[1] = (self.cursor[1] + 3) % 4
            if event.char == 'a':
                self.cursor[1] = (self.cursor[1] + 1) % 4
            # if event.char in ['s', 'w', 'd', 'a']:
            self.send_paint_event(self.cursor[0], self.cursor[1], 15)
        
        if event.char in "m":
            # means selecting this tile
            # TODO: edge case different objects at the same location
            if self.game_stage == "setup":
                obj_index = self.p1_pieces[6 - self.pending_pieces] if self.turn == 0 else self.p2_pieces[6 - self.pending_pieces]
                self.object_locs[self.cursor[0]] = {} if self.cursor[0] not in self.object_locs else self.object_locs[self.cursor[0]]
                self.object_locs[self.cursor[0]][self.cursor[1]] = obj_index
                if self.turn == 1:
                    self.pending_pieces-=1
                self.turn = 0 if self.turn == 1 else 1
                self.send_move_event(self.cursor[0], self.cursor[1],obj_index)
            else:    
                # obj_index = self.get_object_at()
                pass
    def wait_update(self,):
        with open(pipe_in, 'r') as pipe:
            pipe.read()
            print("Frame is updated...")


    def send_paint_event(self, first, second, material):
        self.write_to_pipe(f"p,{first},{second},{material}")

    def send_move_event(self, first, second, obj_index):
        self.write_to_pipe(f"m,{first},{second},{obj_index}")

    def get_object_at(self):
        try:
            return self.object_locs[self.cursor[0]][self.cursor[1]]
        except Exception as e:
            return -1
        
    
        
    def __init__(self):
        self.p1_pieces = [38, 39, 42, 43, 46, 47]
        self.p2_pieces = [40, 41, 44, 45, 48, 49]
        self.cursor = [0,0]
        self.turn = 0
        self.selected_object = 0
        self.object_locs = {}
        self.game_stage = "setup" # means when we select the tile, we will place objects there
        self.pending_pieces = 6
