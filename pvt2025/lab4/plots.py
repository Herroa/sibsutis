import matplotlib.pyplot as plt


process_counts = [8, 16, 24]
k1_times = [0.673635, 0.434739, 0.316632]
k10_times = [69.685093, 35.316490, 24.733604]


plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(process_counts, k1_times, marker='o', linestyle='-', color='b')
plt.title('1k элементов', fontsize=14)
plt.xlabel('Количество процессов', fontsize=12)
plt.ylabel('Время выполнения (сек)', fontsize=12)
plt.xticks(process_counts)
plt.grid()

plt.subplot(1, 2, 2)
plt.plot(process_counts, k10_times, marker='o', linestyle='-', color='r')
plt.title('10к элементов', fontsize=14)
plt.xlabel('Количество процессов', fontsize=12)
plt.ylabel('Время выполнения (сек)', fontsize=12)
plt.xticks(process_counts)
plt.grid()


plt.tight_layout()
plt.show()
