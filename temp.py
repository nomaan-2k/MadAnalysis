import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt('input.dat',
                     skip_header=0,
                     skip_footer=0,
                     dtype=float,
                     delimiter='  ')

print(data[0][2])
pX=data[:,0]

print(pX)
plt.hist(pX,10000)
plt.show()
