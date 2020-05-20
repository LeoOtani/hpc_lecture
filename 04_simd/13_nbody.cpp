#include <cstdio>
#include <cstdlib>
#include <cmath>

int main() {
  const int N = 8;
  float x[N], y[N], m[N], tmpx[N], tmpy[N], fx[N], fy[N];
  int j[N];
  for(int i=0; i<N; i++) {
    x[i] = drand48();
    y[i] = drand48();
    m[i] = drand48();
    fx[i] = fy[i] = 0;
    j[i] = i;
  }
  for(int i=0; i<N; i++) {
    __m256 xvec = _mm256_load_ps(x);
    __m256 xivec = _mm256_set1_ps(x[i]);
    __m256 yvec = _mm256_load_ps(y);
    __m256 yivec = _mm256_set1_ps(y[i]);
    __m256 mvec = _mm256_load_ps(m);
    __m256 rxvec = _mm256_setzero_ps();
    __m256 rx2vec = _mm256_setzero_ps();
    __m256 ryvec = _mm256_setzero_ps();
    __m256 ry2vec = _mm256_setzero_ps();
    __m256 rvec = _mm256_setzero_ps();
    __m256 rrtvec = _mm256_setzero_ps();
    __m256i ivec = _mm256_set1_ps(i);
    __m256i jvec = _mm256_load_ps(j);
    __m256 one = _m256_set1_ps(1);
    __m256 mask = _mm256_cmp_ps(i, j, _CMP_NEQ_OQ);

    xvec = _mm256_sub_ps(xivec, xvec);
    rxvec = _mm256_blendv_ps(rxvec, xvec, mask);
    yvec = _mm256_sub_ps(yivec, yvec);
    ryvec = _mm256_blendv_ps(ryvec, yvec, mask);
    rx2vec  = _mm256_mul_ps(rxvec, rxvec);
    ry2vec  = _mm256_mul_ps(ryvec, ryvec);
    rvec = _mm256_sum_ps(rx2vec, ry2vec);
    rrtvec = _mm256_sqrt_ps(rvec);
    rvec = _mm256_mul_ps(rvec, rrtvec);
    rvec = _mm256_blendv_ps(one, rvec, mask); //ゼロ除算回避
    rxvec = _mm256_mul_ps(rxvec, mvec);
    rxvec = _mm256_div_ps(rxvec, rvec);
    ryvec = _mm256_mul_ps(ryvec, mvec);
    ryvec = _mm256_div_ps(ryvec, rvec);
    _mm256_store_ps(tmpx, rxvec);
    _mm256_store_ps(tmpy, ryvec);

    //全ての要素の加算がわかりませんでした
    for(int j=0; j<N; j++) {
      fx[i] -= tmpx[j];
      fy[i] -= tmpy[j];
    }
    
    printf("%d %g %g\n",i,fx[i],fy[i]);
  }
}
