#!/usr/bin/python3
import sys
import os
import random
built_in_test_cases = [
"""10 10
........X.
..........
..........
......X...
..........
.X.......X
..........
X........X
..........
X....XX...
0 0""",
"""10 10
.....X...X
.........X
X..X......
X.X.......
..X.......
..........
..........
..........
..........
X.........
0 0""",
"""10 10
.XX.......
...X...XX.
..........
...X......
.X......X.
..X....X..
XX....X...
..X...X...
..X.X.X.X.
....X.....
0 9""",
"""10 10
.......X..
X.....X...
X...X.....
...X......
..........
.X.X.X....
..X.X..X..
.X.X.....X
....X.....
.....X....
4 9""",
"""20 20
X...........X.......
..XX.............X.X
.X......X.........X.
....X.X...X...X.....
....X.X.........X...
X.XX...........X.X..
...X.............XX.
...XX...X..X..X.X...
...X.X...X...X......
.............XX.X...
.X............X.....
.X..X.........X....X
X.X.X..X...X..X.....
.X....X.X......X....
...X.........X..X...
...X.X...X..........
..X.XX.X......XXXX..
.X.X...............X
XXX..X....X.XX..X...
.X...X.XX........X.X
0 10"""
]
mine_rate=0.133
total_round=0
win_round=0
for i in range(0,1):
	for data in built_in_test_cases:
		fn=open("tmp/test.in","w")
		fn.write(data)
		fn.close()
		os.system("build/src/client < tmp/test.in > tmp/test.out")
		fn=open("tmp/test.out","r")
		lines=fn.readlines()
		fn.close()
		print(lines)
		total_round+=1
		if lines[0]=='YOU WIN!\n':
			win_round+=1
		print("win rate: "+str(win_round/total_round),win_round,total_round)

input("Press Enter to continue...")

while True:
	# randomly generate n,m in [2,30]
	n=random.randint(20,30)
	m=random.randint(20,30)
	# print(n,m)
	# randomly generate mine_rate in [0,1]
	#mine_rate=random.random()
	# generate a random map
	map=[]
	for i in range(n):
		map.append([])
		for j in range(m):
			if random.random()<mine_rate:
				map[i].append('X')
			else:
				map[i].append('.')
	# generate a random start point with no mine
	sx=random.randint(0,n-1)
	sy=random.randint(0,m-1)
	while map[sx][sy]=='X':
		sx=random.randint(0,n-1)
		sy=random.randint(0,m-1)
	# output the map
	fn=open("tmp/test.in","w")
	fn.write(str(n)+" "+str(m)+"\n")
	for i in range(n):
		for j in range(m):
			fn.write(map[i][j])
		fn.write("\n")
	fn.write(str(sx)+" "+str(sy))
	fn.close()
	# run the program
	os.system("build/src/client < tmp/test.in > tmp/test.out")
	# read the output
	fn=open("tmp/test.out","r")
	lines=fn.readlines()
	fn.close()
	# check the output
	print(lines)
	total_round+=1
	if lines[0]=='YOU WIN!\n':
		win_round+=1
	print("win rate: "+str(win_round/total_round),win_round,total_round)