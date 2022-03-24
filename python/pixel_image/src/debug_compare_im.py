import cv2
import matplotlib.pyplot as plt
import src.image_utils as imu
import numpy as np

image_name1 = "D:/Repos/pythonProject/data/album/nevermind.jpg"
# image_name1 = "D:/Repos/pythonProject/data/album/ride_the_lightning.jpeg"
# image_name1 = "D:/Repos/pythonProject/data/album/carpenter_brut_3.jpg"
# image_name1 = "D:/Repos/pythonProject/data/album/carpenter_logo.jpg"
image_name1 = "D:/Repos/pythonProject/data/album/in_utero.jpg"
image_name2 = "D:/Repos/pythonProject/data/RGB_setup/red.jpeg"

image_paths = [image_name1, image_name2]

rgb = ['r', 'g', 'b']
line_types = ['-', '--']

im_pair = []

for im_path in image_paths:
    im_pair.append(imu.load_im(im_path))


score = np.linalg.norm(im_pair[0] - im_pair[1])


id_im = 0
for im in im_pair:
    fig =plt.figure(id_im)
    fig.suptitle(image_paths[id_im])
    for i in range(3):
            ax = plt.subplot(3,1,i+1)
            h = cv2.calcHist(im, [i], None, [256], [0, 255])
            plt.plot(range(0,256), h, rgb[i]+line_types[id_im])


    id_im = id_im + 1

plt.show()

print ('SCORE '+ score)



