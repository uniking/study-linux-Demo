from numpy import random

def create_ran_file(filename, eline, ecol):
    output=open(filename, 'w')
    randArray = random.random(size=(eline,ecol))

    for line in randArray:
        i=0
        for elem in line:
            strelem='%f'%elem
            if i is 0:
                output.write(strelem)
            else:
                output.write(' '+strelem)
            i+=1
        output.write('\n')
    output.close()
    return

create_ran_file('test.txt', 5, 4)
