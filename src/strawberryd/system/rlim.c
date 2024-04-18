#include <sys/time.h>
#include <sys/resource.h>

#include <system/rlim.h>

void limit_memory_usage(int limit)
{
	struct rlimit rlimit;
	rlim_t new_limit = (rlim_t)limit;

	getrlimit(RLIMIT_AS, &rlimit);
	if (new_limit > rlimit.rlim_max) {
		// @todo: log this
		// specified soft limit is larger than the hard limit, so we lower
		// the soft limit to the maximum possible value
		new_limit = rlimit.rlim_max;
	}

	rlimit.rlim_cur = new_limit;
	setrlimit(RLIMIT_AS, &rlimit);
}
