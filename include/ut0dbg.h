#ifndef ut0dbg_h
#define ut0dbg_h

#include <cstdio>
#include <functional>
#include <stdexcept>

enum CALLBACK_TYPE {
    _LEAF_SEGMENT,
    _SYS_TABLES
};

/** Set before leaf segment parsing stage starting. */
void ut_leaf_segment_callback_for_swat() throw(std::logic_error);
void ut_dump_sys_tables_callback_for_swat() throw(std::logic_error);

void ut_set_assert_callback(std::function<void()> callback);
/** Clear assert callback. */
void ut_reset_assert_callback() noexcept;

std::function<void()> ut_get_assert_callback(CALLBACK_TYPE type);

/** Report a failed assertion.
 * @param[in] expr The failed assertion
 * @param[in] file Source file containing the assertion
 * @param[in] line Line number of the assertion */
[[noreturn]] void ut_dbg_assertion_failed(const char *expr, const char *file,
                                          uint64_t line);

/** Abort execution if EXPR does not evaluate to nonzero.
 * @param EXPR assertion expression that should hold */
#define ut_a(EXPR)                                        \
    do {                                                    \
        if (false == (bool)(EXPR)) {                \
            ut_dbg_assertion_failed(#EXPR, __FILE__, __LINE__); \
        }                                                     \
    } while (0)

/** Abort execution. */
#define ut_error ut_dbg_assertion_failed(0, __FILE__, __LINE__)

#ifdef UNIV_DEBUG
/** Debug assertion. Does nothing unless UNIV_DEBUG is defined. */
#define ut_ad(EXPR) ut_a(EXPR)
/** Debug statement. Does nothing unless UNIV_DEBUG is defined. */
#define ut_d(EXPR) EXPR
/** Opposite of ut_d().  Does nothing if UNIV_DEBUG is defined. */
#define ut_o(EXPR)
#else
/** Debug assertion. Does nothing unless UNIV_DEBUG is defined. */
#define ut_ad(EXPR)
/** Debug statement. Does nothing unless UNIV_DEBUG is defined. */
#define ut_d(EXPR)
/** Opposite of ut_d().  Does nothing if UNIV_DEBUG is defined. */
#define ut_o(EXPR) EXPR
#endif

#endif
