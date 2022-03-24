from PIL import Image
import numpy as np

def load_im(image_path):
    im = Image.open(image_path)
    im.load()
    print(im.mode)
    im_array = np.array(im)
    return im_array


def save_im(image_path: str, im_array: np.ndarray):
    im = Image.fromarray(im_array)
    im.save(image_path)
