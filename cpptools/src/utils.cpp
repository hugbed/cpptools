#include "utils.h"

namespace hb {

size_t probe<probe_policy::count>::baseConstructionCount = 0;
size_t probe<probe_policy::count>::copyConstructionCount = 0;
size_t probe<probe_policy::count>::crefConstructionCount = 0;
size_t probe<probe_policy::count>::moveConstructionCount = 0;

}