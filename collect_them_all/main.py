import tkinter as tk
from tkinter import messagebox

# 1. 创建主窗口
root = tk.Tk()
root.title("Tkinter 极简界面")  # 窗口标题
root.geometry("600x600")  # 窗口大小（宽x高）

# 中心添加一个框体
frame = tk.Frame(root, width=500, height=500, bg="lightgray")
frame.place(relx=0.5, rely=0.5, anchor="center")  # 中心对齐

# 禁止调整窗口大小
root.resizable(False, False)

ballon_row_num=6
ballon_col_num=6

# 在框体内画6x6个蓝色圆形
for i in range(ballon_row_num):
    for j in range(ballon_col_num):
        # 计算每个圆的中心坐标
        x = 100 + j * 100  # 列索引 * 间距
        y = 100 + i * 100  # 行索引 * 间距
        # 绘制圆形
        canvas = tk.Canvas(frame, width=50, height=50, bg="lightgray", highlightthickness=0)
        canvas.create_oval(x-25, y-25, x+25, y+25, fill="blue")
        canvas.place(x=x, y=y)

# 3. 运行主循环（界面常驻）
root.mainloop()