#pragma once

#define ADJUST_CRECT_ARRAY_LEN	8
#define HORT_KEY_ID_CTRL_ALT_A	5402

#define RESIZE_LOCATION_NONE		0
#define RESIZE_LOCATION_NORTHWEST	1
#define RESIZE_LOCATION_SOUTHEAST	2
#define RESIZE_LOCATION_NORTHEAST	3
#define RESIZE_LOCATION_SOUTHWEST	4
#define RESIZE_LOCATION_NORTH		5
#define RESIZE_LOCATION_SOUTH		6
#define RESIZE_LOCATION_WEST		7
#define RESIZE_LOCATION_EAST		8
#define RESIZE_LOCATION_AREA		9

#define SCREEN_CAPTURE_STATE_NONE		0
#define SCREEN_CAPTURE_STATE_START		1
#define SCREEN_CAPTURE_STATE_RESIZE		2

#define SELECT_AREA_EDIT_NONE			0
#define SELECT_AREA_EDIT_DRAW_RECT		1
#define SELECT_AREA_EDIT_DRAW_CIRCLE	2
#define SELECT_AREA_EDIT_DRAW_ARROW		3
#define SELECT_AREA_EDIT_DRAW_TEXT		4

#define FONT_ADJUST						8
#define DEFAULT_FONT					_T("Î¢ÈíÑÅºÚ")

#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_INFOR 2
#define LOG_LEVEL_DEBUG 3

#define SCLog(fmt, TAG, LEVEL, ...) do {\
	TRACE("[%s]%s - %d "fmt, TAG, __FUNCTION__, __LINE__, __VA_ARGS__); \
} while (0);

#define SCDbg(fmt, ...) SCLog(fmt, "DBG", LOG_LEVEL_DEBUG, __VA_ARGS__)
#define SCInf(fmt, ...) SCLog(fmt, "INF", LOG_LEVEL_INFOR, __VA_ARGS__)
#define SCErr(fmt, ...) SCLog(fmt, "ERR", LOG_LEVEL_ERROR, __VA_ARGS__)

#ifndef CHECK_FREE
#define CHECK_FREE(ptr) \
if (ptr) {\
	free(ptr);\
	ptr = NULL; \
}
#endif

#ifndef CHECK_DELETE
#define CHECK_DELETE(ptr) \
if (ptr) {\
	delete ptr; \
	ptr = NULL; \
}
#endif