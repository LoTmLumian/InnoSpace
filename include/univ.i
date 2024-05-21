/** The 'undefined' value for a  32-bit unsigned integer */
constexpr uint32_t UINT32_UNDEFINED = ~0U;

/** The following number as the length of a logical field means that the field
has the SQL NULL as its value. NOTE that because we assume that the length
of a field is a 32-bit integer when we store it, for example, to an undo log
on disk, we must have also this number fit in 32 bits, also in 64-bit
computers! */
constexpr uint32_t UNIV_SQL_NULL = UINT32_UNDEFINED;