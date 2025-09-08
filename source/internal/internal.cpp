#include "internal.hpp"
#include <hard_assert.hpp>
#include <make_string.hpp>
#include "utils/utils.hpp"

#define WINDOW_CLASS_NAME "{A65FCD8B-E093-4488-B3BD-F1D7461BE42A}"

namespace wintray::internal
{	icon::icon(icon_data data)
	{	thread =
			std::thread
			(	[this, data]
				{	construct(data);
					process_messages();
					destruct();
				}
			);
	}

	icon::~icon()
	{	ha_assert
		(	std::this_thread::get_id() != thread.get_id(),
			MODULE_NAME,
			"Can't destruct an icon from one of its callbacks."
		);
		ha_assert
		(	PostMessageA(window, WM_CLOSE, 0, 0),
			MODULE_NAME,
			"PostMessageA failure."
		);
		thread.join();
	}

	void icon::construct(icon_data data)
	{	should_exit = false;
		register_window_class();
		create_window();
		load_icon(data.icon);
		compute_shell_icon_data(data.description);
		create_shell_icon();
		click_callback = data.click_callback;
		buttons = data.buttons;
	}

	void icon::register_window_class()
	{	WNDCLASSA window_class = {};
		window_class.lpszClassName = WINDOW_CLASS_NAME;
		window_class.lpfnWndProc =
			[](HWND window, UINT message, WPARAM wparam, LPARAM lparam) -> LRESULT
			{	icon * self = (icon*)GetWindowLongPtrA(window, GWLP_USERDATA);
				return
					self
						? self->window_procedure(message, wparam, lparam)
						: DefWindowProcA(window, message, wparam, lparam);
			};
		if(!RegisterClassA(&window_class))
			ha_assert
			(	GetLastError() == ERROR_CLASS_ALREADY_EXISTS,
				MODULE_NAME,
				"RegisterClassA failure."
			);
	}

	void icon::create_window()
	{	window = CreateWindowA(WINDOW_CLASS_NAME, "", 0, 0, 0, 0, 0, NULL, NULL, NULL, 0);
		ha_assert(window, MODULE_NAME, "CreateWindowA failure.");
		SetLastError(0);
		SetWindowLongPtrA(window, GWLP_USERDATA, (LONG_PTR)this);
		ha_assert(!GetLastError(), MODULE_NAME, "SetWindowLongPtrA failure.");
	}

	void icon::load_icon(decltype(icon_data::icon) icon)
	{	icon_ = utils::load_icon(icon);
	}

	void icon::compute_shell_icon_data(decltype(icon_data::description) description)
	{	shell_icon_data.cbSize = sizeof(NOTIFYICONDATAA);
		shell_icon_data.hWnd = window;
		shell_icon_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		shell_icon_data.hIcon = icon_;
		if(description)
		{	enum { max_characters = sizeof(NOTIFYICONDATAA::szTip) - 1 };
			ha_assert
			(	description->length() <= max_characters,
				MODULE_NAME,
				make_string("Description exceeds max length (%i).", max_characters)
			);
			strcpy(shell_icon_data.szTip, description->c_str());
		}
		else
			shell_icon_data.szTip[0] = '\0';
		shell_icon_data.uCallbackMessage = RegisterWindowMessageA("{12FB2A35-D66B-40B6-81AB-7436739B2E16}");
		ha_assert(shell_icon_data.uCallbackMessage, MODULE_NAME, "RegisterWindowMessageA failure.");
	}

	void icon::create_shell_icon()
	{	ha_assert
		(	Shell_NotifyIconA(NIM_ADD, &shell_icon_data),
			MODULE_NAME,
			"Shell_NotifyIconA failure."
		);
	}

	void icon::process_messages()
	{	MSG message;
		int result;
		while(!should_exit && (result = GetMessageA(&message, window, 0, 0)))
		{	ha_assert(result != -1, MODULE_NAME, "GetMessageA failure.");
			DispatchMessageA(&message);
		}
	}

	LRESULT icon::window_procedure(UINT message, WPARAM wparam, LPARAM lparam)
	{	if(message == shell_icon_data.uCallbackMessage)
		{	switch(lparam)
			{	case WM_LBUTTONUP:
					if(click_callback)
						click_callback();
					break;
				case WM_RBUTTONUP:
					HMENU menu = CreatePopupMenu();
					ha_assert(menu, MODULE_NAME, "CreatePopupMenu failure.");
					std::map<int, std::string> registry;
					int i = 0;
					for(std::pair button : buttons)
					{	++i;
						ha_assert
						(	AppendMenuA(menu, MF_STRING, i, button.first.c_str()),
							MODULE_NAME,
							"AppendMenuA failure."
						);
						registry[i] = button.first;
					}
					if(GetForegroundWindow() != window)
						ha_assert(SetForegroundWindow(window), MODULE_NAME, "SetForegroundWindow failure.");
					POINT cursor_position;
					ha_assert(GetCursorPos(&cursor_position), MODULE_NAME, "GetCursorPos failure.");
					int selection = TrackPopupMenu(menu, TPM_NONOTIFY | TPM_RETURNCMD, cursor_position.x, cursor_position.y, 0, window, NULL);
					if(selection)
						buttons[registry[selection]]();
					ha_assert(DestroyMenu(menu), MODULE_NAME, "DestroyMenu failure.");
			}
		}
		else if(message == WM_CLOSE)
		{	should_exit = true;
			return 0;
		}
		//{	ha_assert(DestroyWindow(window), MODULE_NAME, "DestroyWindow failure.");
		//	should_exit = true;
		//}
		return DefWindowProcA(window, message, wparam, lparam);
	}

	void icon::destruct()
	{	destroy_shell_icon();
		unload_icon();
		destroy_window();
	}

	void icon::destroy_shell_icon()
	{	ha_assert
		(	Shell_NotifyIconA(NIM_DELETE, &shell_icon_data),
			MODULE_NAME,
			"Shell_NotifyIconA failure."
		);
	}

	void icon::unload_icon()
	{	ha_assert(DestroyIcon(icon_), MODULE_NAME, "DestroyIcon failure.");
	}

	void icon::destroy_window()
	{	ha_assert(DestroyWindow(window), MODULE_NAME, "DestroyWindow failure.");
		window = NULL;
	}
}
