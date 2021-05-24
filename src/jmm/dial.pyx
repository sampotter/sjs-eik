import array
import numpy as np

cdef class _Dial3:
    def __cinit__(self, stype stype, int[:] shape, dbl h):
        dial3_alloc(&self.dial)
        dial3_init(self.dial, stype, &shape[0], h)

        self.shape[0] = shape[0]
        self.shape[1] = shape[1]
        self.shape[2] = shape[2]

        # Strides that haven't been scaled by the size of the
        # underlying type
        cdef Py_ssize_t base_strides[3]
        base_strides[2] = 1
        base_strides[1] = self.shape[2]
        base_strides[0] = self.shape[2]*self.shape[1]

        self.Toff_view = ArrayView(3)
        self.Toff_view.readonly = False
        self.Toff_view.ptr = <void *>dial3_get_Toff_ptr(self.dial)
        self.Toff_view.shape[0] = self.shape[0]
        self.Toff_view.shape[1] = self.shape[1]
        self.Toff_view.shape[2] = self.shape[2]
        self.Toff_view.strides[0] = sizeof(dbl)*base_strides[0]
        self.Toff_view.strides[1] = sizeof(dbl)*base_strides[1]
        self.Toff_view.strides[2] = sizeof(dbl)*base_strides[2]
        self.Toff_view.format = 'd'
        self.Toff_view.itemsize = sizeof(dbl)

        self.xsrc_view = ArrayView(4)
        self.xsrc_view.readonly = False
        self.xsrc_view.ptr = <void *>dial3_get_xsrc_ptr(self.dial)
        self.xsrc_view.shape[0] = self.shape[0]
        self.xsrc_view.shape[1] = self.shape[1]
        self.xsrc_view.shape[2] = self.shape[2]
        self.xsrc_view.shape[3] = 3
        self.xsrc_view.strides[0] = 4*sizeof(dbl)*base_strides[0]
        self.xsrc_view.strides[1] = 4*sizeof(dbl)*base_strides[1]
        self.xsrc_view.strides[2] = 4*sizeof(dbl)*base_strides[2]
        self.xsrc_view.strides[3] = sizeof(dbl)
        self.xsrc_view.format = 'd'
        self.xsrc_view.itemsize = sizeof(dbl)

        self.state_view = ArrayView(3)
        self.state_view.readonly = False
        self.state_view.ptr = <void *>dial3_get_state_ptr(self.dial)
        self.state_view.shape[0] = self.shape[0]
        self.state_view.shape[1] = self.shape[1]
        self.state_view.shape[2] = self.shape[2]
        self.state_view.strides[0] = sizeof(state)*base_strides[0]
        self.state_view.strides[1] = sizeof(state)*base_strides[1]
        self.state_view.strides[2] = sizeof(state)*base_strides[2]
        self.state_view.format = 'i'
        self.state_view.itemsize = sizeof(state)

    def __dealloc__(self):
        dial3_deinit(self.dial)
        dial3_dealloc(&self.dial)

    def add_point_source(self, int[:] ind0, dbl Toff):
        dial3_add_point_source(self.dial, &ind0[0], Toff)

    def add_boundary_points(self, int[::1, :] inds):
        # TODO: handle the case where inds is in a weird format
        if inds.shape[0] != 3:
            raise Exception('inds must be an 3xN array')
        dial3_add_boundary_points(self.dial, &inds[0, 0], inds.shape[1])

    def step(self):
        dial3_step(self.dial)

    def solve(self):
        dial3_solve(self.dial)

    @property
    def Toff(self):
        return self.Toff_view

    @property
    def xsrc(self):
        return self.xsrc_view

    @property
    def state(self):
        return self.state_view

class Dial:
    def __init__(self, stype, shape, h):
        self.shape = shape
        self.h = h
        if len(self.shape) == 3:
            self._dial = _Dial3(stype.value, array.array('i', [*shape]), h)
        else:
            raise Exception('len(shape) == %d not supported yet' % len(shape))

    def add_point_source(self, ind0, Toff):
        self._dial.add_point_source(array.array('i', [*ind0]), Toff)

    def add_boundary_points(self, inds):
        self._dial.add_boundary_points(inds)

    def step(self):
        self._dial.step()

    def solve(self):
        self._dial.solve()

    @property
    def _x(self):
        x = np.linspace(0, self.h*self.shape[0], self.shape[0])
        return x.reshape(self.shape[0], 1, 1)

    @property
    def _y(self):
        y = np.linspace(0, self.h*self.shape[1], self.shape[1])
        return y.reshape(1, self.shape[1], 1)

    @property
    def _z(self):
        z = np.linspace(0, self.h*self.shape[2], self.shape[2])
        return z.reshape(1, 1, self.shape[2])

    @property
    def T(self):
        dx = self._x - self.xsrc[:, :, :, 0]
        dy = self._y - self.xsrc[:, :, :, 1]
        dz = self._z - self.xsrc[:, :, :, 2]
        return self.Toff + np.sqrt(dx**2 + dy**2 + dz**2)

    @property
    def Toff(self):
        return np.asarray(self._dial.Toff)

    @property
    def xsrc(self):
        return np.asarray(self._dial.xsrc)

    @property
    def state(self):
        return np.asarray(self._dial.state)