s1 = [1, 1, 0, 0, 0, 0, 0, 0]
s2 = [1, 0, 1, 1, 0, 1, 0, 0]
s3 = [0, 0, 0, 1, 1, 0, 1, 0]
s4 = [0, 0, 0, 0, 0, 1, 1, 1]

result = []
for i in range(0, 2):
    for j in range(0, 2):
        for k in range(0, 2):
            for l in range(0, 2):
                result.append(
                    [
                        (a * i + b * j + c * k + d * l) % 2
                        for a, b, c, d in zip(s1, s2, s3, s4)
                    ]
                )
unique_res = [list(x) for x in set(tuple(x) for x in result)]

for x in unique_res:
    print(x)
