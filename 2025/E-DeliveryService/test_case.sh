#!/bin/bash

if [ ! -f "main" ]; then
 g++ -g main.cpp -o main
elif [ $(stat -c %Y main.cpp) -gt $(stat -c %Y main) ]; then
 g++ -g main.cpp -o main
fi

if [[ $1 == "case" ]];then
  correct=$(cat ./data/$2.ans)
  ans=$(cat ./data/$2.in |./main)
  if [ "$ans" != "$correct" ];then
    echo -e "\033[31mfailed\033[0m"
    echo "failed"
    diff <(echo "$ans") <(echo "correct") |less # > diff.txt
    echo "$ans" > ./data/my_ans
#    cat diff.txt | less
#    echo "correct " $correct
#    echo "ans " $ans
  else
    echo -e "\033[32msucc\033[0m"
  fi
  exit
fi

if [[ $1 == "prev" ]]; then
  cat ./data/my_ans
  exit
fi

input=$(ls ./data/*.in)
count=0
for inf in $input;do
#  if(( $count >= 2 ));then exit; fi
  echo $inf
  filename=$(basename -- "$inf")
  #if [ "$filename" == "01.in" ];then continue ;fi
  extension="${filename##*.}"
  ans_filename="${filename%.*}"".ans"
  ans=$(cat $inf |./main)
  echo "$ans" > ./data/my_ans
  correct=$(cat ./data/$ans_filename)
  if [ "$ans" != "$correct" ];then
    echo -e "\033[31mfailed\033[0m"
#    echo "correct " $correct
#    echo "ans " $ans
#    exit
  else
    echo -e "\033[32msucc\033[0m"
  fi
  count=$(( $count + 1 ))
done