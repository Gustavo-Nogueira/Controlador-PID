import matplotlib.pyplot as plt
import math

def f(x):
    answer = (-(math.sin(x)*100.0/pow(x,2)) + 0)
    return answer

maxi = -1000000
mini = f(3)
eps = 0.1
data = 3.0
i = 1
new_stage = 2
current_stage = 1

def select_stage():
    if(new_stage > current_stage): bol = True
    else: bol = False
    return bol


bol = select_stage()

while(1):
    print(i)
    y = f(data)
    print(y)
    plt.plot(data, y,'go' ,color = 'green')
    if(bol):
        
        if(y < maxi): 
            if(abs(maxi - mini) < eps): break; 
            mini = 1000000
            bol =  False
        if(y > maxi): maxi = y
        
    else:
        
        if(y > mini):
            if(abs(maxi - mini) < eps): break
            maxi = -1000000
            bol = True
        if(y < mini): mini = y 
    
    print(mini, maxi)
    data += 0.1
    i = i + 1

plt.show()
