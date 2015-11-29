import numpy as np
import cv2
import sys

if len(sys.argv) != 3:
    print "usage: kmean <input_image> <output_image>"
    sys.exit(0)

def kmean(img, K = 6):
    Z = img.reshape((-1,3))
    Z = np.float32(Z)
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
    ret,label,center=cv2.kmeans(Z,K, criteria,10, cv2.KMEANS_RANDOM_CENTERS)
    center = np.uint8(center)
    res = center[label.flatten()]
    res2 = res.reshape((img.shape))
    return res2, center

def extract_color(img, color):
    mask = cv2.inRange(img, color, color)
    result = img.copy()
    result[np.where(mask == 0)] = 0
    return result

input = sys.argv[1]
output = sys.argv[2]

img = cv2.imread(input)
edges = cv2.Canny(img,100,100)
h,w,d = img.shape
#blobs = np.zeros(h,w)
km, colors = kmean(img, 6)


i = 0
for c in colors:
    img_c = extract_color(km, c)
    #img_c = cv2.GaussianBlur(img_c,(5,5),0)
    #img_c = extract_color(img_c, c)
    cv2.imshow("img_" + str(i), img_c)
    i+=1

small = cv2.resize(img, (0,0), fx=0.1, fy=0.1) 
edges2 = cv2.Canny(small,100,100)
blob_colors = []

#for j in xrange(0, h):
#    for i in xrange(0, w):
#        color = img[j,i]
#        id = color_identifier(color)


debug = True
if debug:
    cv2.imshow('edges',edges)
    cv2.imshow('img',img)
    #cv2.imshow('edges2',edges2)
    cv2.imshow('km',km)
    #cv2.imshow('imgc',img_c)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
cv2.imwrite(output, res2)

