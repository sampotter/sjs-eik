#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "jet.h"

typedef struct mesh3 mesh3_s;

typedef struct utetra utetra_s;

void utetra_alloc(utetra_s **cf);
void utetra_dealloc(utetra_s **cf);
void utetra_init(utetra_s *cf, mesh3_s const *mesh, jet3 const *jet,
                 size_t l, size_t l0, size_t l1, size_t l2);
void utetra_reset(utetra_s *cf);
void utetra_solve(utetra_s *cf);
void utetra_get_lambda(utetra_s *cf, dbl lam[2]);
void utetra_set_lambda(utetra_s *cf, dbl const lam[2]);
dbl utetra_get_value(utetra_s const *cf);
void utetra_get_gradient(utetra_s const *cf, dbl g[2]);
void utetra_get_jet(utetra_s const *cf, jet3 *jet);
void utetra_get_lag_mults(utetra_s const *cf, dbl alpha[3]);
int utetra_get_num_iter(utetra_s const *cf);

#ifdef __cplusplus
}
#endif
