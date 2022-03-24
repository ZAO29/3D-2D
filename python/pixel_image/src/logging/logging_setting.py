import logging
from src.logging.CustomFormatter import CustomFormatter
import datetime

def log_setting():

    # Create custom logger logging all five levels
    logger = logging.getLogger("root")
    logger.setLevel(logging.INFO)



    # Define format for logs
    fmt = '%(asctime)s | %(levelname)8s | %(message)s'

    # Create stdout handler for logging to the console (logs all five levels)
    stdout_handler = logging.StreamHandler()
    stdout_handler.setLevel(logging.DEBUG)
    stdout_handler.setFormatter(CustomFormatter(fmt))

    # Create file handler for logging to a file (logs all five levels)
    today = datetime.date.today()
    file_handler = logging.FileHandler('my_app_{}.log'.format(today.strftime('%Y_%m_%d')))
    file_handler.setLevel(logging.DEBUG)
    file_handler.setFormatter(logging.Formatter(fmt))

    # Add both handlers to the logger
    logger.addHandler(stdout_handler)
    logger.addHandler(file_handler)