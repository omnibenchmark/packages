// gve-leiden GRAPH.bin LABELS.bin: GVE-Leiden (Sahu 2024, puzzlef/leiden-communities-openmp, MIT) on
// a CSR graph, as clust-libreville-rs' bench/gve/leiden_timed.cxx and brrr-gve's gve/leiden.cxx call it.
//
// GRAPH.bin, little-endian: int64 n, int64 m, int64 indptr[n + 1], int32 indices[m], float32
// weights[m]; both directions of every undirected edge. Writes int32 labels[n] to LABELS.bin and
// prints the passes and the time of leidenStaticOmp alone (no graph build, no relabelling).
// Resolution 1, vertices in input order; threads: OMP_NUM_THREADS. GVE has no RNG: with more than one
// thread its result depends on scheduling.
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "inc/main.hxx"

template <class T> static void read(FILE *f, std::vector<T> &v) {
  if (fread(v.data(), sizeof(T), v.size(), f) != v.size()) { perror("gve-leiden: read"); std::exit(1); }
}

int main(int argc, char **argv) {
  if (argc != 3) { fprintf(stderr, "usage: gve-leiden GRAPH.bin LABELS.bin\n"); return 2; }
  FILE *f = fopen(argv[1], "rb");
  if (!f) { perror(argv[1]); return 1; }
  std::vector<int64_t> nm(2);
  read(f, nm);
  const int64_t n = nm[0];
  std::vector<int64_t> indptr(n + 1);
  std::vector<int32_t> indices(nm[1]);
  std::vector<float> weights(nm[1]);
  read(f, indptr); read(f, indices); read(f, weights);
  fclose(f);

  using K = uint32_t;
  DiGraph<K, None, float> x;
  addVerticesU(x, K(0), K(n));
  #pragma omp parallel
  {
    for (int64_t u = 0; u < n; ++u) {
      if (!belongsOmp(K(u))) continue;  // each thread fills only the rows it owns
      for (int64_t j = indptr[u]; j < indptr[u + 1]; ++j) x.addEdge(K(u), K(indices[j]), weights[j]);
    }
  }
  updateOmpU(x);
  auto t = std::chrono::steady_clock::now();
  auto a = leidenStaticOmp(x, {1, 1.0});
  double s = std::chrono::duration<double>(std::chrono::steady_clock::now() - t).count();

  std::vector<int32_t> labels(n);
  for (int64_t u = 0; u < n; ++u) labels[u] = int32_t(a.membership[u]);
  FILE *o = fopen(argv[2], "wb");
  if (!o || fwrite(labels.data(), sizeof(int32_t), n, o) != size_t(n)) { perror(argv[2]); return 1; }
  fclose(o);
  printf("passes %d algo_s %.4f\n", a.passes, s);
}
