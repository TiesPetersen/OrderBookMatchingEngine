import matplotlib.pyplot as plt

latencies = []

print("Reading latencies from latencies.txt...")

with open('latencies.txt', 'r') as f:
    for line in f:
        parts = line.strip().split()
        if not parts:
            continue
        latency = int(parts[0])
        latencies.append(latency)

print(f"Read {len(latencies)} latency measurements.")

print("Plotting latency distribution...")

plt.figure(figsize=(10, 5))
plt.hist(latencies, bins=50, color='blue', edgecolor='black', alpha=0.7, log=True)
plt.xlabel('Latency (ns)')
plt.ylabel('Frequency (Log Scale)')
plt.title('Order Processing Latency Distribution')
plt.ticklabel_format(style='plain', axis='x', useOffset=False)
plt.tight_layout()
plt.savefig('latencies_hist.png', dpi=300)

print("Latency distribution saved as latencies_hist.png")