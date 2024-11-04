import matplotlib.pyplot as plt


process_counts = [8, 16, 24]
midpoint_times = [0.002541, 0.002308, 0.001657]
monte_carlo_times = [0.014413, 0.004838, 0.003513]


plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(process_counts, midpoint_times, marker='o', linestyle='-', color='b')
plt.title('Метод средних прямоугольников (Midpoint)', fontsize=14)
plt.xlabel('Количество процессов', fontsize=12)
plt.ylabel('Время выполнения (сек)', fontsize=12)
plt.xticks(process_counts)
plt.grid()

plt.subplot(1, 2, 2)
plt.plot(process_counts, monte_carlo_times, marker='o', linestyle='-', color='r')
plt.title('Метод Монте-Карло (Monte Carlo)', fontsize=14)
plt.xlabel('Количество процессов', fontsize=12)
plt.ylabel('Время выполнения (сек)', fontsize=12)
plt.xticks(process_counts)
plt.grid()


plt.tight_layout()
plt.show()
