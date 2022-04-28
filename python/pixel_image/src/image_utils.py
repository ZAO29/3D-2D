from PIL import Image
import numpy as np
import time




def save_im(image_path: str, im_array: np.ndarray):
    im = Image.fromarray(im_array)
    im.save(image_path)


def time_sec_to_hms(sec : int):
    return time.strftime('%H:%M:%S', time.gmtime(sec))


class MyImage:

    @staticmethod
    def __load_im(image_path):
        im = Image.open(image_path)
        im.load()
        im_array = np.array(im)
        return im_array


    def __init__(self, path: str):
        self.path = path
        self.data = MyImage.__load_im(path)

    @property
    def shape(self):
        return self.data.shape

    def __getitem__(self, *args, **kwargs):
        return self.data.__getitem__(*args, **kwargs)

    def __setitem__(self, *args, **kwargs):
        return self.data.__setitem__(*args,**kwargs)
