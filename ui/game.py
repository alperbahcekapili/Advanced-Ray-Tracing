import numpy as np
import tkinter as tk
from PIL import Image, ImageTk
import time
import os

import threading
import json


pipe_in = '/tmp/pipe_in'
pipe_out = '/tmp/pipe_out'



class Game:

    def kill(self, obj_id):
        print(f"Killing object {obj_id}, it's location: {self.object_locs[obj_id]}")
        print()
        self.send_move_event(8, 0, obj_id)

        print(f"Killing object {obj_id}, it's location: {self.object_locs[obj_id]}")
    def normalize_tile(self, pos1, pos2):
        self.send_paint_event(pos1, pos2, self.get_color_of_tile(pos1, pos2))
    def action(self):
        print("Entered action fucntion. There is a war ! ")
        

        attacking_obj = self.selected_object
        endangered_obj = self.get_object_at()

        

        if attacking_obj in self.p1_pieces and endangered_obj in self.p1_pieces:
            print("No friendly fire !!!")
            return "invalid"
        if attacking_obj in self.p2_pieces and endangered_obj in self.p2_pieces:
            print("No friendly fire !!!")
            return "invalid"
        
        # objects should be in the same diagonal
        attacker_pos = self.object_locs[attacking_obj]
        endangered_pos = self.object_locs[endangered_obj]
        if (attacker_pos[0] != endangered_pos[0]) and (attacker_pos[1] != endangered_pos[1]):
            print("Pieces are not aligned thus you cannot attack that piece...")
            return "invalid"
        
        align_axis = 0 if attacker_pos[0] == endangered_pos[0] else 1
        other_axis = abs(align_axis-1)
        distance = abs(attacker_pos[other_axis] - endangered_pos[other_axis])

        if distance == 0:
            print("Suicide is not allowed!!!")
            return

        wall_ids = [38, 39, 40, 41]
        berserker_ids = [46, 47, 48, 49]

        # if attacking_obj is a dragon then it can attack to pos up to 3 blocks away
        dragon_ids = [42, 43, 44, 45]
        if attacking_obj in dragon_ids:
            if distance > 3:
                print("Dragon can attack to a piece at most 4 tiles away")
                return "invalid"

            # Dragon will spit fire now. All the pieces in the radius will be effected
            direction = 1 if endangered_pos[other_axis] - attacker_pos[other_axis] > 0 else -1

            endangered_pos1 = [attacker_pos[0], attacker_pos[1]]
            endangered_pos1[other_axis] += 1 * direction

            endangered_pos2 = [attacker_pos[0], attacker_pos[1]]
            endangered_pos2[other_axis] += 2 * direction

            endangered_pos3 = [attacker_pos[0], attacker_pos[1]]
            endangered_pos3[other_axis] += 3 * direction
            
            # we set the locations that we will set to fire. Iterate over them
            # location might be empty, there can be an object
            self.send_paint_event(endangered_pos1[0], endangered_pos1[1], 15)
            obj_in_the_middle = self.get_object_at_(endangered_pos1[0], endangered_pos1[1])
            if obj_in_the_middle != -1:
                if obj_in_the_middle in wall_ids:
                    print("Opps, fire shall not pass!!!")
                    self.normalize_tile(endangered_pos1[0], endangered_pos1[1])
                    return
                # if object is not wall, then unfortunately for my guy life is over
                self.kill(obj_in_the_middle)
            self.normalize_tile(endangered_pos1[0], endangered_pos1[1])


            self.send_paint_event(endangered_pos2[0], endangered_pos2[1], 15)
            obj_in_the_middle = self.get_object_at_(endangered_pos2[0], endangered_pos2[1])
            if obj_in_the_middle != -1:
                if obj_in_the_middle in wall_ids:
                    print("Opps, fire shall not pass!!!")
                    self.normalize_tile(endangered_pos2[0], endangered_pos2[1])
                    return
                # if object is not wall, then unfortunately for my guy life is over
                self.kill(obj_in_the_middle)
            self.normalize_tile(endangered_pos2[0], endangered_pos2[1])


            self.send_paint_event(endangered_pos3[0], endangered_pos3[1], 15)
            obj_in_the_middle = self.get_object_at_(endangered_pos3[0], endangered_pos3[1])
            if obj_in_the_middle != -1:
                if obj_in_the_middle in wall_ids:
                    print("Opps, fire shall not pass!!!")
                    self.normalize_tile(endangered_pos3[0], endangered_pos3[1])
                    return
                # if object is not wall, then unfortunately for my guy life is over
                self.kill(obj_in_the_middle)
            self.normalize_tile(endangered_pos3[0], endangered_pos3[1])


        elif attacking_obj in berserker_ids:



            # Berserker will swing it's sword now. All the pieces in the radius will be effected
            direction = 1 if endangered_pos[other_axis] - attacker_pos[other_axis] > 0 else -1
            endangered_pos1 = [attacker_pos[0], attacker_pos[1]]
            endangered_pos1[other_axis] += 1 * direction

            self.send_paint_event(endangered_pos1[0], endangered_pos1[1], 15)
            obj_in_the_middle = self.get_object_at_(endangered_pos1[0], endangered_pos1[1])

            if obj_in_the_middle != -1:
                if obj_in_the_middle in dragon_ids:
                    print("Dragon laughs at you !!!")
                    self.normalize_tile(endangered_pos1[0], endangered_pos1[1])
                    return
                # if object is not wall, then unfortunately for my guy life is over
                self.kill(obj_in_the_middle)
            self.normalize_tile(endangered_pos1[0], endangered_pos1[1])
            
        elif attacking_obj in wall_ids:
            print("Ultra mega attack approaching !!!")












         

    






    def write_to_pipe(self, data):  
        with open(pipe_out, 'w')   as f:
            f.write(data+"\n")
            f.flush()

    def get_color_of_tile(self, first, second):
        return 1 if (first*3 + second) % 2 == 1 else 0

    def handle(self, event):
        print(event)
        # set current tile color if tile movement
        if event.char in ['s', 'w', 'd', 'a']:
            self.send_paint_event(self.cursor[0], self.cursor[1], 1 if (self.cursor[0]*3 + self.cursor[1]) % 2 == 1 else 0)
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
            if self.game_stage == "setup" and (6 - self.pending_pieces) < 6:
                
                obj_index = self.p1_pieces[6 - self.pending_pieces] if self.turn == 0 else self.p2_pieces[6 - self.pending_pieces]
                self.object_locs[obj_index] = [self.cursor[0], self.cursor[1]]
                if self.turn == 1:
                    self.pending_pieces-=1
                if self.pending_pieces == 0:
                    self.game_stage == "game"

                self.turn = 0 if self.turn == 1 else 1
                self.send_move_event(self.cursor[0], self.cursor[1], obj_index)


            else:    
                # selects oject if not already selected, if it is already selected then it moves selected to cursor
                if self.selected_object != -1:
                    print("Selected object is not empty and cursor at an object location")

                    # player might be attacking or might be moving
                    obj_at_selected = self.get_object_at()
                    if obj_at_selected != -1 :
                        # then player is attacking
                        print("Attack is about to be performed...")
                        # let the war begin
                        self.action()
                        self.selected_object = -1
                        return

                        
                    # verify cursor is in a valid location 
                    if abs(np.sum(np.array(self.cursor) - np.array(self.object_locs[self.selected_object]))) != 1:
                        print("Selected position is too far. Please select adjacent tile")
                        return

                    if self.selected_object in self.wall_ids:
                        print("Wall tried it's best to move but life is just ... Maybe next time")
                        self.selected_object = -1
                        return
                    self.send_move_event(self.cursor[0], self.cursor[1], self.selected_object)
                    self.selected_object = -1
                else:
                    obj_index_temp = self.get_object_at()
                    if obj_index_temp == -1:
                        print("There is no piece there so chosen piece is -1")
                        return
                    # TODO: Ensure object is players
                    # TODO: Verify object can move there
                    self.selected_object = obj_index_temp
                    print(f"Selected {self.selected_object} piece...")
                # obj_index = self.get_object_at()
                pass

    
    def wait_update(self,):
        with open(pipe_in, 'r') as pipe:
            pipe.read()
            print("Frame is updated...")
            self.refresh.set()


    def send_paint_event(self, first, second, material):
        self.write_to_pipe(f"p,{first},{second},{material}")
        self.wait_update()

    def send_move_event(self, first, second, obj_index):
        # we need to empty old location
        self.object_locs[obj_index] = [first,second]
        self.write_to_pipe(f"m,{first},{second},{obj_index}")
        self.wait_update()

    def get_object_at_(self, pos1, pos2):
        try:
            for k,v in self.object_locs.items():
                if v[0] == pos1 and v[1] == pos2:
                    return k
            return -1
        except Exception as e:
            print(e)
            return -1

    def get_object_at(self):
        try:
            for k,v in self.object_locs.items():
                if v[0] == self.cursor[0] and v[1] == self.cursor[1]:
                    return k
            return -1
        except Exception as e:
            print(e)
            return -1
        
    
        
    def __init__(self):
        self.p1_pieces = [38, 40, 42, 44, 46, 48]
        self.p2_pieces = [39, 41, 43, 45, 47, 49]
        self.cursor = [0,0]
        self.turn = 0
        self.selected_object = -1
        self.object_locs = {}
        self.game_stage = "setup" # means when we select the tile, we will place objects there
        self.pending_pieces = 6
        self.refresh = threading.Event()


        self.wall_ids = [38, 39, 40, 41]
        self.berserker_ids = [46, 47, 48, 49]
        self.dragon_ids = [42, 43, 44, 45]

        # self.automatic_setup()
