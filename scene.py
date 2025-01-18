"""
Contains functions for creating and managing scenes.

"""

import numpy as np

# border_corners = [(100, 25, -100),(-100, -5, 100),(100, -5, 100), (100, 25, -100)]
border_corners = []
topleft = (-100, 25, -100)
square_edge = 25

grids = {}

for row in range(9):
    for col in range(5):
        y_offset = row * square_edge
        x_offset = col * square_edge
        corner = (topleft[0] + x_offset, topleft[1], topleft[2] + y_offset)
        border_corners.append(np.array(corner))
        # print(f"{corner[0]} {corner[1]} {corner[2]}")


# generate corner list
for row in range(8):
    grids[row] = {}
    for col in range(4):
        corners = [row * 5 + col, row * 5 + col + 1, row * 5 + col + 6, row * 5 + col + 5]
        triangles = [(corners[0], corners[1], corners[2]), (corners[2], corners[3], corners[0])]
        center = (border_corners[corners[0]] + border_corners[corners[1]] +border_corners[corners[2]] + border_corners[corners[2]] + border_corners[corners[3]] + border_corners[corners[0]]) / 6
        # print(f"[{center[0]},{center[1]},{center[2]}],")
        id = row * 3 + col
        if id % 2 == 0:
            material = 1
        else:
            material = 2
        id = row * 4 + col
        
        print(f"""
        <Mesh id="{id}">               
            <Material>{material}</Material>
            <Faces>
                {triangles[0][0]+1} {triangles[0][1]+1} {triangles[0][2]+1}
                {triangles[1][0]+1} {triangles[1][1]+1} {triangles[1][2]+1}
            </Faces>
        </Mesh>
""")
        
        grids[row][col] = {
            "corners": corners,
            "triangles": triangles,
            "id": id,
            "material": material
        }


# print(grids)
