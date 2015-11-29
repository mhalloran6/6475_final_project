import numpy as np
import cv2
import sys

if len(sys.argv) != 3:
    print "usage: recolor <input_image> <output_image>"
    sys.exit(0)

input = sys.argv[1]
output = sys.argv[2]

img = cv2.imread(input)
shape = img.shape
h = shape[0]
w = shape[1]

# get the right center median
start =  w * .5
end = w * .75
a = img[.25 * h:h * .75, start:end]
color_count = {}
colors = {}

[h,w,d] = a.shape
for j in xrange(0, h):
    for i in xrange(0,w):
        color = a[j, i]
        key = str(color)
        if (not key in color_count.keys()):
            color_count[key] = 0
            colors[key] = color
     
        color_count[key]+=1

winner = None
max_count = 0
for k,v in color_count.iteritems():
    if (v > max_count):
        winner = colors[k]
        max_count = v
print winner

r1, g1, b1 = winner[0], winner[1], winner[2]
red, green, blue = img[:,:,0], img[:,:,1], img[:,:,2]
mask = (red == r1) & (green == g1) & (blue == b1)
img[:,:,:3][mask] = [0, 0, 255]


debug = False
if debug:
    cv2.imshow('img',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
cv2.imwrite(output, img)

