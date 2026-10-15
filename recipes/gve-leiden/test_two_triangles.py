# Two triangles joined by one edge: GVE-Leiden must find the two triangles, at 1 and 2 threads.
import os, subprocess
import numpy as np

edges = [(0, 1), (1, 2), (0, 2), (3, 4), (4, 5), (3, 5), (2, 3)]
rows = [[] for _ in range(6)]
for u, v in edges:
    rows[u].append(v)
    rows[v].append(u)
indptr = np.cumsum([0] + [len(r) for r in rows]).astype(np.int64)
indices = np.concatenate([sorted(r) for r in rows]).astype(np.int32)
with open("g.bin", "wb") as f:
    np.array([6, len(indices)], np.int64).tofile(f)
    indptr.tofile(f)
    indices.tofile(f)
    np.ones(len(indices), np.float32).tofile(f)
for t in ("1", "2"):
    subprocess.run(["gve-leiden", "g.bin", "l.bin"], check=True, env={**os.environ, "OMP_NUM_THREADS": t})
    l = np.fromfile("l.bin", np.int32)
    assert len(set(l[:3])) == 1 and len(set(l[3:])) == 1 and l[0] != l[3], l
print("ok")
