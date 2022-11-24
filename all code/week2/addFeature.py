data = open("data")
dataArr = str(data.read()).split('\n')

output = open('newdata.txt', 'w')
for d in dataArr[1:]:
    data = d.split(',')
    op = data[0] + ',' + data[1] + ',' + str(round(float(data[0]) * float(data[0]), 7)) + ',' + str(
        round(float(data[1]) * float(data[1]), 7)) + ',' + data[2] + '\n'
    output.write(op)

output.close()