# This is a sample Python script.

# Press Maj+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import glob
import os

import numpy as np
from PIL import Image
import logging
import pathlib
import src.logging.logging_setting as log_setting
import src.algo.pixelisation as pix
import src.image_utils as imu
import src.algo.ConfigPixelisation as cplst
import random as rd
from src.logging.CustomFormatter import CustomFormatter

# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    # init logging settings
    log_setting.log_setting()


    config_file = "D:/Repos/2D-3D/python/pixel_image/config/config1.json"

    config = cplst.ConfigPixelisation()
    # config.exportFile(config_file)
    config.readFile(config_file)
    config.importData()

    im_process = pix.process(config)

    output_im_filepath = "D:/Repos/2D-3D/python/pixel_image/data/process/" + \
                         os.path.basename(config.getParameter(cplst.IMAGE_FILEPATH_KEY))

    logging.info("writing output image in file : " + output_im_filepath)
    imu.save_im(output_im_filepath, im_process)
