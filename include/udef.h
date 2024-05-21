#ifndef inno_space_udef_h
#define inno_space_udef_h

#include <assert.h>
#include <stdint.h>

#define byte unsigned char
#define ulint uint32_t
// #define ulint unsigned long

#define ut_ad assert
#define ut_a assert

#define UT_BITS_IN_BYTES(b) (((b) + 7) / 8)

static inline ulint ut_align_offset(const void *ptr, ulint align_no) {
  ut_ad(align_no > 0);
  ut_ad(((align_no - 1) & align_no) == 0);
  ut_ad(ptr);

  static_assert(sizeof(void *) == sizeof(ulint));

  return (((ulint)ptr) & (align_no - 1));
}

#endif
