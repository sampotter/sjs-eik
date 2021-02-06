from defs cimport bool, dbl
from geom cimport rect3, Rect3, tri3

cdef extern from "mesh2.h":
    ctypedef struct mesh2:
        pass
    void mesh2_alloc(mesh2 **mesh)
    void mesh2_dealloc(mesh2 **mesh)
    void mesh2_init_from_binary_files(mesh2 *mesh, const char *verts_path, const char *faces_path)
    void mesh2_deinit(mesh2 *mesh)
    size_t mesh2_get_num_points(const mesh2 *mesh)
    dbl *mesh2_get_points_ptr(const mesh2 *mesh)
    size_t mesh2_get_num_faces(const mesh2 *mesh)
    size_t *mesh2_get_faces_ptr(const mesh2 *mesh)
    rect3 mesh2_get_bounding_box(const mesh2 *mesh)
    void mesh2_get_centroid(const mesh2 *mesh, size_t i, dbl *centroid)
    void mesh2_get_vertex(const mesh2 *mesh, size_t i, size_t j, dbl *v)
    bool mesh2_tri_bbox_overlap(const mesh2 *mesh, size_t i, const rect3 *bbox)
    tri3 mesh2_get_tri(const mesh2 *mesh, size_t i)

cdef class Mesh2:
    cdef:
        mesh2 *_mesh

    cdef mesh2 *get_mesh_ptr(self)
