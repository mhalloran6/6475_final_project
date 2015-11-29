import numpy as np
import cv2
import sys

if len(sys.argv) != 2:
    print "usage: kmean <input_image> <output_image>"
    sys.exit(0)

input = sys.argv[0]
output = sys.argv[1]

img = cv2.imread(input)
Z = img.reshape((-1,3))
Z = np.float32(Z)

criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
K = 6
ret,label,center=cv2.kmeans(Z,K, criteria,10, cv2.KMEANS_RANDOM_CENTERS)
center = np.uint8(center)
res = center[label.flatten()]
res2 = res.reshape((img.shape))
edges = cv2.Canny(res2,100,200)
#kernel = np.ones((3,3),np.uint8)
#edges = cv2.Canny(res2,100,200)



cv2.imshow('edges',edges)
cv2.imshow('img',img)
cv2.imshow('res2',res2)
cv2.waitKey(0)
cv2.destroyAllWindows()
cv2.imwrite(output, res2)

