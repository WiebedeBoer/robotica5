import numpy as np
import cv2
import pyzbar.pyzbar as pyzbar


def ShowQR():
    cap = cv2.VideoCapture(0)
    allQRCodes = []

    _, frame = cap.read()
    decodedObjects = pyzbar.decode(frame)

    for obj in decodedObjects:
        print("Data: ", obj.data, "\n")
        qrdata = obj.data

        if(qrdata =="'s-Hertogenbosch" or qrdata =="http://Barneveld" or
                qrdata =="http://Duckstad" or qrdata =="http://Eibergen" or qrdata =="http://Eindhoven"):
            allQRCodes.append(obj.data)

    if len(allQRCodes) != 0:
        print("Returning")
        return allQRCodes
    else:
        return None

    cap.release()

ShowQR()
