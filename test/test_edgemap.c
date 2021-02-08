#include <cgreen/cgreen.h>

#include "edgemap.h"

#include "def.h"

Describe(edgemap);
BeforeEach(edgemap) {}
AfterEach(edgemap) {}

Ensure (edgemap, basic_test) {
  edgemap_s *edgemap;
  edgemap_alloc(&edgemap);
  edgemap_init(edgemap, sizeof(int));

  int value = 1;
  edgemap_set(edgemap, make_edge(1, 0), &value);

  value = 2;
  edgemap_set(edgemap, make_edge(2, 0), &value);

  value = 3;
  edgemap_set(edgemap, make_edge(1, 2), &value);

  assert_that(edgemap_get(edgemap, make_edge(0, 1), &value));
  assert_that(value, is_equal_to(1));

  assert_that(edgemap_get(edgemap, make_edge(0, 2), &value));
  assert_that(value, is_equal_to(2));

  assert_that(edgemap_get(edgemap, make_edge(1, 2), &value));
  assert_that(value, is_equal_to(3));

  assert_that(edgemap_get(edgemap, make_edge(1, 0), &value));
  assert_that(value, is_equal_to(1));

  assert_that(edgemap_get(edgemap, make_edge(2, 0), &value));
  assert_that(value, is_equal_to(2));

  assert_that(edgemap_get(edgemap, make_edge(2, 1), &value));
  assert_that(value, is_equal_to(3));

  assert_that(edgemap_get(edgemap, make_edge(0, 3), &value), is_false);
  assert_that(edgemap_get(edgemap, make_edge(1, 1), &value), is_false);
  assert_that(edgemap_get(edgemap, make_edge(2, 5), &value), is_false);

  edgemap_iter_s *iter;
  edgemap_iter_alloc(&iter);
  edgemap_iter_init(iter, edgemap);

  edge_s edge;

  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(0));
  assert_that(edge.l[1], is_equal_to(1));
  assert_that(value, is_equal_to(1));

  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(0));
  assert_that(edge.l[1], is_equal_to(2));
  assert_that(value, is_equal_to(2));

  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(1));
  assert_that(edge.l[1], is_equal_to(2));
  assert_that(value, is_equal_to(3));

  assert_that(edgemap_iter_next(iter, &edge, &value), is_false);

  edgemap_iter_dealloc(&iter);

  edgemap_deinit(edgemap);
  edgemap_dealloc(&edgemap);
}

Ensure (edgemap, iter_test) {
  edgemap_s *edgemap;
  edgemap_alloc(&edgemap);
  edgemap_init(edgemap, sizeof(dbl));

  dbl value;

  // added edge to shadow cutset: l0 = 30 (VALID), l1 = 94 (SHADOW), t = 0
  value = 0;
  edgemap_set(edgemap, make_edge(30, 94), &value);

  // added edge to shadow cutset: l0 = 11 (VALID), l1 = 94 (SHADOW), t = 0
  value = 0;
  edgemap_set(edgemap, make_edge(11, 94), &value);

  // added edge to shadow cutset: l0 = 30 (VALID), l1 = 93 (SHADOW), t = 0
  value = 0;
  edgemap_set(edgemap, make_edge(30, 93), &value);

  // added edge to shadow cutset: l0 = 5 (VALID), l1 = 93 (SHADOW), t = 0
  value = 0;
  edgemap_set(edgemap, make_edge(5, 93), &value);

  // added edge to shadow cutset: l0 = 30 (VALID), l1 = 60 (SHADOW), t = 0
  value = 0;
  edgemap_set(edgemap, make_edge(30, 60), &value);

  // added edge to shadow cutset: l0 = 5 (VALID), l1 = 60 (SHADOW), t = 0
  value = 0;
  edgemap_set(edgemap, make_edge(5, 60), &value);

  // added edge to shadow cutset: l0 = 29 (VALID), l1 = 60 (SHADOW), t = 0.118576
  value = 0.118576;
  edgemap_set(edgemap, make_edge(29, 60), &value);

  // added edge to shadow cutset: l0 = 5 (VALID), l1 = 45 (SHADOW), t = 0
  value = 0;
  edgemap_set(edgemap, make_edge(5, 45), &value);

  // added edge to shadow cutset: l0 = 30 (VALID), l1 = 33 (SHADOW), t = 0
  value = 0;
  edgemap_set(edgemap, make_edge(30, 33), &value);

  // added edge to shadow cutset: l0 = 11 (VALID), l1 = 33 (SHADOW), t = 0
  value = 0;
  edgemap_set(edgemap, make_edge(11, 33), &value);

  // added edge to shadow cutset: l0 = 33 (SHADOW), l1 = 77 (VALID), t = 0.218488
  value = 0.218488;
  edgemap_set(edgemap, make_edge(33, 77), &value);

  assert_that(edgemap_size(edgemap), is_equal_to(11));

  edgemap_iter_s *iter;
  edgemap_iter_alloc(&iter);
  edgemap_iter_init(iter, edgemap);

  edge_s edge;

  // added edge to shadow cutset: l0 = 30 (VALID), l1 = 94 (SHADOW), t = 0
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(30));
  assert_that(edge.l[1], is_equal_to(94));
  assert_that(value, is_equal_to(0));

  // added edge to shadow cutset: l0 = 11 (VALID), l1 = 94 (SHADOW), t = 0
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(11));
  assert_that(edge.l[1], is_equal_to(94));
  assert_that(value, is_equal_to(0));

  // added edge to shadow cutset: l0 = 30 (VALID), l1 = 93 (SHADOW), t = 0
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(30));
  assert_that(edge.l[1], is_equal_to(93));
  assert_that(value, is_equal_to(0));

  // added edge to shadow cutset: l0 = 5 (VALID), l1 = 93 (SHADOW), t = 0
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(5));
  assert_that(edge.l[1], is_equal_to(93));
  assert_that(value, is_equal_to(0));

  // added edge to shadow cutset: l0 = 30 (VALID), l1 = 60 (SHADOW), t = 0
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(30));
  assert_that(edge.l[1], is_equal_to(60));
  assert_that(value, is_equal_to(0));

  // added edge to shadow cutset: l0 = 5 (VALID), l1 = 60 (SHADOW), t = 0
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(5));
  assert_that(edge.l[1], is_equal_to(60));
  assert_that(value, is_equal_to(0));

  // added edge to shadow cutset: l0 = 29 (VALID), l1 = 60 (SHADOW), t = 0.118576
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(29));
  assert_that(edge.l[1], is_equal_to(60));
  assert_that(value, is_equal_to(0.118576));

  // added edge to shadow cutset: l0 = 5 (VALID), l1 = 45 (SHADOW), t = 0
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(5));
  assert_that(edge.l[1], is_equal_to(45));
  assert_that(value, is_equal_to(0));

  // added edge to shadow cutset: l0 = 30 (VALID), l1 = 33 (SHADOW), t = 0
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(30));
  assert_that(edge.l[1], is_equal_to(33));
  assert_that(value, is_equal_to(0));

  // added edge to shadow cutset: l0 = 11 (VALID), l1 = 33 (SHADOW), t = 0
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(11));
  assert_that(edge.l[1], is_equal_to(33));
  assert_that(value, is_equal_to(0));

  // added edge to shadow cutset: l0 = 33 (SHADOW), l1 = 77 (VALID), t = 0.218488
  assert_that(edgemap_iter_next(iter, &edge, &value));
  assert_that(edge.l[0], is_equal_to(33));
  assert_that(edge.l[1], is_equal_to(77));
  assert_that(value, is_equal_to(0.218488));

  assert_that(edgemap_iter_next(iter, &edge, &value), is_false);

  edgemap_deinit(edgemap);
  edgemap_dealloc(&edgemap);
}

bool edge_sum_is_even(edge_s edge, char const *c, void const *aux) {
  (void)c;
  (void)aux;
  return (edge.l[0] + edge.l[1]) % 2 == 0;
}

bool char_matches(edge_s edge, char const *c, void const *aux) {
  (void)edge;
  (void)aux;
  return *c == 'a' || *c == 'd' || *c == 'g' || *c == 'i';
}

bool id_filter_and_count(edge_s edge, char const *c, void const *aux) {
  (void)edge;
  (void)c;
  ++*(int *)aux;
  return true;
}

Ensure (edgemap, filter_works) {
  edgemap_s *edgemap;
  edgemap_alloc(&edgemap);
  edgemap_init(edgemap, sizeof(char));

  char value = 'a';
  size_t l[2];
  for (l[0] = 0; l[0] < 3; ++l[0]) {
    for (l[1] = 0; l[1] < 3; ++l[1]) {
      edgemap_set(edgemap, make_edge(l[0], l[1]), &value);
      ++value;
    }
  }

  assert_that(edgemap_size(edgemap), is_equal_to(6));

  edgemap_s *filtered;
  edgemap_alloc(&filtered);
  edgemap_init(filtered, sizeof(char));

  edgemap_filter(edgemap, filtered, (edgemap_prop_t)edge_sum_is_even, NULL);

  assert_that(edgemap_contains(edgemap, make_edge(0, 0)));
  assert_that(edgemap_contains(edgemap, make_edge(0, 2)));
  assert_that(edgemap_contains(edgemap, make_edge(1, 1)));
  assert_that(edgemap_contains(edgemap, make_edge(2, 2)));

  edgemap_clear(filtered);
  assert_that(edgemap_is_empty(filtered));

  edgemap_filter(edgemap, filtered, (edgemap_prop_t)char_matches, NULL);
  assert_that(edgemap_size(filtered), is_equal_to(4));

  value = 'a';
  assert_that(edgemap_contains_value(filtered, &value));

  value = 'd';
  assert_that(edgemap_contains_value(filtered, &value));

  value = 'g';
  assert_that(edgemap_contains_value(filtered, &value));

  value = 'i';
  assert_that(edgemap_contains_value(filtered, &value));

  edgemap_clear(filtered);
  assert_that(edgemap_is_empty(filtered));

  int count = 0;
  edgemap_filter(edgemap, filtered, (edgemap_prop_t)id_filter_and_count, &count);
  assert_that(count, is_equal_to(edgemap_size(edgemap)));
  assert_that(edgemap_size(filtered), is_equal_to(edgemap_size(edgemap)));

  edgemap_deinit(filtered);
  edgemap_dealloc(&filtered);

  edgemap_deinit(edgemap);
  edgemap_dealloc(&edgemap);
}