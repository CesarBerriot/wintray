#include <thread>
#include <memory>
#include <wintray.hpp>
#include <cmrc/cmrc.hpp>
#include <make_string.hpp>

CMRC_DECLARE(resource_library);

int main()
{	cmrc::file icon_file = cmrc::resource_library::get_filesystem().open("resources/tray_icon.ico");

	std::vector<int> icons_awaiting_deletion;

	std::map<int, std::unique_ptr<wintray::icon>> icons;
	for(int i = 0; i < 5; ++i)
		icons[i] =
			std::make_unique<wintray::icon>
			(	wintray::icon_data
				{	.icon = { (std::byte*)icon_file.begin(), (std::byte*)icon_file.end() },
					.description = make_string("Icon %i", i + 1),
					.click_callback = [i] { printf("Clicked icon %i\n", i + 1); },
					.buttons =
						{	{	"Log",
								[i] { printf("Log from icon %i\n", i + 1); }
							},
							{	"Quit",
								[&icons_awaiting_deletion, i]
								{	icons_awaiting_deletion.push_back(i);
								}
							}
						}
				}
			);

	puts("Icons registered. Start interacting to test their features.");

	while(!icons.empty())
	{	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		for(int index : icons_awaiting_deletion)
		{	printf("Quitting icon %i\n", index + 1);
			icons.erase(icons.find(index));
		}
		icons_awaiting_deletion.clear();
	}

	return EXIT_SUCCESS;
}
