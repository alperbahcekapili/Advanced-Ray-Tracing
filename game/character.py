from enum import Enum

class CharacterType(Enum):
    WALL=1
    BERSERKER=2
    DRAGON=3

class Character:
    """
    positon is grids-wise values as follows: (0,1), (5,7)
    material and textures are id's
    """
    def __init__(self, position, material, texture, transformations, type):
        self.position = position
        self.material = material
        self.texture = texture

        assert isinstance(type, CharacterType)
        self.type = type

    def move(self, new_pos):
        euc_dis = abs(new_pos[0] - self.position[0]) + abs(new_pos[1] - self.position[1]) 
        if(euc_dis > 1):
            raise Exception("Pieces can only move to adj. sqaure")
    
    def action(self, target):
        euc_dis = abs(target[0] - self.position[0]) + abs(target[1] - self.position[1]) 
        if(euc_dis > 1):
            raise Exception("In order to take action choose an adj. square")
        

        
