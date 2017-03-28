import numpy as np
import math
import operator
f = open('0.txt', 'r')
inputs = f.readlines()
formatinputs = []
name2id = {}
id2name = ['' for x in range(8)]
nowid = 7
for i in inputs:
	t = i.strip().split(' ')
	t2 = []
	t2.append(float(t[0]))
	if t[1] not in name2id.keys():
		nowid += 1
		name2id[t[1]] = nowid
		id2name.append(t[1])
	t2.append(name2id[t[1]])
	t2.append(int(t[2]))
	formatinputs.append(t2)
formatinputs.append([22 * 3600.0, 8, 1])
nowid += 1
useracc = [[0 for x in range(7)] for x in range(nowid)]
probacc = [0 for x in range(7)]
score = [0.0 for x in range(nowid)]
lasttime = 0.0
m = 0.0
for I in formatinputs:
	deltime = I[0] - lasttime
	
	A = [[0.0 for x in range(nowid)] for x in range(nowid)]
	b = [0.0 for x in range(nowid)]
	b[7] = m
	A[7] = [1.0 for x in range(nowid)]
	
	#root
	totout = 1.0
	for i in probacc:
		totout += i
	#A[7][7] = - 1.0 + 1.0 / totout
	for num, i in enumerate(probacc):
		A[num][7] = i / totout
	#root over
	
	#problem
	for pnum in range(7):
		A[pnum][pnum] = - 1.0;
		totout = 1.0
		for i in range(1, probacc[pnum] + 1):
			totout += 1.0 / i
		#if (totout != 0):
		#	A[7][pnum] = A[7][pnum] / totout
		for unum, i in enumerate(useracc):
			if i[pnum] == 1:
				A[unum][pnum] = 1.0 / probacc[pnum] / totout
	#problem over
	
	#user
	for unum in range(8, nowid):
		A[unum][unum] = - 1.0;
		outtot = 0
		for pnum in range(7):
			outtot += useracc[unum][pnum]
		for pnum in range(7):
			if useracc[unum][pnum] == 1:
				A[pnum][unum] = 1.0 / outtot;
	#user over
	
	Amat = np.asmatrix(np.array(A))
	res = np.linalg.solve(Amat, b)
	#if lasttime < 20000:
	#	print A
	#	print [x for x in res[:]]
	for num, i in enumerate(res):
		score[num] += deltime * i
	lasttime += deltime
	probacc[I[2]] += 1
	useracc[I[1]][I[2]] = 1
	m += 1
	
#print score

sortvec = []
for num, i in enumerate(score):
	if num > 7:
		sortvec.append([i, num]);
sortvec.sort(key = lambda x:x[0], reverse = True)
#print sortvec
part = [x for x, y in sortvec]
if len(part) > 32:
	part = part[:32]
usernum = len(part)
total = 0.0
for i in part:
	total += i
total /= usernum
sqr = 0.0
for i in part:
	sqr += (i - total) * (i - total)
sqr = sqr / usernum
std = np.sqrt(sqr)
E = []
Etot = 0.0
for i in part:
	E.append(math.e ** (i / std))
	Etot += E[-1]
award = [0.0 for x in range(nowid)]
for num, i in enumerate(E):
	award[sortvec[num][1]] = 88888.0 / Etot * i
f = open('0.out','w')
for num, i in enumerate(sortvec[:128]):
	f.write(str(num + 1) + ' ' + str(id2name[i[1]]) + ' ' + str(i[0]) + ' ' + str(award[i[1]]) + '\n')