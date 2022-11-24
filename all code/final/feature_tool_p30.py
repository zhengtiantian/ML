import csv
import time
import datetime

map = {}
data = []
header = ['STATION ID', 'TIME',
          '7 DAYS MEAN', '3 DAYS MEAN0',
          'TWO WEEKS AHEAD', 'TEN DAYS AHEAD', 'ONE WEEK AHEAD', 'THREE DAYS AHEAD',
          'ONE DAY AHEAD', 'FOUR AHEAD', 'THREE AHEAD', 'TWO AHEAD', 'ONE AHEAD', 'AVAILABLE BIKES', 'TARGET']
station5_3 = open('station21_12_p30_mean.csv', 'w', newline='')
csvWriter5_3 = csv.writer(station5_3, quoting=csv.QUOTE_NONE, escapechar=',')
csvWriter5_3.writerow(header)
with open('station21.csv', 'r') as csvfile:
    reader = csv.reader(csvfile)

    for i, row in enumerate(reader):
        if i != 0:
            data.append(row)
            t = row[1]
            timeArray = time.strptime(t, "%Y-%m-%d %H:%M:%S")
            otherStyleTime = time.strftime("%Y-%m-%d %H:%M", timeArray)
            map[otherStyleTime] = row

    for row in data:
        features = []
        t = row[1]
        # ID
        features.append(row[0])
        # TIME
        features.append(t)

        timeArray = time.strptime(t, "%Y-%m-%d %H:%M:%S")
        t = datetime.datetime.strptime(t, '%Y-%m-%d %H:%M:%S')

        # #HOURS
        # hour = time.strftime("%H", timeArray)
        # features.append(int(hour))
        #
        # #MINIUTS
        # miniuts = time.strftime("%M", timeArray)
        # features.append(int(miniuts))

        # MINIUTS
        # hour = time.strftime("%H", timeArray)
        # miniuts = time.strftime("%M", timeArray)
        # features.append(int(hour) * 60 + int(miniuts))

        # 7 days mean
        days = -1
        values = []
        for i in range(7):
            seven_days_mean_offset = datetime.timedelta(days=-days)
            seven_days_mean_time = (t + seven_days_mean_offset).strftime('%Y-%m-%d %H:%M')
            if map.get(seven_days_mean_time) == None:
                continue
            seven_days_mean = map.get(seven_days_mean_time)
            values.append(seven_days_mean[6])
            days = days - 1

        if len(values) != 7:
            continue

        sum = 0
        for i in values:
            sum = sum + int(i)
        features.append(round(sum / len(values), 2))

        # 3days mean
        days = -1
        values = []
        for i in range(3):
            three_days_mean_offset = datetime.timedelta(days=-days)
            three_days_mean_time = (t + three_days_mean_offset).strftime('%Y-%m-%d %H:%M')
            if map.get(three_days_mean_time) == None:
                continue
            three_days_mean = map.get(three_days_mean_time)
            values.append(three_days_mean[6])
            days = days - 1

        if len(values) != 3:
            continue

        sum = 0
        for i in values:
            sum = sum + int(i)
        features.append(round(sum / len(values), 2))

        # 14 days ago
        two_weeks_ahead_offset = datetime.timedelta(days=-14)
        two_weeks_ahead_time = (t + two_weeks_ahead_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(two_weeks_ahead_time) == None:
            continue
        two_weeks_ahead = map.get(two_weeks_ahead_time)
        features.append(two_weeks_ahead[6])

        # 10 days ago
        ten_day_ahead_offset = datetime.timedelta(days=-10)
        ten_day_ahead_time = (t + ten_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(ten_day_ahead_time) == None:
            continue
        ten_day_ahead = map.get(ten_day_ahead_time)
        features.append(ten_day_ahead[6])

        # 7 days ago
        seven_day_ahead_offset = datetime.timedelta(days=-7)
        seven_day_ahead_time = (t + seven_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(seven_day_ahead_time) == None:
            continue
        seven_day_ahead = map.get(seven_day_ahead_time)
        features.append(seven_day_ahead[6])

        # 3 days ago
        three_day_ahead_offset = datetime.timedelta(days=-3)
        three_day_ahead_time = (t + three_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(three_day_ahead_time) == None:
            continue
        three_day_ahead = map.get(three_day_ahead_time)
        features.append(three_day_ahead[6])

        # 1 day ago
        one_day_ahead_offset = datetime.timedelta(days=-1)
        one_day_ahead_time = (t + one_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(one_day_ahead_time) == None:
            continue
        one_day_ahead = map.get(one_day_ahead_time)
        features.append(one_day_ahead[6])

        # 40m ago
        four_ahead_offset = datetime.timedelta(minutes=-120)
        four_ahead_time = (t + four_ahead_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(four_ahead_time) == None:
            continue
        four_ahead = map.get(four_ahead_time)
        features.append(four_ahead[6])

        # 30m ago
        three_ahead_offset = datetime.timedelta(minutes=-90)
        three_ahead_time = (t + three_ahead_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(three_ahead_time) == None:
            continue
        three_ahead = map.get(three_ahead_time)
        features.append(three_ahead[6])

        # 20m ago
        one_ahead_offset = datetime.timedelta(minutes=-60)
        one_ahead_time = (t + one_ahead_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(one_ahead_time) == None:
            continue
        one_ahead = map.get(one_ahead_time)
        features.append(one_ahead[6])

        # 10m ago
        two_ahead_offset = datetime.timedelta(minutes=-30)
        two_ahead_time = (t + two_ahead_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(two_ahead_time) == None:
            continue
        two_ahead = map.get(two_ahead_time)
        features.append(two_ahead[6])

        # now
        features.append(row[6])

        # target
        target_offset = datetime.timedelta(minutes=30)
        target_time = (t + target_offset).strftime('%Y-%m-%d %H:%M')
        if map.get(target_time) == None:
            continue
        target = map.get(target_time)
        features.append(target[6])

        csvWriter5_3.writerow(features)

csvfile.close()
station5_3.close()
