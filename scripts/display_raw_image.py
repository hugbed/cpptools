import numpy as np
import cv2
import sys

if len(sys.argv) < 4:
    print("Usage is %s filename size_x size_y nb_channels" % sys.argv[0])

# input arguments
in_filename = sys.argv[1]
image_size = (int(sys.argv[2]), int(sys.argv[3]))
nb_channels = int(sys.argv[4])
display_size = (960, 540)

# load image
with open(in_filename, 'rb') as fd:
    cols, rows = image_size
    f = np.fromfile(fd, dtype=np.uint8, count=rows*cols*nb_channels)
    im = f.reshape((rows, cols, nb_channels))

# display image
im = cv2.resize(im, display_size)
cv2.imshow('', im)
cv2.waitKey()
cv2.destroyAllWindows()
