import logging
from typing import List

import numpy as np
import cv2
import src.image_utils as imu
import src.algo.ConfigPixelisation as cplst
import time


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
    return cv2.resize(im, dsize=(int(new_shape[0]), int(new_shape[1])), interpolation=cv2.INTER_AREA)


def process(config: cplst.ConfigPixelisation) -> object:
    upscale_factor = config.getParameter(cplst.UPSCALE_FACTOR_KEY)
    im_to_process = config.getParameter(cplst.IMAGE_KEY)
    list_imagette = config.getParameter(cplst.IMAGES_KEY)
    nb_imagette_per_side = config.getParameter(cplst.NB_IMAGETTE_PER_SIZE_KEY)
    random_factor = config.getParameter(cplst.RANDOMNESS_KEY)
    step_time_display = config.getParameter(cplst.DISPLAY_TIME_STEP_KEY)

    if im_to_process.shape[0] != im_to_process.shape[1]:
        logging.error("input image must be squared ! ")
        return None

    im_to_process = resize(im_to_process,
                           (upscale_factor * im_to_process.shape[0], upscale_factor * im_to_process.shape[1]))

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

    begin_time = time.time()
    last_time = time.time()
    nb_total_imagette= nb_imagette_per_side**2
    for i in range(nb_imagette_per_side):

        for j in range(nb_imagette_per_side):
            im_to_compare = im_to_process[i * size_imagette:(i + 1) * size_imagette,
                            j * size_imagette:(j + 1) * size_imagette, :]
            im_selected = select_best_imagette(im_to_compare, list_imagette,random_factor)
            im_returned[i * size_imagette:(i + 1) * size_imagette,
            j * size_imagette:(j + 1) * size_imagette, :] = im_selected

            current_time = time.time()

            if (current_time - last_time)> step_time_display:
                logging.info(str(i) + '/' + str(nb_imagette_per_side))
                last_time = current_time
                elapsed_time = current_time - begin_time
                nb_imagette_treated = i*nb_imagette_per_side + j
                speed = float(nb_imagette_treated) / float(elapsed_time)
                remaining_time = float(nb_total_imagette - nb_imagette_treated)/float(speed)
                percent = float(nb_imagette_treated)/float(nb_total_imagette)*100.0
                logging.info("**********************")
                logging.info("percent : " + str(percent))
                logging.info("elapsed time : " + imu.time_sec_to_hms(elapsed_time))
                logging.info("estimated remaining time : " + imu.time_sec_to_hms(remaining_time))

            #imu.save_im("D:\\Repos\\pythonProject\\data\\debug\\" + str(i) + "_" + str(j) + "selected.jpeg",
            #            im_selected)
            #imu.save_im("D:\\Repos\\pythonProject\\data\\debug\\" + str(i) + "_" + str(j) + ".jpeg", im_to_compare)
    total_time = time.time() - begin_time
    logging.info("**********************")
    logging.info("total compute time : " + imu.time_sec_to_hms(total_time))
    return im_returned


def select_best_imagette(image_to_comp: np.ndarray, list_image, random_factor: float):
    min_score = -1
    returned_im = None
    for im in list_image:
        score = np.linalg.norm(image_to_comp - im)*(1.0 + np.random.uniform(0.0, random_factor))
        if min_score < 0 or min_score > score:
            min_score = score
            returned_im = im

    return returned_im
