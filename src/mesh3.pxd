from defs cimport bool, dbl

from geom cimport rect3

cdef extern from "mesh3.h":
    cdef struct mesh3:
        pass
    void mesh3_alloc(mesh3 **mesh)
    void mesh3_dealloc(mesh3 **mesh)
    void mesh3_init(mesh3 *mesh,
                    dbl *verts, size_t nverts,
                    size_t *cells, size_t ncells)
    void mesh3_deinit(mesh3 *mesh)
    void mesh3_get_bbox(const mesh3 *mesh, rect3 *bbox)
    size_t mesh3_nverts(const mesh3 *mesh)
    int mesh3_nvc(const mesh3 *mesh, size_t i)
    void mesh3_vc(const mesh3 *mesh, size_t i, size_t *vc)
    int mesh3_nvv(const mesh3 *mesh, size_t i)
    void mesh3_vv(const mesh3 *mesh, size_t i, size_t *vv)
    int mesh3_ncc(const mesh3 *mesh, size_t i)
    void mesh3_cc(const mesh3 *mesh, size_t i, size_t *cc)
    void mesh3_cv(const mesh3 *mesh, size_t i, size_t *cv)
    int mesh3_nec(const mesh3 *mesh, size_t i, size_t j)
    void mesh3_ec(const mesh3 *mesh, size_t i, size_t j, size_t *ec)
    bool mesh3_bdc(const mesh3 *mesh, size_t i)
    bool mesh3_bdv(const mesh3 *mesh, size_t i)
    bool mesh3_bde(const mesh3 *mesh, const size_t l[2])
    bool mesh3_bdf(const mesh3 *mesh, const size_t l[3])
    bool mesh3_is_diff_edge(const mesh3 *mesh, const size_t l[2])