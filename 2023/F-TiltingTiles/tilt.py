def print_board(board):
    """打印棋盘状态"""
    for row in board:
        print(' '.join(row))
    print()

def move(board, direction):
    """
    对棋盘执行一次倾斜操作
    board: 二维列表，每个元素为 '.'（空）、'#'（障碍）、'o'（滑块）
    direction: 'up', 'down', 'left', 'right'
    返回新的棋盘状态
    """
    h=len(board)
    w=len(board[0])
    new_board = [row[:] for row in board]  # 创建副本

    # 根据倾斜方向确定遍历顺序
    if direction == 'up':
        rows = range(h)
        cols = range(w)
        dr, dc = -1, 0
    elif direction == 'down':
        rows = range(h-1, -1, -1)
        cols = range(w)
        dr, dc = 1, 0
    elif direction == 'left':
        rows = range(h)
        cols = range(w)
        dr, dc = 0, -1
    elif direction == 'right':
        rows = range(h)
        cols = range(w-1, -1, -1)
        dr, dc = 0, 1
    else:
        raise ValueError("方向必须是 'up', 'down', 'left', 'right'")

    moved = True
    while moved:  # 多次扫描直到所有滑块都停止
        moved = False
        for r in rows:
            for c in cols:
                if new_board[r][c].isalpha() or new_board[r][c].isdigit():  # 找到滑块
                    chess=new_board[r][c]
                    nr, nc = r + dr, c + dc
                    # 检查是否可以移动
                    if 0 <= nr < h and 0 <= nc < w and new_board[nr][nc] == '.':
                        new_board[nr][nc] = chess
                        new_board[r][c] = '.'
                        moved = True
    return new_board

def main():
    # 初始棋盘示例：5x5，'#' 为障碍，'o' 为滑块，'.' 为空
    # board = [
    #     ['.', '.', '.', '.', '.'],
    #     ['.', '1', '.', '.', '.'],
    #     ['.', '.', '.', '.', '2'],
    #     ['.', '.', '.', '.', '.'],
    #     ['3', '.', '.', '.', '.']
    # ]

    stt='''
.r..
rgyb
.b..
.yr.
'''
    stt='''
I y A z L y
B z x F z .
y z x E K .
C z D z . .
J y . . . .
'''


# .r..
# rgyb
# .b..
# .yr.
#
# l d r u
#
# yrbr
# ..yr
# ...g
# ...b


    # 把stt转换成二维数组
    board = [list(line.strip().split()) for line in stt.strip().split('\n')]


    print("初始棋盘:")
    print_board(board)

    while True:
        cmd=input("diretcion:")
        if cmd=="reset":
            print('--------------------')
            print('--------------------')
            print('--------------------')
            board = [list(line.strip()) for line in stt.strip().split('\n')]
            print_board(board)
            continue
        m={"l":"left","r":"right","u":"up","d":"down"}
        board=move(board,m[cmd])
        print_board(board)
        print('-------------------')

    # 测试向右倾斜
    # board = move(board, 'right')
    # print("向右倾斜后:")
    # print_board(board)
    #
    # # 测试向下倾斜
    # board = move(board, 'down')
    # print("向下倾斜后:")
    # print_board(board)

if __name__ == "__main__":
    main()
