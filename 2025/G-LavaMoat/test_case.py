#!/usr/bin/python

import os

# 列出data文件夹下所有*.in文件
input_files = os.listdir('./data')
input_files = [f for f in input_files if f.endswith('.in')]

jump_case=["secret-76-skewed","secret-77-skewed","secret-99-bmerry","secret-z100-bmerry"]
start_from=None

for ifi in input_files:
    #打开data/a.in
    # 去掉ifi的后缀.in
    fname = ifi[:-3]

    if start_from is not None:
        if str(start_from) in fname:
            start_from=None
        else:
            continue

    if ifi in jump_case or fname in jump_case:
        continue

    print(fname)

    #运行cat ifi | ./main，并获取输出
    cmd = "cat ./data/"+ifi+" | ./main"
    out = os.popen(cmd).read()
    #out按每一行划分成数组
    outlines = out.splitlines()
    # print(outlines)

    anfi = fname+".ans"
    f=open('./data/'+anfi, 'r')
    lines = f.readlines()

    total_failed_num=0
    # lines和outlines逐行比对，每一行都是一个浮点数，如果相差大于1e-3，打印failed
    for i in range(len(lines)):
        line = lines[i].strip()
        if line == "impossible" or outlines[i] == "impossible":
            if outlines[i] != line:
                # 输出红色的failed
                print("\033[91mfailed\033[0m")
                print("test case "+str(i))
                print("correct")
                print(line)
                print("ans")
                print(outlines[i])
                total_failed_num+=1
                if total_failed_num>=10:
                    exit(0)
            continue
        f1 = float(line)
        f2 = float(outlines[i])
        if(abs(f1-f2)>1e-3):
            print("\033[91mfailed\033[0m")
            print("correct")
            print(f1)
            print("ans")
            print(f2)
            exit(0)

    # 输出绿色的succ
    print("\033[92msucc\033[0m")
    # exit(0)

