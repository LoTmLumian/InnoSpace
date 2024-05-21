#include "ut0dbg.h"

static std::function<void()> assert_callback;

void ut_set_assert_callback(std::function<void()> &callback) {
    assert_callback = callback;
}

void ut_reset_assert_callback() noexcept {
    std::function<void()> empty_cb;
    assert_callback.swap(empty_cb);
}

void ut_leaf_segment_callback_for_swat() throw(std::logic_error) {
    throw std::logic_error("Failed to parse leaf segment");
}

void ut_set_leaf_segment_callback_for_swat() {
    std::function<void()> leaf_segment_cb = ut_leaf_segment_callback_for_swat;
    ut_set_assert_callback(leaf_segment_cb);
}

/** Report a failed assertion.
 * @param[in] expr The failed assertion
 * @param[in] file Source file containing the assertion
 * @param[in] line Line number of the assertion */
[[noreturn]] void ut_dbg_assertion_failed(const char *expr, const char *file,
                                          uint64_t line) {
    fflush(stderr);
    fflush(stdout);
    /* Call any registered callback function. */
    if (assert_callback) {
        assert_callback();
    }
    abort();
}

