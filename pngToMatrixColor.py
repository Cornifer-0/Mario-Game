from PIL import Image
import os

# Function to process a single image into a matrix
def process_image(file_path):
    img = Image.open(file_path).convert("RGBA")  # Ensure it has alpha
    pixels = img.load()
    width, height = img.size

    matrix = []
    for y in range(height):
        row = []
        for x in range(width):
            r, g, b, a = pixels[x, y]
            if a == 0:
                row.append("_")
            else:
                hex_val = f"0x{r:02x}{g:02x}{b:02x}"
                row.append(hex_val)
        matrix.append(row)

    # # Remove rows that are entirely "_"
    # matrix = [row for row in matrix if any(cell != "_" for cell in row)]

    # # Remove columns that are entirely "_"
    # if matrix:
    #     transposed = list(zip(*matrix))
    #     transposed = [col for col in transposed if any(cell != "_" for cell in col)]
    #     matrix = [list(row) for row in zip(*transposed)]

    return matrix

# Directory and base name for the sprite files
base_name = "bowser_dead"
file_extension = ".png"

# Vector of matrices
vector_of_matrices = []

# Iterate through sequentially numbered files
n = 1
while n == 1:
    file_path = f"sprites/{base_name}{file_extension}"
    if not os.path.exists(file_path):
        break
    matrix = process_image(file_path)
    vector_of_matrices.append(matrix)
    n += 1

# Print as a C++-style vector of matrices
print("const AnimationFrame " + base_name  + " = {")
for matrix in vector_of_matrices:
    print("  {")
    for row in matrix:
        print("    {" + ", ".join(row) + "},")
    print("  },")
print("};")
