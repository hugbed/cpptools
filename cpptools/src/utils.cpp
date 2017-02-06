#include "utils.h"

namespace hb {

size_t probe_count::baseConstructionCount = 0;
size_t probe_count::copyConstructionCount = 0;
size_t probe_count::crefConstructionCount = 0;
size_t probe_count::moveConstructionCount = 0;

}