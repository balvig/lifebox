 # -*- coding: utf-8 -*-

import epd2in13
from PIL import Image
import requests
import time

def main():
    while True:
        try:
            # Get dimensions
            width = epd2in13.EPD_WIDTH
            height = epd2in13.EPD_HEIGHT

            # Download image
            base_url = "http://lifeboxes.herokuapp.com/work"
            # base_url = "http://4a707c28.ngrok.io/work"
            url = base_url + "?width={}&height={}".format(width, height)
            image_data = requests.get(url, stream = True).raw
            image = Image.open(image_data)

            # Display image
            epd = epd2in13.EPD()
            epd.init(epd.lut_full_update)
            epd.clear_frame_memory(0xFF)

            epd.set_frame_memory(image, 0, 0)
            epd.display_frame()
        except KeyboardInterrupt:
            raise
        except Exception as e:
            print "Rescued error {0}: {1}".format(e.__doc__, e.message)
        finally:
            time.sleep(30 * 60)

if __name__ == '__main__':
    main()
