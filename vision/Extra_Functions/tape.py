import cv2
import numpy as np

def BlackTape():
    cam = cv2.VideoCapture(0)
    #cam = cv2.VideoCapture('tape.avi')    

    while(cam.isOpened()):
        #black tape
        lower_black = np.array([30,227,18])
        upper_black = np.array([105,94,84])
        #[30,227,18],[105,94,84]

        while (True):
            _, img = cam.read()
            
            gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
            edges = cv2.Canny(gray, 50, 150, apertureSize = 3)
            # minLineLength = 100
            # maxLineGap = 10
            # lines = cv2.HoughLinesP(edges,1,np.pi/180,100,minLineLength,maxLineGap)
            # i=0
            # for x1,y1,x2,y2 in lines[0]:
            #     cv2.line(img,(x1,y1),(x2,y2),(0,255,0),2)

            cv2.imshow("Lines", edges)

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break

    cam.release()
    cv2.destroyAllWindows()
    
BlackTape()
