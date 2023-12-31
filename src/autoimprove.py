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
0 10""",
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
3 6""",
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
4 12"""
]
mine_rate=0.133

def ATest():
	total_round=0
	win_round=0
	for i in range(0,10):
		for data in built_in_test_cases:
			fn=open("tmp/test.in","w")
			fn.write(data)
			fn.close()
			os.system("tmp/client < tmp/test.in > tmp/test.out")
			fn=open("tmp/test.out","r")
			lines=fn.readlines()
			fn.close()
			print(lines)
			total_round+=1
			if len(lines)>0:
				if lines[0]=='YOU WIN!\n':
					win_round+=1
			print("win rate: "+str(win_round/total_round),win_round,total_round)

	#input("Press Enter to continue...")

	for loop_cnt in range(10000):
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
		os.system("tmp/client < tmp/test.in > tmp/test.out")
		# read the output
		fn=open("tmp/test.out","r")
		lines=fn.readlines()
		fn.close()
		# check the output
		print(lines)
		total_round+=1
		if len(lines)>0:
			if lines[0]=='YOU WIN!\n':
				win_round+=1
		print("win rate: "+str(win_round/total_round),win_round,total_round)
	return win_round/total_round

value1_list=[0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.25,2.5,3]
value2_list=[0.25,0.5,0.75,1,1.25,1.5,1.75,2,2.25,2.5,3]
def GenerateSource(value1,value2):
	# read src/include/client.h
	fn=open("src/include/client.h","r")
	lines=fn.readlines()
	fn.close()
	# change the 441 th line to "const double m_pow_edge = "+str(value1)+";"
	lines[440]="const double m_pow_edge = "+str(value1)+";\n"
	# change the 442 th line to "const double m_pow_coner = "+str(value2)+";"
	lines[441]="const double m_pow_coner = "+str(value2)+";\n"
	# write the source to "tmp/client.h"
	fn=open("tmp/client.h","w")
	fn.writelines(lines)
	fn.close()
	# copy src/include/server.h to tmp/server.h
	os.system("cp src/include/server.h tmp/server.h")
	# copy src/advanced.cpp to tmp/advanced.cpp
	os.system("cp src/advanced.cpp tmp/advanced.cpp")
	# compile the source tmp/advanced.cpp to tmp/client
	os.system("g++ -std=c++11 -g -O2 -Wall -Wextra tmp/advanced.cpp -o tmp/client")

for value1 in value1_list:
	for value2 in value2_list:
		GenerateSource(value1,value2)
		print("value1="+str(value1)+", value2="+str(value2))
		ratio=ATest()
		print("win rate: "+str(ratio))
		print("")
		# write to tmp/log.txt
		fn=open("tmp/log.txt","a")
		fn.write("value1="+str(value1)+", value2="+str(value2)+"\n")
		fn.write("win rate: "+str(ratio)+"\n")
		fn.write("\n")
		fn.close()