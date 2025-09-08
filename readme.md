# WinTray - Windows Tray Notifications Made Simple
Make tray notifications using a single line of code.

### Usage
All you need to know is that the icon image data vector
assumes you'll provide it a full in-memory **ico** file
and icons run from their construction to their destruction,
the rest is entirely self-explanatory. 

#### See
* [**wintray.hpp**](include/wintray.hpp)
* [**An Example Program**](example/source/main.cpp)

#### Code Snippets
```cpp
// Basic Usage
wintray::icon my_icon({ .icon = my_icon_image });
```
```cpp
// Icon Descriptions
wintray::icon my_icon
(   {   .icon = my_icon_image,
        .description = "This will appear when hovering the icon."
    }
);
```

```cpp
// Icon Click Callbacks
wintray::icon my_icon
(   {   .icon = my_icon_image,
        .click_callback = [] { puts("Icon Clicked!"); }
    }
);
```

```cpp
// Icon Context Menus
wintray::icon my_icon
(   {   .icon = my_icon_image,
        .buttons =
        {   {   "About",
                []
                {   puts("This is example software.");
                }
            },
            {   "Quit",
                []
                {   exit(EXIT_SUCCESS);
                }
            }
        }
    }
);
```

### Credits
Example tray icon from HideMaru on [FlatIcon](https://www.flaticon.com/free-icon/terminal_10478244).