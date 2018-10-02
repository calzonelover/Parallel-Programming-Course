import cv2

def writeImgData(_grey_img, _filename):
	_f = open(_filename, 'w')
	for i in range(_grey_img.shape[0]):
		for j in range(_grey_img.shape[1]):
			_f.write("{}\t".format(_grey_img[i,j]))
	_f.write("\n")


img = cv2.imread('turtle.jpg')

grey_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
writeImgData(grey_img, 'monoturtle.dat')

cv2.imwrite('monoturtle.jpg', grey_img)


## change img size and save
grey_img_small = cv2.resize(grey_img, (64,64))
grey_img_medium = cv2.resize(grey_img, (256,256))
grey_img_large = cv2.resize(grey_img, (1024,1024))
grey_img_huge = cv2.resize(grey_img, (2048,2048))

cv2.imwrite('resizedImg/monoturtle_small.jpg', grey_img_small)
cv2.imwrite('resizedImg/monoturtle_medium.jpg', grey_img_medium)
cv2.imwrite('resizedImg/monoturtle_large.jpg', grey_img_large)
cv2.imwrite('resizedImg/monoturtle_huge.jpg', grey_img_huge)

writeImgData(grey_img_small, 'resizedImg/monoturtle_small.dat')
writeImgData(grey_img_medium, 'resizedImg/monoturtle_medium.dat')
writeImgData(grey_img_large, 'resizedImg/monoturtle_large.dat')
writeImgData(grey_img_huge, 'resizedImg/monoturtle_huge.dat')

# print(grey_img.shape)