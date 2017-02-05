import numpy as np
with open('image.raw', 'rb') as fd:
    rows = 1920
    cols = 1080
    cols = 1920
    rows = 1080
    f = np.fromfile(fd, dtype=np.uint8, count=rows*cols*4)
    im = f.reshape((rows, cols, 4))

import cv2
im = cv2.resize(im, (960, 540))
cv2.imshow('', im)
cv2.waitKey()
cv2.destroyAllWindows()
