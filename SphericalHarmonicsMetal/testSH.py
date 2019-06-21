from SphericalHarmonics import SphericalHarmonics, read_code
import numpy as np
from timeit import default_timer as timer

resolution=400
colorMap = 8
code = 0

t = timer()
coords = SphericalHarmonics(resolution, colorMap, code) # res, colorMap index, code
print('lap:', timer()-t)

npcoords = np.array(coords)
print('coords:', npcoords[0][:10], '\nnormals:', npcoords[1][:10], '\ncolors:', npcoords[2][:10], '\ntextures:', npcoords[3][:10])

new_coords = read_code(code)
print(np.all(npcoords==np.array(new_coords)))
exit(1)
