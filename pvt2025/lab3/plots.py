import matplotlib.pyplot as plt


process_counts = [8, 16, 24]
k28_times = [43.029222, 46.721068, 46.350524]
k10_times = [0.127488, 0.079496, 0.078333]


plt.figure(figsize=(12, 6))

plt.subplot(1, 2, 1)
plt.plot(process_counts, k28_times, marker='o', linestyle='-', color='b')
plt.title('28k элементов', fontsize=14)
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
