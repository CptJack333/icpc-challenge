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
        # 计算每个圆的位置
        x_pos = 50 + j * 80  # Canvas在frame中的x位置
        y_pos = 50 + i * 80  # Canvas在frame中的y位置
        # 创建Canvas并放置
        radius=30
        canvas = tk.Canvas(frame, width=radius*2, height=radius*2, bg="lightgray", highlightthickness=0)
        canvas.place(x=x_pos-radius, y=y_pos-radius)  # 调整位置使圆心对齐
        # 在Canvas内绘制圆形（使用相对坐标）
        canvas.create_oval(0, 0, radius*2, radius*2, fill="blue")

# 3. 运行主循环（界面常驻）
root.mainloop()