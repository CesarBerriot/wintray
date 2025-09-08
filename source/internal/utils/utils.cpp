#include "utils.hpp"
#include <hard_assert.hpp>

namespace wintray::internal::utils
{	HICON load_icon(std::vector<std::byte> data)
	{	// http://msdn.microsoft.com/en-us/library/ms997538.aspx
		#pragma pack(push, 1)
			struct ICONDIRENTRY
			{	BYTE bWidth;
				BYTE bHeight;
				BYTE bColorCount;
				BYTE bReserved;
				WORD wPlanes;
				WORD wBitCount;
				DWORD dwBytesInRes;
				DWORD dwImageOffset;
			};

			struct ICONDIR
			{	WORD idReserved;
				WORD idType;
				WORD idCount;
				ICONDIRENTRY idEntries[1];
			};
		#pragma pack(pop)

		ICONDIR & directory = *(ICONDIR*)data.data();
		ICONDIRENTRY entry = directory.idEntries[0];

		HICON result =
			CreateIconFromResourceEx
			(	(BYTE*)data.data() + entry.dwImageOffset,
				entry.dwBytesInRes,
				true,
				0x00030000,
				0,
				0,
				0
			);

		ha_assert(result, MODULE_NAME, "Failed to load the provided icon.");

		return result;
	}
}
