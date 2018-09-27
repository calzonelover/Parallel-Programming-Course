import cv2

img = cv2.imread('turtle.jpg')

grey_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

f = open('monoturtle.jpg', 'w')

for i in range(grey_img.shape[0]):
	for j in range(grey_img.shape[1]):
		f.write("{}\t".format(grey_img[i,j]))
	f.write("\n")

f.close()

# print(grey_img.shape)