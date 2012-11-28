def sortfunc(a, b):
	f1 = a[0]+a[1]
	f2 = b[0]+b[1]

	if f1<f2: return -1
	elif f1>f2: return 1
	else: return 0
