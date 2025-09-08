#include <windows.h>
#include <vector>
#include <cstddef>

namespace wintray::internal::utils
{	HICON load_icon(std::vector<std::byte> data);
}
