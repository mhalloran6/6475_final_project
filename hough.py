import numpy as np
import cv2
import sys

if len(sys.argv) != 3:
    print "usage: kmean <input_image> <output_image>"
    sys.exit(0)

input = sys.argv[1]
output = sys.argv[2]

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

plane = center[0]
lower = plane
upper = plane
mask = cv2.inRange(res2, lower, upper)
kernel = np.ones((5,5),np.uint8)
mask = cv2.erode(mask,kernel,iterations = 1)
mask = cv2.dilate(mask,kernel,iterations = 1)

cv2.imshow('edges', edges)

minInter = 10
minLineLength = 5
maxLineGap = 5

lines = cv2.HoughLinesP(edges,1,np.pi/180,minInter, minLineLength,maxLineGap)
if lines.any():
    for x1,y1,x2,y2 in lines[0]:
        cv2.line(res2,(x1,y1),(x2,y2),(0,255,0),2)
else:
    print "No lines"

cv2.imshow('images', res2)
cv2.waitKey(0)
cv2.destroyAllWindows()

debug = False
if debug:
    cv2.imshow('mask',mask)
    cv2.imshow('edges',edges)
    cv2.imshow('img',img)
    cv2.imshow('res2',res2)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

cv2.imwrite(output, res2)

