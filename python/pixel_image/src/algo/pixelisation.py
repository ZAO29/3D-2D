import logging
from typing import List

import numpy as np
import cv2
import src.image_utils as imu

def gray_to_rgb(im: np.ndarray):
    im_3 = np.reshape(im, (im.shape[0], im.shape[1], 1))
    return np.tile(im_3, [1, 1, 3])


def squarification(array: np.ndarray) -> np.ndarray:
    min_dim = min(array.shape[0], array.shape[1])
    id_max = np.argmax(array.shape[0:2])
    middles = np.array(array.shape[0:2]).astype(np.float) / 2.0

    minimum_l = int(np.floor(min_dim / 2))
    middle = int(np.floor(middles[id_max]))

    minimum_r = minimum_l

    if minimum_l != min_dim / 2:
        minimum_r = minimum_l + 1

    if id_max == 0:

        square_array = array[middle - minimum_l:middle + minimum_r, :, :]
    else:
        square_array = array[:, middle - minimum_l:middle + minimum_r, :]

    return square_array

def resize(im: np.ndarray, new_shape):
    return cv2.resize(im, dsize=(int(new_shape[0]),int(new_shape[1])), interpolation=cv2.INTER_AREA)

def process(im_to_process: np.ndarray, list_imagette: List[np.ndarray], nb_imagette_per_side: int, upscale_factor : int) -> object:

    if im_to_process.shape[0] != im_to_process.shape[1]:
        logging.error("input image must be squared ! ")
        return None

    im_to_process = resize(im_to_process, (upscale_factor * im_to_process.shape[0], upscale_factor * im_to_process.shape[1]))

    im_returned = np.zeros(im_to_process.shape, dtype=np.uint8)

    size_imagette = im_to_process.shape[0] / nb_imagette_per_side

    if size_imagette != np.floor(size_imagette):
        logging.error(
            "size imagette n'est pas un entier " + str(size_imagette) + " size im " + str(im_to_process.shape[0])
            + " nb imagette " + str(nb_imagette_per_side))

    size_imagette = int(size_imagette)

    i = 0
    for im in list_imagette:
        im = resize(im, [size_imagette, size_imagette])
        list_imagette[i] = im
        i = i + 1

    for i in range(nb_imagette_per_side):
        logging.info(str(i)+'/'+str(nb_imagette_per_side))
        for j in range(nb_imagette_per_side):
            im_to_compare = im_to_process[i * size_imagette:(i + 1) * size_imagette,
                            j * size_imagette:(j + 1) * size_imagette, :]
            im_selected = select_best_imagette(im_to_compare, list_imagette)
            im_returned[i * size_imagette:(i + 1) * size_imagette,
                            j * size_imagette:(j + 1) * size_imagette, :] = im_selected

            imu.save_im("D:\\Repos\\pythonProject\\data\\debug\\" + str(i) + "_" + str(j) + "selected.jpeg", im_selected)
            imu.save_im("D:\\Repos\\pythonProject\\data\\debug\\" + str(i) + "_" + str(j) + ".jpeg", im_to_compare)

    return im_returned

def select_best_imagette(image_to_comp: np.ndarray, list_image):
    min_score = -1
    returned_im = None
    for im in list_image:
        score = np.linalg.norm(image_to_comp - im)
        if min_score < 0 or min_score > score:
            min_score = score
            returned_im = im

    return returned_im
