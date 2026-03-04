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

# 创建一个大的Canvas覆盖整个frame
canvas = tk.Canvas(frame, width=500, height=500, bg="lightgray", highlightthickness=0)
canvas.pack(fill=tk.BOTH, expand=True)

# 存储圆形ID和对应的编号
circle_ids = []

# 点击事件处理函数
def on_circle_click(event):
    # 获取点击位置的所有对象
    items = canvas.find_closest(event.x, event.y)
    if items:
        item_id = items[0]
        # 查找该圆形的编号
        for i, (circle_id, number) in enumerate(circle_ids):
            if circle_id == item_id:
                messagebox.showinfo("圆形点击", f"你点击了第 {number} 个圆形")
                break

# 绑定点击事件
canvas.bind("<Button-1>", on_circle_click)

# 在框体内画6x6个蓝色圆形
circle_number = 1
for i in range(ballon_row_num):
    for j in range(ballon_col_num):
        # 计算每个圆的中心坐标
        x = 50 + j * 80  # 列索引 * 间距
        y = 50 + i * 80  # 行索引 * 间距
        # 绘制圆形并保存ID
        radius=30
        circle_id = canvas.create_oval(x-radius, y-radius, x+radius, y+radius, fill="blue")
        # 存储圆形ID和编号
        circle_ids.append((circle_id, circle_number))
        # 递增编号
        circle_number += 1

# 3. 运行主循环（界面常驻）
root.mainloop()
