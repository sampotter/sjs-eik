import logging
import json
import numpy as np
import pyvista as pv

from jmm.mesh import Mesh3
from jmm.multiple_arrivals import Domain, PointSourceField, MultipleArrivals

logging.basicConfig(level=logging.INFO)
log = logging.getLogger('testing_multiple_arrivals.py')

surf_grid = pv.read(f'sethian_shadow/Building.obj')
grid = pv.read(f'sethian_shadow/Building.vtu')
with open('sethian_shadow/Building.json', 'r') as f:
    info = json.load(f)

extended_cells = grid.cells.reshape(-1, 5)[:, 1:].astype(np.uintp)
extended_verts = grid.points.astype(np.float64)

num_dom_cells = info['num_dom_cells']
num_dom_verts = info['num_dom_points']

domain = Domain(extended_verts, extended_cells, num_dom_verts, num_dom_cells)

src_index = next(l for l in range(num_dom_verts) if not domain.mesh.bdv(l))
num_arrivals = 5

log.info('computing %d arrivals starting from index %d', num_arrivals, src_index)

field = PointSourceField(domain, src_index)

multiple_arrivals = MultipleArrivals(domain, field, num_arrivals)