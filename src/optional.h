#include <stdint.h>
#include <stdio.h>

#define ASSIGN_OPTIONAL(opt, val) \
	opt.hasValue = true;          \
	opt.value = val;

#define CREATE_OPTIONAL_TYPE(type)   \
	typedef struct type##_optional { \
		type value;                  \
		bool hasValue;               \
	} type##_optional;

CREATE_OPTIONAL_TYPE(uint32_t)
