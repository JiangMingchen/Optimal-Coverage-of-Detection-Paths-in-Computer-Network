import sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.pylab as plab
DFS_path_num = [2,8,10,20,18,23,27,33,32,40]
SA_path_num = [2,5,6,11,12,13,17,23,24,23]
EP_path_num = [2,5,6,11,12,13,17,23,24,23]

EP_var = [110.25,453.76,610.139,385.884,403.076,402.544,388.477,308.722,472.957,491.270]
SA_var = [72.25,58.96,266.139,220.066,115.576,390.237,254.242,230.896,369.54,465.618]
x = range(10)
graph_name = ['10_UG','20_UG','30_UG','40_UG','50_UG','60_UG','70_UG','80_UG','90_UG','100_UG']
plt.figure(figsize=(10,5))
plt.title("variance of path")
plt.xticks(x, graph_name[0:10],color='black',rotation=60)
plt.xlabel("graph")
plt.ylabel("variance")
plt.plot(x,EP_var,'-',label="EP")
plt.plot(x,SA_var,'-',color='r',label="SA")
plt.legend()
plt.show()

x = np.arange(10)
total_width, n = 0.8, 3
width = total_width / n
x = x - (total_width - width) / 2

plt.title("number of paths")
plt.xlabel("graph")
plt.ylabel("number")
plt.bar(x, DFS_path_num,  width=width, label='DFS',tick_label=graph_name)
plt.xticks(rotation = 60)
plt.bar(x + width, EP_path_num, width=width, label='EP',tick_label=graph_name)
plt.bar(x + 2 * width, SA_path_num, width=width, label='SA',tick_label=graph_name)
plt.legend()
plt.show()