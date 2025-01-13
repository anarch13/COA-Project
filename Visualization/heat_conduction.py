import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Heat conduction parameters
dx = 0.1  # Space step (m)
dt = 0.01  # Time step (s)
k = 0.1  # Thermal diffusivity (m^2/s)
alpha = k * dt / dx**2 

# Grid size
nx, ny = 50, 50  # Number of grid points
steps = 100  # Time steps

# Initialize the temperature field
temp = np.zeros((steps, nx, ny))

# Add random hotspots to the field
np.random.seed(10) 
random_pos = [(np.random.randint(1, nx - 1), np.random.randint(1, ny - 1)) for _ in range(50)]

# Assign random temperatures to the selected positions
random_temp = {}
for x, y in random_pos:
    random_temp[(x, y)] = np.random.uniform(80, 100)
    temp[0][x][y] = random_temp[(x, y)]

# Calculate the temperature for all time steps
for t in range(0, steps - 1):
    for x in range(1, nx - 1):
        for y in range(1, ny - 1):
            if (x, y) in random_temp:
                temp[t + 1][x][y] = random_temp[(x, y)]
            else:
                temp[t + 1][x][y] = temp[t][x][y] + alpha * (
                    temp[t][x - 1][y] + temp[t][x + 1][y] + temp[t][x][y - 1] + temp[t][x][y + 1] - 4 * temp[t][x][y])

# Animation
def animate(t):
    plt.clf()
    plt.xlabel("x")
    plt.ylabel("y")
    plt.pcolormesh(temp[t], cmap="hot", vmin=0, vmax=100)
    plt.colorbar()

anim = animation.FuncAnimation(plt.figure(), animate, interval=1, frames=steps, repeat=False)
anim.save("heat_conduction_2d.gif")