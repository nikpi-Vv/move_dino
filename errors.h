#ifndef ERRORS_H
#define ERRORS_H

// Сообщения об ошибках
#define ERROR_LEADING_SPACES "Invalid format - no leading spaces allowed"
#define ERROR_SIZE_FIRST "First command must be SIZE"
#define ERROR_SIZE_USED "SIZE command already used"
#define ERROR_START_BEFORE_SIZE "START before SIZE"
#define ERROR_START_USED "START command already used"
#define ERROR_START_OUTSIDE "Cannot create dinosaur outside field"
#define ERROR_NO_DINOSAUR "Cannot execute - dinosaur not placed (START missing)"
#define ERROR_UNKNOWN_CMD "Unknown command"

// Ошибки формата
#define ERROR_SIZE_FORMAT "Invalid SIZE format. Use: SIZE width height"
#define ERROR_DIMENSIONS "Invalid dimensions"
#define ERROR_START_FORMAT "Invalid START format. Use: START x y"
#define ERROR_MOVE_FORMAT "Invalid MOVE format. Use: MOVE direction"
#define ERROR_PAINT_FORMAT "Invalid PAINT format. Use: PAINT color"
#define ERROR_DIG_FORMAT "Invalid DIG format. Use: DIG direction"
#define ERROR_MOUND_FORMAT "Invalid MOUND format. Use: MOUND direction"
#define ERROR_JUMP_FORMAT "Invalid JUMP format. Use: JUMP direction distance"
#define ERROR_LINE_TOO_LONG "Line too long"

// Ошибки параметров
#define ERROR_COLOR "Invalid color. Use a-z"
#define ERROR_DIRECTION "Invalid direction. Use UP/DOWN/LEFT/RIGHT"  
#define ERROR_JUMP_DISTANCE "Jump distance must be positive"

#define ERROR_GROW_FORMAT "Invalid GROW format. Use: GROW direction"
#define ERROR_CHOP_FORMAT "Invalid CHOP format. Use: CHOP direction" 
#define ERROR_MAKE_FORMAT "Invalid MAKE format. Use: MAKE direction"
#define ERROR_PUSH_FORMAT "Invalid PUSH format. Use: PUSH direction"
// Простые макросы для ошибок
#define RETURN_ERROR(line, msg) { printf("ERROR line %d: %s\n", line, msg); return 0; }

// Макросы для ошибок без номера строки (ИСПРАВЛЕНО!)
#define PRINT_ERROR(msg) printf("ERROR: %s\n", msg)
#define PRINT_ERROR_DETAILS(msg, details) printf("ERROR: %s: %s\n", msg, details)

#endif