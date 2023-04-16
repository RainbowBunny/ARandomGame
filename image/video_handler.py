import cv2
from PIL import Image

def process(file_name, destination_name):
    print(f'Reading animation from {file_name}, destination: {destination_name}')
    vidcap = cv2.VideoCapture(file_name)
    success, image = vidcap.read()
    count = 0
    while success:
        image = cv2.resize(image, (200, 200), interpolation = cv2.INTER_AREA)
        cv2.imwrite(f'{destination_name}_{count}.jpg', image)
        success, image = vidcap.read()
        print('Read a new frame: ', success)
        count += 1

if __name__ == "__main__":
    process("Fire.mp4", r"fire_animation/fire")
    process("Water.mp4", r"water_animation/water")
