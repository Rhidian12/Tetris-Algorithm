import sys
from PIL import ImageGrab

if __name__ == "__main__":
    if len(sys.argv) > 1:
        screenshot = ImageGrab.grab()
        screenshot.save(sys.argv[1] + ".png", 'PNG')
