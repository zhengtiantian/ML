import csv
import time
import datetime
import numpy as np
import pandas as pd
from sklearn.linear_model import Lasso

map = {}
data = []
header = ['STATION ID', 'TIME', 'TWO WEEKS AHEAD', 'TEN DAYS AHEAD', 'ONE WEEK AHEAD', 'THREE DAYS AHEAD',
          'ONE DAY AHEAD', 'FOUR AHEAD', 'THREE AHEAD', 'TWO AHEAD', 'ONE AHEAD', 'AVAILABLE BIKES', 'ONE LATER','TWO LATER','THREE LATER','FOUR LATER','FIVE LATER',
          'TARGET']
station5_3 = open('station21_15_p60.csv', 'w', newline='')
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

csvfile.close()

data = pd.read_csv("station21_14_p60.csv")
X_train = data.iloc[:, 2:16]
y_train = data.iloc[:, 16]
X_train = np.array(X_train)
y_train = np.array(y_train)

Lasso = Lasso(alpha=0.18)
Lasso.fit(X_train, y_train)

with open('station21_14_p60.csv', 'r') as s21:

    reader = csv.reader(s21)
    for i, row in enumerate(reader):
        if i != 0:
            features = row
            features = list(features)


            target = features[-1]

            del(features[-1])

            predict_data = np.array(features[2:]).reshape(1, -1)


            pre = Lasso.predict(predict_data)

            features.append(round(pre[0],2))

            t = row[1]
            t = datetime.datetime.strptime(t, '%Y-%m-%d %H:%M:%S')

            # target
            target_offset = datetime.timedelta(minutes=60)
            target_time = (t + target_offset).strftime('%Y-%m-%d %H:%M')
            if map.get(target_time) == None:
                continue
            target = map.get(target_time)

            features.append(target[6])


            csvWriter5_3.writerow(features)

# with open('station21_11_p30.csv', 'r') as s21:
#     reader = csv.reader(s21)
#     for i, row in enumerate(reader):
#         if i != 0:
#             features = row
#             features = list(features)
#             target = features[-1]
#
#             del (features[-1])
#
#             predict_data = []
#
#             t = row[1]
#
#             t = datetime.datetime.strptime(t, '%Y-%m-%d %H:%M:%S')
#             offset = datetime.timedelta(minutes=10)
#             t = (t + offset).strftime('%Y-%m-%d %H:%M:%S')
#
#             t = datetime.datetime.strptime(t, '%Y-%m-%d %H:%M:%S')
#             # 14 days ago
#             two_weeks_ahead_offset = datetime.timedelta(days=-14)
#             two_weeks_ahead_time = (t + two_weeks_ahead_offset).strftime('%Y-%m-%d %H:%M')
#             if map.get(two_weeks_ahead_time) == None:
#                 continue
#             two_weeks_ahead = map.get(two_weeks_ahead_time)
#             predict_data.append(two_weeks_ahead[6])
#
#             # 10 days ago
#             ten_day_ahead_offset = datetime.timedelta(days=-10)
#             ten_day_ahead_time = (t + ten_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
#             if map.get(ten_day_ahead_time) == None:
#                 continue
#             ten_day_ahead = map.get(ten_day_ahead_time)
#             predict_data.append(ten_day_ahead[6])
#
#             # 7 days ago
#             seven_day_ahead_offset = datetime.timedelta(days=-7)
#             seven_day_ahead_time = (t + seven_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
#             if map.get(seven_day_ahead_time) == None:
#                 continue
#             seven_day_ahead = map.get(seven_day_ahead_time)
#             predict_data.append(seven_day_ahead[6])
#
#             # 3 days ago
#             three_day_ahead_offset = datetime.timedelta(days=-3)
#             three_day_ahead_time = (t + three_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
#             if map.get(three_day_ahead_time) == None:
#                 continue
#             three_day_ahead = map.get(three_day_ahead_time)
#             predict_data.append(three_day_ahead[6])
#
#             # 1 day ago
#             one_day_ahead_offset = datetime.timedelta(days=-1)
#             one_day_ahead_time = (t + one_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
#             if map.get(one_day_ahead_time) == None:
#                 continue
#             one_day_ahead = map.get(one_day_ahead_time)
#             predict_data.append(one_day_ahead[6])
#
#             predict_data.append(features[8])
#             predict_data.append(features[9])
#             predict_data.append(features[10])
#             predict_data.append(features[11])
#             predict_data.append(features[12])
#
#             predict_data = np.array(predict_data).reshape(1, -1)
#
#             pre = Lasso.predict(predict_data)
#
#             features.append(round(pre[0],2))
#
#             features.append(target)
#
#             csvWriter5_3.writerow(features)



s21.close()
station5_3.close()
# for row in data:
#     features = []
#     t = row[1]
#     # ID
#     features.append(row[0])
#     # TIME
#     features.append(t)
#
#     timeArray = time.strptime(t, "%Y-%m-%d %H:%M:%S")
#     t = datetime.datetime.strptime(t, '%Y-%m-%d %H:%M:%S')


# # 14 days ago
# two_weeks_ahead_offset = datetime.timedelta(days=-14)
# two_weeks_ahead_time = (t + two_weeks_ahead_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(two_weeks_ahead_time) == None:
#     continue
# two_weeks_ahead = map.get(two_weeks_ahead_time)
# features.append(two_weeks_ahead[6])
#
# # 10 days ago
# ten_day_ahead_offset = datetime.timedelta(days=-10)
# ten_day_ahead_time = (t + ten_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(ten_day_ahead_time) == None:
#     continue
# ten_day_ahead = map.get(ten_day_ahead_time)
# features.append(ten_day_ahead[6])
#
# # 7 days ago
# seven_day_ahead_offset = datetime.timedelta(days=-7)
# seven_day_ahead_time = (t + seven_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(seven_day_ahead_time) == None:
#     continue
# seven_day_ahead = map.get(seven_day_ahead_time)
# features.append(seven_day_ahead[6])
#
# # 3 days ago
# three_day_ahead_offset = datetime.timedelta(days=-3)
# three_day_ahead_time = (t + three_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(three_day_ahead_time) == None:
#     continue
# three_day_ahead = map.get(three_day_ahead_time)
# features.append(three_day_ahead[6])
#
# # 1 day ago
# one_day_ahead_offset = datetime.timedelta(days=-1)
# one_day_ahead_time = (t + one_day_ahead_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(one_day_ahead_time) == None:
#     continue
# one_day_ahead = map.get(one_day_ahead_time)
# features.append(one_day_ahead[6])
#
# # 40m ago
# four_ahead_offset = datetime.timedelta(minutes=-120)
# four_ahead_time = (t + four_ahead_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(four_ahead_time) == None:
#     continue
# four_ahead = map.get(four_ahead_time)
# features.append(four_ahead[6])
#
# # 30m ago
# three_ahead_offset = datetime.timedelta(minutes=-90)
# three_ahead_time = (t + three_ahead_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(three_ahead_time) == None:
#     continue
# three_ahead = map.get(three_ahead_time)
# features.append(three_ahead[6])
#
# # 20m ago
# one_ahead_offset = datetime.timedelta(minutes=-60)
# one_ahead_time = (t + one_ahead_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(one_ahead_time) == None:
#     continue
# one_ahead = map.get(one_ahead_time)
# features.append(one_ahead[6])
#
# # 10m ago
# two_ahead_offset = datetime.timedelta(minutes=-30)
# two_ahead_time = (t + one_ahead_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(two_ahead_time) == None:
#     continue
# two_ahead = map.get(two_ahead_time)
# features.append(two_ahead[6])
#
# # now
# features.append(row[6])
#
# # target
# target_offset = datetime.timedelta(minutes=30)
# target_time = (t + target_offset).strftime('%Y-%m-%d %H:%M')
# if map.get(target_time) == None:
#     continue
# target = map.get(target_time)
# features.append(target[6])
