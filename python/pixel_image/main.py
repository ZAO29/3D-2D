# This is a sample Python script.

# Press Maj+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import glob

import numpy as np
from PIL import Image
import logging
import pathlib
import src.logging.logging_setting as log_setting
import src.algo.pixelisation as pix
import src.image_utils as imu

from src.logging.CustomFormatter import CustomFormatter


def import_images(images_folder: pathlib.Path):
    extensions = ("jpg", "jpeg", "png")

    logging.info("**** import image folder : " + str(images_folder))

    list_images = []

    for ext in extensions:
        image_filepaths = glob.glob(str(images_folder) + "\\*" + ext)

        for image_filepath in image_filepaths:
            logging.info("**** import image : " + str(image_filepath))

            im_array = imu.load_im(image_filepath)

            if im_array.ndim == 2:
                logging.warning('gray image transform to rgb ' + image_filepath)
                im_array = pix.gray_to_rgb(im_array)

            if im_array.shape[0] != im_array.shape[1]:
                logging.warning("non squared image " + image_filepath + " dimension" + str(im_array.shape))
                im_array = pix.squarification(im_array)
            list_images.append(im_array)

    logging.warning(" YOLO ")
    return list_images


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    # init logging settings
    log_setting.log_setting()
    # name_im = "wish_you_were_here"
    name_im = "ride_the_lightning.jpeg"
    # name_im = "RGB_square.jpeg"

    image_folder = pathlib.Path("D:/Repos/2D-3D/python/pixel_image/data/album/")
    im_path = pathlib.Path("D:/Repos/2D-3D/python/pixel_image/data/album/" + name_im)

    list_im = import_images(image_folder)
    im_to_process = imu.load_im(im_path)

    im_process = pix.process(im_to_process=im_to_process, list_imagette=list_im, nb_imagette_per_side=16,
                             upscale_factor=2)

    output_im_filepath = "D:\\Repos\\pythonProject\\data\\process\\" + name_im
    logging.info("writing output image in file : " + output_im_filepath)
    imu.save_im(output_im_filepath, im_process)
