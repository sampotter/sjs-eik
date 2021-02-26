#include <cgreen/cgreen.h>

#include "bmesh.h"
#include "mesh3.h"

Describe(bmesh33);

BeforeEach(bmesh33) {
  double_absolute_tolerance_is(1e-15);
  double_relative_tolerance_is(1e-15);
}

AfterEach(bmesh33) {}

/**
 * This function creates a `bmesh33_s` that approximates the sphere
 * over a tetrahedron mesh that discretizes [-1, 1]^3, where each
 * octant is discretized into five tetrahedra so that the whole thing
 * is perfectly symmetric.
 */
static void
create_approximate_sphere_bmesh33(
  bmesh33_s **bmesh_handle, mesh3_s **mesh_handle, jet3 **jet_handle)
{
  /**
   * First, set up the `mesh3_s` discretizing [-1, 1]^3.
   */

  // Set up the first eight vertices as a template (we'll mirror these
  // onto the other eight octants).
  dbl verts[64][3] = {
    [0] = {0, 0, 0},
    [1] = {0, 0, 1},
    [2] = {0, 1, 0},
    [3] = {0, 1, 1},
    [4] = {1, 0, 0},
    [5] = {1, 0, 1},
    [6] = {1, 1, 0},
    [7] = {1, 1, 1}
  };

  // Explicitly lay out the signs for each reflection of the first
  // eight template vertices onto the remaining seven octans.
  int signs[8][3] = {
    { 1,  1,  1},
    { 1,  1, -1},
    { 1, -1,  1},
    { 1, -1, -1},
    {-1,  1,  1},
    {-1,  1, -1},
    {-1, -1,  1},
    {-1, -1, -1}
  };

  // Iterate through the first eight octants repeatedly, mirroring
  // them onto the remaining octants. There will be duplicate
  // vertices, which is perfectly fine.
  for (int i = 1; i < 8; ++i)
    for (int j = 0; j < 8; ++j)
      for (int k = 0; k < 3; ++k)
        verts[8*i + j][k] = signs[i][k]*verts[j][k];

  // Lay out the template cell, which should be symmetric under
  // permutation of the orthant axes.
  size_t cells[40][4] = {
    [0] = {0, 1, 3, 5}, // 000, 001, 011, 101
    [1] = {0, 2, 3, 6}, // 000, 010, 011, 110
    [2] = {0, 4, 5, 6}, // 000, 100, 101, 110
    [3] = {0, 3, 5, 6}, // 000, 011, 101, 110
    [4] = {3, 5, 6, 7}, // 011, 101, 110, 111
  };

  // Now, repeat the template cell's index list by shifting it for
  // each other orthant. (Nothing fancy needs to happen here.)
  for (int i = 1; i < 8; ++i)
    for (int j = 0; j < 5; ++j)
      for (int k = 0; k < 4; ++k)
        cells[8*i + j][k] = 8*i + cells[j][k];

  mesh3_alloc(mesh_handle);
  mesh3_init(*mesh_handle, &verts[0][0], 64, &cells[0][0], 40, false);

  /**
   * Next, compute the jets for each vertex in `verts`.
   */

  *jet_handle = malloc(64*sizeof(jet3));

  for (int i = 0; i < 8; ++i) {
    // We specially set the jet at (0, 0, 0) to zero to avoid a
    // singularity. This makes the overall approximation worse, but
    // this is just a test...
    (*jet_handle)[8*i] = (jet3) {.f = 0, .fx = 0, .fy = 0, .fz = 0};

    dbl *x;
    for (int j = 1; j < 8; ++j) {
      x = verts[8*i + j];
      jet3 J = {.f = dbl3_norm(x)};
      dbl3_normalized(x, &J.fx);
      (*jet_handle)[8*i + j] = J;
    }
  }

  /**
   * Finally, set up the `bmesh33_s` instance.
   */

  bmesh33_alloc(bmesh_handle);
  bmesh33_init_from_mesh3_and_jets(*bmesh_handle, *mesh_handle, *jet_handle);
}

static void
destroy_approximate_sphere_bmesh33(
  bmesh33_s **bmesh_handle, mesh3_s **mesh_handle, jet3 **jet_handle)
{
  bmesh33_deinit(*bmesh_handle);
  bmesh33_dealloc(bmesh_handle);

  mesh3_deinit(*mesh_handle);
  mesh3_dealloc(mesh_handle);

  free(*jet_handle);
  *jet_handle = NULL;
}

Ensure (bmesh33, approximate_sphere_setup_and_teardown_works) {
  bmesh33_s *bmesh;
  mesh3_s *mesh;
  jet3 *jet;

  create_approximate_sphere_bmesh33(&bmesh, &mesh, &jet);

  assert_that(bmesh33_num_cells(bmesh), is_equal_to(40));

  destroy_approximate_sphere_bmesh33(&bmesh, &mesh, &jet);
}
