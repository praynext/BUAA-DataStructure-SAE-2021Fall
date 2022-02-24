import re
import os
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from test import test

def _test(a, b):
    for i in range(a):
        k = 10
        while k <= b:
            test(k, k, k)
            os.system(".\\test.exe")
            print([i, k])
            k *= 10

def picture3(k, r, m):
    '''
    x= 100
    while x > 0:
        y = 100
        while y > 0:
            test(k, x*r, y*m)
            os.system(".\\test.exe")
            print([x,y])
            y -= 1
        x -= 1
    '''
    p = re.compile(r"\d+\.\d+")
    array1, array2, array3 = [], [], []
    buffer1, buffer2, buffer3 = [], [], []
    result = open("result.txt", "r")
    i = 1
    while True:
        if i == 4:
            i = 1
        line = result.readline()
        if not line:
            break
        a = re.findall(p, line)
        if a != []:
            if i == 1:
                array1.extend(a)
            elif i == 2:
                array2.extend(a)
            else:
                array3.extend(a)
            i += 1
    i = 0
    while i < len(array1):
        buffer1.append(float(array1[i]) + float(array1[i+1]) + float(array1[i+2]))
        buffer2.append(float(array2[i]) + float(array2[i+1]) + float(array2[i+2]))
        buffer3.append(float(array3[i]) + float(array3[i+1]) + float(array3[i+2]))
        i += 3
    x1, x2 = [], []
    n, y = 1, 1
    while n >= 0.01:
        for i in range(100):
            x1.append(n)
        n -= 0.01
    for i in range(100):
        while y >= 0.01:
            x2.append(y)
            y -= 0.01
        y = 1
    pic1 = plt.figure()
    ax = Axes3D(pic1,auto_add_to_figure=False)
    pic1.add_axes(ax)
    ax.set_zlabel('time', fontdict={'size': 10, 'color': 'blue'})
    ax.set_ylabel('delete', fontdict={'size': 10, 'color': 'blue'})
    ax.set_xlabel('search', fontdict={'size': 10, 'color': 'blue'})

    X = np.array(x1)
    Y = np.array(x2)
    Z = np.array(buffer1)

    ax.plot_trisurf(X, Y, Z, cmap='spring')

    plt.suptitle("AVLTree", color='r', size=15)
    plt.savefig("AVLTree.png",dpi = 400)
    plt.show()

    pic2 = plt.figure()
    ax = Axes3D(pic2,auto_add_to_figure=False)
    pic2.add_axes(ax)

    ax.set_zlabel('time', fontdict={'size': 10, 'color': 'blue'})
    ax.set_ylabel('delete', fontdict={'size': 10, 'color': 'blue'})
    ax.set_xlabel('search', fontdict={'size': 10, 'color': 'blue'})

    X = np.array(x1)
    Y = np.array(x2)
    Z = np.array(buffer2)

    ax.plot_trisurf(X, Y, Z, cmap='spring')

    plt.suptitle("BTree", color='r', size=15)
    plt.savefig("BTree.png",dpi = 400)
    plt.show()

    pic3 = plt.figure()
    ax = Axes3D(pic3,auto_add_to_figure=False)
    pic3.add_axes(ax)

    ax.set_zlabel('time/s', fontdict={'size': 10, 'color': 'blue'})
    ax.set_ylabel('delete', fontdict={'size': 10, 'color': 'blue'})
    ax.set_xlabel('search', fontdict={'size': 10, 'color': 'blue'})

    X = np.array(x1)
    Y = np.array(x2)
    Z = np.array(buffer3)

    ax.plot_trisurf(X, Y, Z, cmap='spring')

    plt.suptitle("RBTree", color="r", size=15)
    plt.savefig("RBTree.png",dpi = 400)
    plt.show()
    result.close()
def picture2(k, j, b):
    while k <= j:
        test(k, k, k)
        os.system(".\\test.exe")
        k *= 10
    p = re.compile(r"\d+\.\d+")
    array1, array2, array3 = [], [], []
    res = open("result.txt", "r")
    i = 1
    while True:
        if i == 4:
            i = 1
        line = res.readline()
        if not line:
            break
        l = re.findall(p, line)
        if l != []:
            if i == 1:
                array1.extend(l)
            elif i == 2:
                array2.extend(l)
            else:
                array3.extend(l)
            i += 1
    y1 = [float(array1[i]) for i in range(0, len(array1), 3)]
    y2 = [float(array2[i]) for i in range(0, len(array2), 3)]
    y3 = [float(array3[i]) for i in range(0, len(array3), 3)]
    x = []
    for i in y1:
        x.append(int(10 ** b))
        b += 1
    plt.plot(x, y1, label='AVLTree', linewidth=3, marker='+',
            markerfacecolor='black', markersize=4)
    plt.plot(x, y2, label='BTree', linewidth=3, marker='+',
            markerfacecolor='black', markersize=4)
    plt.plot(x, y3, label='RBTree', linewidth=3, marker='+',
            markerfacecolor='black', markersize=4)
    plt.xlabel('Operation Ratio')
    plt.ylabel('time')
    plt.title('Insert Comparison')
    plt.legend()
    plt.savefig("Insert Comparison.png", dpi=600)
    plt.show()
    y1 = [float(array1[i]) for i in range(1, len(array1), 3)]
    y2 = [float(array2[i]) for i in range(1, len(array2), 3)]
    y3 = [float(array3[i]) for i in range(1, len(array3), 3)]
    x = []
    for i in y1:
        x.append(int(10 ** b))
        b += 1
    plt.plot(x, y1, label='AVLTree', linewidth=3, marker='+',
            markerfacecolor='black', markersize=4)
    plt.plot(x, y2, label='b_tree', linewidth=3, marker='+',
            markerfacecolor='black', markersize=4)
    plt.plot(x, y3, label='red_black_tree', linewidth=3, marker='+',
            markerfacecolor='black', markersize=4)
    plt.xlabel('Operation Ratio')
    plt.ylabel('time')
    plt.title('Search Comparison')
    plt.legend()
    plt.savefig("Search Comparison.png", dpi=600)
    plt.show()
    y1 = [float(array1[i]) for i in range(2, len(array1), 3)]
    y2 = [float(array2[i]) for i in range(2, len(array2), 3)]
    y3 = [float(array3[i]) for i in range(2, len(array3), 3)]
    x = []
    for i in y1:
        x.append(int(10 ** b))
        b += 1
    plt.plot(x, y1, label='AVLTree', linewidth=3, marker='+',
            markerfacecolor='black', markersize=4)
    plt.plot(x, y2, label='b_tree', linewidth=3, marker='+',
            markerfacecolor='black', markersize=4)
    plt.plot(x, y3, label='RBTree', linewidth=3, marker='+',
            markerfacecolor='black', markersize=4)
    plt.xlabel('Operation Ratio')
    plt.ylabel('time')
    plt.title('Delete Comparison')
    plt.legend()
    plt.savefig("Delete Comparison.png", dpi=600)
    plt.show()
    res.close()

#_test(3, 10000000)
#picture3(500000, 5000, 5000)
#picture2(10000, 10000000, 4)