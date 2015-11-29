import numpy as np
import cv2


def process_image(src, box_size = 3):
    image = src.copy()
    shape = image.shape
    h = shape[0]
    w = shape[1]
    for j in xrange(0,h):
        for i in xrange(0,w):
            if (j < box_size + 1 ) or (i < box_size +1):
                continue
            if (j + box_size > h -1) or (i + box_size > w -1):
                continue
            color = image[j - box_size, i - box_size]
            contained_box = True
            for jj in xrange(j - box_size, j + box_size):
                if (not np.all(image[jj, i - box_size] == color) or 
                    not np.all(image[jj, i + box_size] == color)):
                    contained_box = False
            for ii in xrange(i - box_size, i + box_size):
                if (not np.all(image[j - box_size, ii] == color) or 
                    not np.all(image[j + box_size, ii] == color)):
                    contained_box = False
            if contained_box:
                image[j-box_size:j+box_size, i-box_size:i+box_size] = color
    return image



img = cv2.imread('bush.jpg')
Z = img.reshape((-1,3))
Z = np.float32(Z)

criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
K = 6
ret,label,center=cv2.kmeans(Z,K, criteria,10, cv2.KMEANS_RANDOM_CENTERS)
center = np.uint8(center)
res = center[label.flatten()]
res2 = res.reshape((img.shape))
edges = cv2.Canny(res2,100,200)
kernel = np.ones((3,3),np.uint8)
#res2 = cv2.morphologyEx(res2, cv2.MORPH_OPEN, kernel)
edges = cv2.Canny(res2,100,200)

print center
cv2.imshow('res2',res2)
image_1 = process_image(res2, 1)
cv2.imshow('image_1',image_1)

image_3 = process_image(res2, 3)
cv2.imshow('image_3',image_3)
image_6 = process_image(res2, 6)
cv2.imshow('image_6',image_6)
image_9 = process_image(res2, 9)
cv2.imshow('image_9',image_9)
image_12 = process_image(res2, 12)
cv2.imshow('image_12',image_12)

cv2.imshow('edges',edges)
cv2.imshow('img',img)
cv2.imshow('res2',res2)
cv2.waitKey(0)
cv2.destroyAllWindows()
