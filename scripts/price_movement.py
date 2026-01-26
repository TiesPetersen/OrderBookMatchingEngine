import matplotlib.pyplot as plt

prices = []
limit_orders = []

with open('price_movement.txt', 'r') as f:
    for line in f:
        parts = line.strip().split()
        if not parts:
            continue
        current_price = float(parts[0])
        prices.append(current_price)
        if len(parts) > 1:
            limit_price = float(parts[1])
            order_type = int(parts[2])  # 0 for buy, 1 for sell
            limit_orders.append((len(prices) - 1, limit_price, current_price, order_type))

plt.figure(figsize=(12, 6))
plt.plot(prices, linewidth=1, label='Base Price', color='grey')

if limit_orders:
    buy_orders = [(i, lp) for i, lp, cp, ot in limit_orders if ot == 0]
    sell_orders = [(i, lp) for i, lp, cp, ot in limit_orders if ot == 1]
    
    if buy_orders:
        xs, ys = zip(*buy_orders)
        plt.scatter(xs, ys, color='green', label='Buy Limit Orders', zorder=3, s=2, alpha=0.8, marker='^')
    if sell_orders:
        xs, ys = zip(*sell_orders)
        plt.scatter(xs, ys, color='red', label='Sell Limit Orders', zorder=3, s=2, alpha=0.8, marker='v')

plt.xlabel('Tick')
plt.ylabel('Base Price')
plt.title('Limit Orders Generated + Base Price Movement')
plt.grid(True, alpha=0.3, linestyle='--')
plt.legend()
plt.ticklabel_format(style='plain', axis='y', useOffset=False)
plt.tight_layout()
plt.savefig('price_movement.png', dpi=300)