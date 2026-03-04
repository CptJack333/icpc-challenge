import tkinter as tk
from random import randint
from tkinter import messagebox

# 1. 创建主窗口
root = tk.Tk()
root.title("Tkinter 极简界面")  # 窗口标题

# 中心添加一个框体
frame = tk.Frame(root, width=500, height=500, bg="lightgray")
frame.place(relx=0.5, rely=0.5, anchor="center")  # 中心对齐



############################global configs###########################################

ballon_row_num=8
ballon_col_num=8
colors=["blue","red","green","yellow","orange","purple"]
radius=30

############################global configs###########################################

# 创建一个大的Canvas覆盖整个frame
frame_width=2*(radius*4/3)*(ballon_col_num)
frame_height=2*(radius*4/3)*ballon_row_num
canvas = tk.Canvas(frame, width=frame_width, height=frame_height, bg="lightgray", highlightthickness=0)
canvas.pack(fill=tk.BOTH, expand=True)

root_window_width=int(frame_width+100)
root_window_height=int(frame_height+100)
root.geometry(f"{root_window_width}x{root_window_height}")  # 窗口大小（宽x高）
# 禁止调整窗口大小
root.resizable(False, False)

# 存储圆形ID和对应的编号
circles = []

# 点击事件处理函数
def get_ballon(event):
    # 获取点击位置的所有对象
    items = canvas.find_closest(event.x, event.y)
    if items:
        item_id = items[0]
        # 查找该圆形的编号
        for arr in (circles):
            for i, (circle_id, number, color) in enumerate(arr):
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

    if len(route)>=2:
        for c in route:
            x,y=c
            circles[x-1][y-1][2]="unknown"

        for col in range(ballon_col_num):
            i=ballon_row_num-1
            j=ballon_row_num-1
            while(j>=0):
                if circles[j][col][2]!="unknown":
                    circles[i][col][2]=circles[j][col][2]
                    i-=1
                j-=1
            for k in range(i+1):
                circles[k][col][2]="unknown"

        for row in range(ballon_row_num):
            for col in range(ballon_col_num):
                if circles[row][col][2]=="unknown":
                    circles[row][col][2]=colors[ randint(0,len(colors)-1)]

        for row in range(ballon_row_num):
            for col in range(ballon_col_num):
                canvas.itemconfig(circles[row][col][0], fill=circles[row][col][2])

    route.clear()
    for line_id in route_line:
        canvas.delete(line_id)
    route_line.clear()


canvas.bind("<ButtonPress-1>", on_mouse_press)
canvas.bind("<ButtonRelease-1>", on_mouse_release)

route_line=[]

#绑定鼠标移动事件
def on_mouse_move(event):
    if mouse_pressed:
        ret=get_ballon(event)
        if ret==None:
            return
        x,y=ret
        if(route[-1]!=(x,y)):
            prev=route[-1]
            if len(route)>=2:
                pp=route[-2]
                if((x,y)==pp):
                    # print("不能返回上一个点")
                    return
            px,py=prev
            if(abs(x-prev[0])>1 or abs(y-prev[1])>1):
                # print("错误,请点击相邻的圆形")
                return
            if(circles[x-1][y-1][2]!=circles[px-1][py-1][2]):
                # print("错误,请点击相同颜色的圆形")
                return
            route.append((x,y))
            # 用线段把circle连接起来
            line_start_y=(radius*4/3) + (px-1) * 2*(radius*4/3)
            line_start_x=(radius*4/3) + (py-1) * 2*(radius*4/3)
            line_end_y=(radius*4/3) + (x-1) * 2*(radius*4/3)
            line_end_x=(radius*4/3) + (y-1) * 2*(radius*4/3)
            line_id=canvas.create_line(line_start_x, line_start_y, line_end_x, line_end_y, fill="black", width=2)
            route_line.append(line_id)
            print(x,y)

canvas.bind("<Motion>", on_mouse_move)

# 在框体内画6x6个蓝色圆形
circle_number = 0
for i in range(ballon_row_num):
    circles.append([])
    for j in range(ballon_col_num):
        # 计算每个圆的中心坐标
        x = (radius*4/3) + j * 2*(radius*4/3)  # 列索引 * 间距
        y = (radius*4/3) + i * 2*(radius*4/3)  # 行索引 * 间距
        # 绘制圆形并保存ID
        color=colors[ randint(0,len(colors)-1)]
        circle_id = canvas.create_oval(x-radius, y-radius, x+radius, y+radius, fill=color)
        # 存储圆形ID和编号
        circles[i].append([circle_id, circle_number,color])
        # 递增编号
        circle_number += 1
    #     if(j>0):break
    # if(i>0):break
# 3. 运行主循环（界面常驻）
root.mainloop()
