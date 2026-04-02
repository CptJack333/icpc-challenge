def frobenius_dp(coins):
    import math
    from functools import reduce
    # 整体不互质则有无穷多不能凑的数
    if reduce(math.gcd, coins) != 1:
        return float('inf')

    # 找一个上界：硬币最大值² 足够安全
    max_coin = max(coins)
    upper = max_coin * max_coin
    dp = [False] * (upper + 1)
    dp[0] = True

    for i in range(upper + 1):
        if dp[i]:
            for c in coins:
                if i + c <= upper:
                    dp[i + c] = True

    # 从后往前找第一个不能凑的
    for x in range(upper, -1, -1):
        if not dp[x]:
            return x
    return 0

# # 测试
# print(frobenius_dp([3, 5]))      # 7
# print(frobenius_dp([60, 100, 222, 650])) # 1678 (麦乐鸡问题)
# print(frobenius_dp([30, 50, 111, 325])) # 839 (麦乐鸡问题)




import math
from functools import reduce

def get_reachable_under_frobenius(coins):
    # 检查是否整体互质
    g = reduce(math.gcd, coins)
    if g != 1:
        return [], float('inf')  # 不互质 → 有无穷多不能凑的数

    # 安全上界：最大硬币的平方
    max_coin = max(coins)
    upper = max_coin * max_coin

    # dp[x] = x 是否能凑出来
    dp = [False] * (upper + 1)
    dp[0] = True

    for x in range(upper + 1):
        if dp[x]:
            for c in coins:
                if x + c <= upper:
                    dp[x + c] = True

    # 找 Frobenius 数（最大不能凑的数）
    frob = -1
    for x in range(upper, -1, -1):
        if not dp[x]:
            frob = x
            break

    # 所有 < frob 且能凑出的数（不含0可以自己去掉）
    reachable = [x for x in range(frob) if dp[x]]
    return reachable, frob





coins = [60, 100, 222, 650]
coins = [30, 50, 111, 325]
reachable, frob = get_reachable_under_frobenius(coins)

# print("Frobenius 数 =", frob)
# print("小于它且能组成的数：")
# print(reachable)

reachable=[2*n for n in reachable if 2*n<=1000]
print(reachable)

from collections import defaultdict

def analyze_digits(arr):
    # 保存每个数字 0-9 的【最大出现次数】和【对应的数字】
    max_count = {d: 0 for d in range(10)}
    max_num = {d: None for d in range(10)}

    # 遍历数组里每个数
    for num in arr:
        s = str(num)

        # 统计当前数字中 0-9 出现次数
        cnt = defaultdict(int)
        for ch in s:
            d = int(ch)
            cnt[d] += 1

        # 打印：每个数内部 0-9 出现次数
        # print(f"数字 {num} 的数字统计: {dict(cnt)}")

        # 更新全局最大次数
        for d in range(10):
            c = cnt.get(d, 0)
            if c > max_count[d]:
                max_count[d] = c
                max_num[d] = num

    # 输出最终结果
    print("\n===== 整个数组的最终结果 =====")
    for d in range(10):
        print(f"数字 {d} 在单个数字中最多出现 {max_count[d]} 次 → 出现在数字 {max_num[d]} 中")

    return max_count, max_num


analyze_digits(reachable)