from defs cimport dbl

cdef extern from "par.h":
    cdef struct par3:
        size_t l[3]
        dbl b[3]
    int par3_size(const par3 *par)
