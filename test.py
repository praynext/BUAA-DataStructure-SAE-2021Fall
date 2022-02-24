import os
import random
def test(n1, n2, n3):
    inf = 2147483647
    q, array, buffer = [n1, n2, n3], [], []
    AllData = open("AllData.txt", "w")
    AVLData = open("AVLData.txt", "w")
    BData = open("BData.txt", "w")
    RBData = open("RBData.txt", "w")
    AllData.write(str(n1 + n2 + n3) + "\n")
    n = 0
    while n <= 2:
        temp = [str(n), str(q[n])]
        AllData.write(temp[1] + "\n" + temp[0] + "\n")
        if int(temp[0]) == 0:
            buffer = random.sample(range(-inf, inf), int(temp[1]))
            array.extend(buffer)
            j = 0
            while j < int(temp[1]):
                AVLData.write(str(buffer[j]) + "\n")
                BData.write(str(buffer[j]) + "\n")
                RBData.write(str(buffer[j]) + "\n")
                j += 1
        else:
            buffer = random.sample(array, int(temp[1]))
            j = 0
            while j < int(temp[1]):
                AVLData.write(str(buffer[j]) + "\n")
                BData.write(str(buffer[j]) + "\n")
                RBData.write(str(buffer[j]) + "\n")
                j += 1
        n += 1
    for i in array:
        AVLData.write(str(i) + "\n")
        BData.write(str(i) + "\n")
        RBData.write(str(i) + "\n")

    AllData.close()
    AVLData.close()
    BData.close()
    RBData.close()