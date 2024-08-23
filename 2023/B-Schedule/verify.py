#!/usr/bin/python

import subprocess
import sys
import threading

def execute_bash_command(command):
    result = subprocess.run("bash -c \""+command+"\"", shell=True, capture_output=True, text=True)
    if result.returncode == 0:
        output = result.stdout
        return output

def compatible(a,b):
    # print("compatible")
    a=[i-1 for i in a]
    b=[i-1 for i in b]
    # print(a)
    # print(b)
    flag=[False,False,False,False]
    for i in range(len(a)):
        ind=(a[i])*2+(b[i])
        # print(ind)
        flag[ind]=True
        if flag==[True,True,True,True]:
            return True
    # print(flag)
    return flag==[True,True,True,True]

files=execute_bash_command("ls data/*.in").strip()
for f in files.split():
    # f="data/sample-2.in"
    print(f)
    out=execute_bash_command("cat "+f+" | ./main").strip()
    ans=execute_bash_command("cat "+f[:-3]+".ans").strip()
    # print(ans)
    if ans=="infinity":
        # print(ans)
        # exit()
        if out!="infinity":
            print("failed")
            exit()
        else:
            continue
    out=out.splitlines()
    ans=ans.splitlines()
    isolation=int(out[0])
    isolation_ans=int(ans[0])
    # print(isolation)
    # print(isolation_ans)
    if isolation!=isolation_ans:
        print("failed")
        exit()
    sched=[[0 for col in range(len(out)-1)] for row in range(len(out[1]))]
    for i in range(len(out[1])):
        for j in range(len(out)-1):
            sched[i][j]=int(out[j+1][i])
    # for i in range(len(out[1])):
    #     print(sched[i])
    # sched[1]=[1,1,1,1,1,1]
    threadNum=32
    threadResult=[True for i in range(threadNum)]
    def calcThread(index):
        for i in range(len(out[1])):
            for j in range(i+1,len(out[1])):
                if (i*len(out[1])+j)%threadNum!=index:
                    continue
                if not compatible(sched[i],sched[j]):
                    threadResult[index]=False
                    return
    threads=[]
    for i in range(threadNum):
        t=threading.Thread(target=calcThread, args=(i,))
        threads.append(t)
        t.start()

    for thread in threads:
        thread.join ()

    if threadResult!=[True for i in range(threadNum)]:
        print("failed")
        exit()
    # exit()
    # break
print("succ")
