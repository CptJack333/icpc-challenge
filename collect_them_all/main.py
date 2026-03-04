import tkinter as tk
from random import randint
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
circles = []

colors=["blue","red","green","yellow"]#,"orange","purple"]


# 点击事件处理函数
def get_ballon(event):
    # 获取点击位置的所有对象
    items = canvas.find_closest(event.x, event.y)
    if items:
        item_id = items[0]
        # 查找该圆形的编号
        for i, (circle_id, number, color) in enumerate(circles):
            if circle_id == item_id:
                #展示第x,y个圆形，而不是number
                x,y= int(number/ballon_col_num)+1, number%ballon_col_num+1
                return x,y

# 绑定点击事件
# 绑定鼠标按下事件和松开事件

mouse_pressed=False
route=[]

def on_mouse_press(event):
    global mouse_pressed
    mouse_pressed=True
    x,y=get_ballon(event)
    route.append((x,y))
    print("鼠标按下",x,y)

def on_mouse_release(event):
    global mouse_pressed
    mouse_pressed=False
    print("鼠标松开 route",route)
    route.clear()


canvas.bind("<ButtonPress-1>", on_mouse_press)
canvas.bind("<ButtonRelease-1>", on_mouse_release)
#绑定鼠标移动事件
def on_mouse_move(event):
    if mouse_pressed:
        x,y=get_ballon(event)
        if(route[-1]!=(x,y)):
            prev=route[-1]
            px,py=prev
            if(abs(x-prev[0])>1 or abs(y-prev[1])>1):
                # print("错误,请点击相邻的圆形")
                return
            if(circles[(x-1)*ballon_col_num+(y-1)][2]!=circles[(px-1)*ballon_col_num+(py-1)][2]):
                # print("错误,请点击相同颜色的圆形")
                return
            route.append((x,y))
            print(x,y)

canvas.bind("<Motion>", on_mouse_move)

# 在框体内画6x6个蓝色圆形
circle_number = 0
for i in range(ballon_row_num):
    for j in range(ballon_col_num):
        # 计算每个圆的中心坐标
        x = 50 + j * 80  # 列索引 * 间距
        y = 50 + i * 80  # 行索引 * 间距
        # 绘制圆形并保存ID
        radius=30
        color=colors[ randint(0,len(colors)-1)]
        circle_id = canvas.create_oval(x-radius, y-radius, x+radius, y+radius, fill=color)
        # 存储圆形ID和编号
        circles.append((circle_id, circle_number,color))
        # 递增编号
        circle_number += 1

# 3. 运行主循环（界面常驻）
root.mainloop()
