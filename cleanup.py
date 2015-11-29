import numpy as np
import cv2
import sys

if len(sys.argv) != 3:
    print "usage: recolor <input_image> <output_image>"
    sys.exit(0)

input = sys.argv[1]
output = sys.argv[2]
img = cv2.imread(input)
kernel_dim = 3
kernel = np.ones((kernel_dim,kernel_dim),np.uint8)
res2 = cv2.morphologyEx(img, cv2.MORPH_OPEN, kernel)
res2 =cv2.GaussianBlur(res2,(3,3),0)
#res2 = cv2.blur(res2,(3,3))
debug = False
if debug:
    cv2.imshow('img',res2)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
if not debug:
    cv2.imwrite(output, res2)

