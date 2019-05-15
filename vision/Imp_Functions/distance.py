class Distance:
    def __init__(self, width, focalLength, distance):
        self.known_width = width
        self.focal_length = focalLength
        self.known_distance = distance

    def calculateDistance(self, w):
        if self.focal_length is not None and self.known_width is not None:
            return (self.focal_length * (self.known_width / 2)) / (w / 2)
        else:
            print("Focallength or KnownWidth isn't defined")

    def calibration(self, w):
        if self.known_distance is not None and self.known_width is not None:
            return (self.known_distance * (w / 2)) / (self.known_width / 2)
        else:
            print("KnownDistance or KnownWidth isn't defined")
