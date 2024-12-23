import matplotlib.pyplot as plt

procs = [8, 16, 24, 32]
total_time = [6.962248, 3.707130, 2.457214, 1.918792]
singlecore_total_time = 17.358041
for i in range(len(total_time)):
  total_time[i] = singlecore_total_time / total_time[i]

plt.figure(figsize=(10, 6))
plt.plot(procs, total_time, marker='o', label='Total Time')

for x, y in zip(procs, total_time):
    plt.text(x, y, f'({x}, {y:.2f})', fontsize=10, ha='right', va='bottom')

plt.xlabel('Количество процессов')
plt.ylabel('Ускорение')
plt.title('Зависимость ускорения от количества процессов')

plt.grid(True)
plt.savefig("plot.png", format="png")
plt.show()

