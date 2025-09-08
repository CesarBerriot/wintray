#include "wintray.hpp"
#include <windows.h>
#include <thread>

namespace wintray::internal
{	class icon
	{	private:
			std::thread thread;
			bool should_exit;
			HWND window;
			HICON icon_;
			NOTIFYICONDATAA shell_icon_data;
			callback_t click_callback;
			decltype(icon_data::buttons) buttons;
		public:
			explicit icon(icon_data);
			~icon();
		private:
			void construct(icon_data);
			void register_window_class();
			void create_window();
			void load_icon(decltype(icon_data::icon));
			void compute_shell_icon_data(decltype(icon_data::description));
			void create_shell_icon();
			void process_messages();
			LRESULT window_procedure(UINT message, WPARAM wparam, LPARAM lparam);
			void destruct();
			void destroy_shell_icon();
			void unload_icon();
			void destroy_window();
	};
}