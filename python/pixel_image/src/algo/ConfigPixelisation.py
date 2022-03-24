import glob
import json
import logging
import os
import pathlib
import src.image_utils as imu
import src.algo.pixelisation as pix

IMAGES_FOLDER_KEY = "FOLDER_IMAGES"
IMAGE_FILEPATH_KEY = "IMAGE_FILEPATH"
NB_IMAGETTE_PER_SIZE_KEY = "NB_IMAGETTE_PER_SIDE"
UPSCALE_FACTOR_KEY = "UPSCALE_FACTOR"
IMAGES_KEY = "IMAGES"
IMAGE_KEY = "IMAGE"
RANDOMNESS_KEY = "RANDOM"

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

                im_array = imu.load_im(image_filepath)

                if im_array.ndim == 2:
                    logging.warning('gray image transform to rgb ' + image_filepath)
                    im_array = pix.gray_to_rgb(im_array)

                if im_array.shape[0] != im_array.shape[1]:
                    logging.warning("non squared image " + image_filepath + " dimension" + str(im_array.shape))
                    im_array = pix.squarification(im_array)
                list_images.append(im_array)


        return list_images


    def __init__(self):
        self.params = {IMAGES_FOLDER_KEY: "D:/NotSetPathFolder/",
                       IMAGE_FILEPATH_KEY: "D:/NotSetPath.jpeg",
                       NB_IMAGETTE_PER_SIZE_KEY: 10,
                       UPSCALE_FACTOR_KEY: 1.0,
                       RANDOMNESS_KEY: 0.0}


    def readFile(self, file_path : str):
        with open(file_path, 'r') as f:
            params = json.load(f)
        for key in params:
            self.params[key]= params[key]

    def importData(self):
        self.params[IMAGES_KEY]= ConfigPixelisation.import_images(self.params[IMAGES_FOLDER_KEY])
        self.params[IMAGE_KEY]= imu.load_im(self.params[IMAGE_FILEPATH_KEY])

    def exportFile(self, file_path: str):
        with open(file_path, 'w') as json_file:
            json.dump(self.params, json_file, sort_keys=True, indent=4)

    def getParameter(self, key: str):
        if key in self.params:
            return self.params[key]
        else:
            logging.error(key + " key not present in the parameters")
            return None