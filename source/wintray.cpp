#include "wintray.hpp"
#include "internal/internal.hpp"
#include <map>
#include <memory>

static std::map<wintray::icon*, std::unique_ptr<wintray::internal::icon>> icons;

namespace wintray
{	icon::icon(icon_data data)
	{	icons[this] = std::make_unique<internal::icon>(data);
	}

	icon::~icon()
	{	icons.erase(icons.find(this));
	}
}
