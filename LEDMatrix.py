#import the necessary libraries: Image (for loading the pic), NumPy (to store the image data) and serial (for talking to the matrix)
import Image, numpy, serial
#import the data type which will store the final number as a byte
from ctypes import c_ubyte

#try and open the serial port:
try:
    tothematrix = serial.Serial('/dev/ttyUSB0', 9600)
    #if that fails, try the other serial port (for some reason it sometimes uses this)
except serial.SerialException:
    tothematrix = serial.Serial('/dev/ttyUSB1', 9600)

#define a function to convert the boolean array into integers
def ba2int(bool_array):
    #create an empty C byte
    x = c_ubyte()
    #set the value of this to the converted bool array
    x.value = (sum(2**i for i, v in enumerate(reversed(bool_array)) if v))
    #return this to be used in the code
    return x

#set up a main function
def main():
    #ask the user where the image is
    imgpath = input('What is the path of the image?')
    #TESTING ONLY
    imgpath = "/home/archieroques/smiley.png"
    #open that image with the Image library
    img = Image.open(imgpath)
    #converts the image to black and white in case it isn't already
    img.convert("1")
    #makes a NumPy array from the image, as this is something the NumPy library will do
    array = numpy.array(img)
    #converts that to a standard python array
    array.tolist()

    #create a eight rows, with eight bools per row (so one per LED)
    #default value is 'X', a string string which is an unacceptable/null value anyway
    row0 = ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X']
    row1 = ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X']
    row2 = ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X']
    row3 = ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X']
    row4 = ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X']
    row5 = ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X']
    row6 = ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X']
    row7 = ['X', 'X', 'X', 'X', 'X', 'X', 'X', 'X']

    #create empty unsigned byte variables to hold the data to be sent
    send0 = c_ubyte(8)
    send1 = c_ubyte(8)
    send2 = c_ubyte(8)
    send3 = c_ubyte(8)
    send4 = c_ubyte(8)
    send5 = c_ubyte(8)
    send6 = c_ubyte(8)
    send7 = c_ubyte(8)

        
    #create a 2D array to temporarily store the boolean image data in
    bool_array = [[0 for x in range(8)] for y in range(8)] 

    #for each pixel in each row:
    for row in range(8):
        for pixel in range(8):
            #if pixel is white: set that LED to off
            if str(array[row][pixel]) == '[255 255 255]':
                bool_array[row][pixel] = False
                print('off')
            #if pixel is not white: set that LED to on
            else:
                bool_array[row][pixel] = True
                print('on')

    #for each pixel in each row:
    for row in range(8):
        for pixel in range(8):
            #store the relevant pixel in a holding variable 
            val = bool_array[row][pixel]
            #if the last item in the row still contains a zero (ie the row hasn't been filled yet)
            if row0[7] == 'X':
                #set the pixel in the 1D array to the value of that pixel's equivalent in the 2D one
                row0[pixel] = val

            #repeat this for the next 7 rows too so all pixels are accounted for:

            elif row1[7] == 'X':
                row1[pixel] = val
                
            elif row2[7] == 'X':
                row2[pixel] = val
                
            elif row3[7] == 'X':
                row3[pixel] = val
                
            elif row4[7] == 'X':
                row4[pixel] = val
                
            elif row5[7] == 'X':
                row5[pixel] = val
                
            elif row6[7] == 'X':
                row6[pixel] = val
                
            elif row7[7] == 'X':
                row7[pixel] = val

    #print all the rows for manual checking
    print(row0)
    print(row1)
    print(row2)
    print(row3)
    print(row4)
    print(row5)
    print(row6)
    print(row7)

    #ask user to confirm data
    response = input('\n\n\nData good? y/n')
    #if they say yes:
    if response == 'y' or response == 'Y':
        #send each array to be converted to an integer and then a c_ubyte
        send0 = ba2int(row0)
        send1 = ba2int(row1)
        send2 = ba2int(row2)
        send3 = ba2int(row3)
        send4 = ba2int(row4)
        send5 = ba2int(row5)
        send6 = ba2int(row6)
        send7 = ba2int(row7)

        #create an empty array of bytes
        barray = bytearray(8)
        #assign each value to a place in the bytearray
        barray[0] = send0.value
        barray[1] = send1.value
        barray[2] = send2.value
        barray[3] = send3.value
        barray[4] = send4.value
        barray[5] = send5.value
        barray[6] = send6.value
        barray[7] = send7.value
        #write that to the matrix
        tothematrix.write(barray)
        
    #if they say no, go back to the start
    else:
        main()

#when the code is run, call the main function
if __name__ == '__main__':
    main()

