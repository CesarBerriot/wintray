#pragma once

#include <vector>
#include <optional>
#include <string>
#include <functional>
#include <cstddef>

namespace wintray
{	typedef std::function<void()> callback_t;

	struct icon_data
	{	std::vector<std::byte> icon;
		std::optional<std::string> description;
		callback_t click_callback;
		std::vector<std::pair<std::string, callback_t>> buttons;
	};

	class icon
	{	public:
			explicit icon(icon_data);
			~icon();
	};
}