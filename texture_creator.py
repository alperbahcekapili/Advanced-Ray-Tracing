from PIL import Image, ImageDraw

def create_checkerboard_image(width, height, rows, cols):
    image = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(image)
    
    tile_width = width // cols
    tile_height = height // rows
    
    for row in range(rows):
        for col in range(cols):
            if (row + col) % 2 == 0:
                color = "white"
            else:
                color = "black"
            draw.rectangle(
                [col * tile_width, row * tile_height, (col + 1) * tile_width, (row + 1) * tile_height],
                fill=color
            )
    
    return image

if __name__ == "__main__":
    width, height = 700, 400  # Adjust the size as needed
    rows, cols = 4, 7
    checkerboard_image = create_checkerboard_image(width, height, rows, cols)
    checkerboard_image.save("checkerboard.png")