import glob
import json
import logging
import math
import pathlib
from typing import List

import numpy as np
import cv2
import src.image_utils as imu
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




def resize(im: imu.MyImage, new_shape):
    im.data = cv2.resize(im.data, dsize=(int(new_shape[0]), int(new_shape[1])), interpolation=cv2.INTER_AREA)
    return im



class ConfigPixelisation:

    @staticmethod
    def import_images(images_folder: pathlib.Path):
        extensions = ("jpg", "jpeg", "png")

        logging.info("**** import image folder : " + str(images_folder))

        list_images = []

        for ext in extensions:
            image_filepaths = glob.glob(str(images_folder) + "\\*" + ext)

            for image_filepath in image_filepaths:
                logging.info("**** import image : " + str(image_filepath))

                im = imu.MyImage(image_filepath)

                if im.data.ndim == 2:
                    logging.warning('gray image transform to rgb ' + image_filepath)
                    im.data = gray_to_rgb(im.data)

                if im.data.shape[0] != im.data.shape[1]:
                    logging.warning("non squared image " + image_filepath + " dimension" + str(im.data.shape))
                    im.data = squarification(im.data)
                list_images.append(im)


        return list_images


    def __init__(self):
        self.params = {IMAGES_FOLDER_KEY: "D:/NotSetPathFolder/",
                       IMAGE_FILEPATH_KEY: "D:/NotSetPath.jpeg",
                       NB_IMAGETTE_PER_SIZE_KEY: 10,
                       UPSCALE_FACTOR_KEY: 1.0,
                       RANDOMNESS_KEY: 0.0,
                       DISPLAY_TIME_STEP_KEY : -1.0}


    def readFile(self, file_path : str):
        with open(file_path, 'r') as f:
            params = json.load(f)
        for key in params:
            self.params[key]= params[key]

    def importData(self):
        self.params[IMAGES_KEY]= ConfigPixelisation.import_images(self.params[IMAGES_FOLDER_KEY])
        self.params[IMAGE_KEY]= imu.MyImage(self.params[IMAGE_FILEPATH_KEY])

    def exportFile(self, file_path: str):
        with open(file_path, 'w') as json_file:
            json.dump(self.params, json_file, sort_keys=True, indent=4)

    def getParameter(self, key: str):
        if key in self.params:
            return self.params[key]
        else:
            logging.error(key + " key not present in the parameters")
            return None

    def log(self):
        logging.info("***** CONFIGURATION *****")
        for key in self.params:
            logging.info(key + ": " + str(self.params[key]))
        logging.info("***** END CONFIGURATION *****")


class Pixelisation:

    def __init__(self, config: ConfigPixelisation):
        self.upscale_factor = config.getParameter(UPSCALE_FACTOR_KEY)
        self.im_to_process = config.getParameter(IMAGE_KEY)
        self.list_imagette = config.getParameter(IMAGES_KEY)
        self.nb_imagette_per_side = config.getParameter(NB_IMAGETTE_PER_SIZE_KEY)
        self.random_factor = config.getParameter(RANDOMNESS_KEY)
        self.step_time_display = config.getParameter(DISPLAY_TIME_STEP_KEY)
        self.penalized_overused_imagette_factor = 1.5
        self.count_per_imagette = {}
        for im in self.list_imagette:
            self.count_per_imagette[im.path] = 0

    def process(self) -> object:
        if self.im_to_process.data.shape[0] != self.im_to_process.data.shape[1]:
            logging.error("input image must be squared ! ")
            return None

        im_to_process = resize(self.im_to_process,
                               (self.upscale_factor * self.im_to_process.shape[0],
                                self.upscale_factor * self.im_to_process.shape[1]))

        im_returned = np.zeros(im_to_process.data.shape, dtype=np.uint8)

        size_imagette = im_to_process.data.shape[0] / self.nb_imagette_per_side

        if size_imagette != np.floor(size_imagette):
            logging.error(
                "size imagette n'est pas un entier " + str(size_imagette) + " size im " + str(im_to_process.shape[0])
                + " nb imagette " + str(self.nb_imagette_per_side))

        size_imagette = int(size_imagette)

        i = 0
        for im in self.list_imagette:
            im = resize(im, [size_imagette, size_imagette])
            self.list_imagette[i] = im
            i = i + 1

        begin_time = time.time()
        last_time = time.time()
        nb_total_imagette = self.nb_imagette_per_side ** 2
        for i in range(self.nb_imagette_per_side):

            for j in range(self.nb_imagette_per_side):
                im_to_compare = im_to_process[i * size_imagette:(i + 1) * size_imagette,
                                j * size_imagette:(j + 1) * size_imagette, :]
                im_selected = Pixelisation.select_best_imagette(im_to_compare, self.list_imagette, self.random_factor, self.count_per_imagette)
                im_returned[i * size_imagette:(i + 1) * size_imagette,
                j * size_imagette:(j + 1) * size_imagette, :] = im_selected

                current_time = time.time()

                if (current_time - last_time) > self.step_time_display:
                    logging.info(str(i) + '/' + str(self.nb_imagette_per_side))
                    last_time = current_time
                    elapsed_time = current_time - begin_time
                    nb_imagette_treated = i * self.nb_imagette_per_side + j
                    speed = float(nb_imagette_treated) / float(elapsed_time)
                    remaining_time = float(nb_total_imagette - nb_imagette_treated) / float(speed)
                    percent = float(nb_imagette_treated) / float(nb_total_imagette) * 100.0
                    logging.info("**********************")
                    logging.info("percent : " + str(percent))
                    logging.info("elapsed time : " + imu.time_sec_to_hms(elapsed_time))
                    logging.info("estimated remaining time : " + imu.time_sec_to_hms(remaining_time))

                # imu.save_im("D:\\Repos\\pythonProject\\data\\debug\\" + str(i) + "_" + str(j) + "selected.jpeg",
                #            im_selected)
                # imu.save_im("D:\\Repos\\pythonProject\\data\\debug\\" + str(i) + "_" + str(j) + ".jpeg", im_to_compare)
        total_time = time.time() - begin_time
        logging.info("**********************")
        logging.info("total compute time : " + imu.time_sec_to_hms(total_time))
        return im_returned

    def select_best_imagette(image_to_comp: np.ndarray, list_image, random_factor: float, count_per_imagette):
        min_score = -1
        returned_im = None
        for im in list_image:
            score = np.linalg.norm(image_to_comp - im.data) * (1.0 + np.random.uniform(0.0, random_factor) + math.log(1 + 0.00000 * count_per_imagette[im.path]))
            if min_score < 0 or min_score > score:
                min_score = score
                returned_im = im

        count_per_imagette[returned_im.path] = count_per_imagette[returned_im.path] + 1
        return returned_im.data



IMAGES_FOLDER_KEY = "FOLDER_IMAGES"
IMAGE_FILEPATH_KEY = "IMAGE_FILEPATH"
NB_IMAGETTE_PER_SIZE_KEY = "NB_IMAGETTE_PER_SIDE"
UPSCALE_FACTOR_KEY = "UPSCALE_FACTOR"
IMAGES_KEY = "IMAGES"
IMAGE_KEY = "IMAGE"
RANDOMNESS_KEY = "RANDOM"
DISPLAY_TIME_STEP_KEY = "DISPLAY_TIME_STEP"

