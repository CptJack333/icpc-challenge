import sys

# 设置递归深度限制，防止在处理长数字时栈溢出
sys.setrecursionlimit(5000)

def solve():
    # 问题参数
    divisor = 2
    upper_bound = 1000  # 对应字面值 10e18

    ans=[]

    for desired_digit in range(10):
        s_limit = str(upper_bound)
        n_len = len(s_limit)

        # memo[pos][rem] 用于存储 (最大1的个数, 对应的最小数值字符串)
        # 仅在 limit=False 时记忆化
        memo = {}

        def dfs(idx, rem, limit,desired_digit):
            """
            idx: 当前处理的位数索引（从0到n_len）
            rem: 当前数值模 divisor 的余数
            limit: 当前是否受到上限约束
            """
            # 基本情况：处理完所有位数
            if idx == n_len:
                # 如果余数为0，说明是1514的倍数，找到有效解，1的个数为0
                # 否则不是倍数，返回无效标记
                if rem == 0: # 判断是否divisor的整数倍
                    return (0, "")
                else:
                    return (-1, "")

            # 记忆化查询：如果不受限且状态已计算，直接返回
            if not limit and (idx, rem) in memo:
                return memo[(idx, rem)]

            # 确定当前位可以填的数字上限
            max_digit = int(s_limit[idx]) if limit else 9

            best_count = -1
            best_num_str = ""

            # 尝试填入数字 d (从0到max_digit)
            # 由于我们需要“出现1最多的数”，若有多个解，通常取数值最小或字典序最小
            # 这里的搜索顺序 0 -> 9 保证了在count相同时，保留数值较小的解
            for d in range(max_digit + 1):
                next_limit = limit and (d == max_digit)
                next_rem = (rem * 10 + d) % divisor

                # 递归调用
                count, suffix = dfs(idx + 1, next_rem, next_limit,desired_digit)

                # 如果子问题是有效解
                if count != -1:
                    current_count = count + (1 if d == desired_digit else 0)

                    # 更新最优解：寻找 count 更大的；如果 count 相同，保留字典序更小的（即最先找到的）
                    if current_count > best_count:
                        best_count = current_count
                        best_num_str = str(d) + suffix

            # 如果没有任何有效解，返回 (-1, "")
            if best_count == -1:
                return (-1, "")

            # 如果这一层不受限，存入备忘录
            if not limit:
                memo[(idx, rem)] = (best_count, best_num_str)

            return (best_count, best_num_str)

        # 开始搜索
        # 初始状态：索引0，余数0，受限
        max_ones, result_num = dfs(0, 0, True,desired_digit)
        ans.append((max_ones, result_num))
        # 结果处理

    for desired_digit in range(10):
        max_ones, result_num=ans[desired_digit]
        print(f"数字{desired_digit}")
        if max_ones == -1:
            print("在给定范围内未找到符合条件的数。")
        else:
            # 去除可能存在的前导零（虽然在我们的最优解选择逻辑中，前导零不影响1的计数，
            # 且字典序最小会倾向于前导零，但输出时应转为整数）
            final_num = int(result_num)
            print(f"出现{desired_digit}最多的数是: {final_num}")
            print(f"该数中包含的{desired_digit}的个数: {max_ones}")

if __name__ == "__main__":
    solve()




