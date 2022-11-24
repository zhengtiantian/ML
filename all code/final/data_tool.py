import csv

station21 = []
station5 = []
header = ['STATION ID','TIME','LAST UPDATED','NAME','BIKE STANDS','AVAILABLE BIKE STANDS','AVAILABLE BIKES','STATUS','ADDRESS','LATITUDE','LONGITUDE']
station21 = open('station18.csv','w',newline= '')
station5 = open('station9.csv','w',newline= '')
csvWriter21 = csv.writer(station21, quoting=csv.QUOTE_NONE, escapechar=',')
csvWriter5 = csv.writer(station5, quoting=csv.QUOTE_NONE, escapechar=',')

csvWriter21.writerow(header)
csvWriter5.writerow(header)

with open('dublinbikes_20200101_20200401.csv', 'r') as csvfile:
    reader = csv.reader(csvfile)
    for i, row in enumerate(reader):

        if row[0] == '18':
            csvWriter21.writerow(row)

        if row[0] == '9':
            csvWriter5.writerow(row)

station21.close()
station5.close()
