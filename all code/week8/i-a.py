import numpy as np
from PIL import Image


def simpleCNN(input, kernel):
    output = []
    k_l = len(kernel)
    k_w = len(kernel[0])
    i_l = len(input)
    i_w = len(input[0])
    for i in range(0, i_l - 1):
        if i + k_l > i_l:
            break
        line = []
        for j in range(0, i_w - 1):
            if j + k_w > i_w:
                continue
            product = 0
            for ki in range(0, k_l - 1):
                for kj in range(0, k_w - 1):
                    product = product + input[i + ki][j + kj] * kernel[ki][kj]

            line.append(product)
        output.append(line)
    return output


im = Image.open('5.png')
rgb = np.array(im.convert('RGB'))
r = rgb[:, :, 0]
# Image.fromarray(np.uint8(r)).show()

kernel1 = [[-1, -1, -1], [-1, 8, -1], [-1, -1, -1]]
kernel2 = [[0, -1, 0], [-1, 8, -1], [0, -1, 0]]
np.set_printoptions(threshold=np.inf)
np.set_printoptions(precision=1)
np.savetxt('kernel1.txt',np.array(simpleCNN(r, kernel1)),fmt='%.01f')
np.savetxt('kernel2.txt',np.array(simpleCNN(r, kernel2)),fmt='%.01f')
Image.fromarray(np.uint8(np.array(simpleCNN(r, kernel2)))).show()
# print(np.array(simpleCNN(r, kernel1)))