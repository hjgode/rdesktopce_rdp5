//keymap.h

#include <winuser.h>

#define KTAB_SIZE 0xFF

//some undefined VK values
#define VK_UNDEF 0x00
#define VK_NUL VK_UNDEF
#define VK_SOH VK_UNDEF
#define VK_STX VK_UNDEF
#define VK_ETX VK_UNDEF
#define VK_EOT VK_UNDEF
#define VK_ENQ VK_UNDEF
#define VK_ACK VK_UNDEF
#define VK_BEL VK_UNDEF
#define VK_VT VK_UNDEF
#define VK_FF VK_UNDEF 
#define VK_SO VK_UNDEF
#define VK_SI VK_UNDEF
#define VK_DLE VK_UNDEF
#define VK_DC1 VK_UNDEF
#define VK_DC2 VK_UNDEF
#define VK_DC3 VK_UNDEF
#define VK_DC4 VK_UNDEF
#define VK_NAK VK_UNDEF
#define VK_SYN VK_UNDEF
#define VK_ETB VK_UNDEF
#define VK_CAN VK_UNDEF
#define VK_EM VK_UNDEF
#define VK_SUB VK_UNDEF
#define VK_FS VK_UNDEF
#define VK_GS VK_UNDEF
#define VK_RS VK_UNDEF
#define VK_US VK_UNDEF

// some more undefined vk values
#define VK_0 	0x30
#define VK_1 	0x31
#define VK_2 	0x32
#define VK_3 	0x33
#define VK_4 	0x34
#define VK_5 	0x35
#define VK_6 	0x36
#define VK_7 	0x37
#define VK_8 	0x38
#define VK_9 	0x39
#define VK_A 	0x41
#define VK_B 	0x42
#define VK_C 	0x43
#define VK_D 	0x44
#define VK_E 	0x45
#define VK_F 	0x46
#define VK_G 	0x47
#define VK_H 	0x48
#define VK_I 	0x49
#define VK_J 	0x4A
#define VK_K 	0x4B
#define VK_L 	0x4C
#define VK_M 	0x4D
#define VK_N 	0x4E
#define VK_O 	0x4F
#define VK_P 	0x50
#define VK_Q 	0x51
#define VK_R 	0x52
#define VK_S 	0x53
#define VK_T 	0x54
#define VK_U 	0x55
#define VK_V 	0x56
#define VK_W 	0x57
#define VK_X 	0x58
#define VK_Y 	0x59
#define VK_Z 	0x5A

// the struct used to save a ASCII <-> VK_ table
typedef struct {
     byte kByte[1];
     char txt[15];
     byte kVKval; //the VK value to send for kChar/kByte
     BOOL kShift;
}KTABLE;

typedef KTABLE* pKTABLE;

// the translation tabel for ASCII to VK_ values
KTABLE vkTable[] = {
	{0x00, "NUL", VK_NUL, FALSE}, 
	{0x01, "SOH", VK_SOH, FALSE}, 
	{0x02, "STX", VK_STX, FALSE}, 
	{0x03, "ETX", VK_ETX, FALSE}, 
	{0x04, "EOT", VK_EOT, FALSE}, 
	{0x05, "ENQ", VK_ENQ, FALSE}, 
	{0x06, "ACK", VK_ACK, FALSE}, 
	{0x07, "BEL", VK_BEL, FALSE}, 
	{0x08, "BS", VK_BACK, FALSE}, 
	{0x09, "HT", VK_TAB, FALSE}, 
	{0x0A, "LF", VK_RETURN, FALSE}, 
	{0x0B, "VT", VK_VT, FALSE}, 
	{0x0C, "FF", VK_FF, FALSE}, 
	{0x0D, "CR", VK_RETURN, FALSE}, 
	{0x0E, "SO", VK_SO, FALSE}, 
	{0x0F, "SI", VK_SI, FALSE}, 
	{0x10, "DLE", VK_DLE, FALSE}, 
	{0x11, "DC1", VK_DC1, FALSE}, 
	{0x12, "DC2", VK_DC2, FALSE}, 
	{0x13, "DC3", VK_DC3, FALSE}, 
	{0x14, "DC4", VK_DC4, FALSE}, 
	{0x15, "NAK", VK_NAK, FALSE}, 
	{0x16, "SYN", VK_SYN, FALSE}, 
	{0x17, "ETB", VK_ETB, FALSE}, 
	{0x18, "CAN", VK_CAN, FALSE}, 
	{0x19, "EM", VK_EM, FALSE}, 
	{0x1A, "SUB", VK_SUB, FALSE}, 
	{0x1B, "ESC", VK_ESCAPE, FALSE}, 
	{0x1C, "FS", VK_FS, FALSE}, 
	{0x1D, "GS", VK_GS, FALSE}, 
	{0x1E, "RS", VK_RS, FALSE}, 
	{0x1F, "US", VK_US, FALSE}, 
	{0x20, " ", VK_SPACE, FALSE}, 
	{0x21, "!", VK_1, TRUE}, 
	{0x22, "\"", VK_APOSTROPHE, TRUE}, 
	{0x23, "#", VK_3, TRUE}, 
	{0x24, "$", VK_4, TRUE}, 
	{0x25, "%", VK_5, TRUE}, 
	{0x26, "&", VK_7, TRUE}, 
	{0x27, "'", VK_APOSTROPHE, FALSE}, 
	{0x28, "(", VK_9, TRUE}, 
	{0x29, ")", VK_0, TRUE}, 
	{0x2A, "*", VK_8, TRUE}, 
	{0x2B, "+", VK_EQUAL, TRUE}, 
	{0x2C, ",", VK_COMMA, FALSE}, 
	{0x2D, "-", VK_HYPHEN, FALSE}, 
	{0x2E, ".", VK_PERIOD, FALSE}, 
	{0x2F, "/", VK_SLASH, FALSE}, 
	{0x30, "0", VK_0, FALSE}, 
	{0x31, "1", VK_1, FALSE}, 
	{0x32, "2", VK_2, FALSE}, 
	{0x33, "3", VK_3, FALSE}, 
	{0x34, "4", VK_4, FALSE}, 
	{0x35, "5", VK_5, FALSE}, 
	{0x36, "6", VK_6, FALSE}, 
	{0x37, "7", VK_7, FALSE}, 
	{0x38, "8", VK_8, FALSE}, 
	{0x39, "9", VK_9, FALSE}, 
	{0x3A, ":", VK_SEMICOLON, TRUE}, 
	{0x3B, ";", VK_SEMICOLON, FALSE}, 
	{0x3C, "<", VK_COMMA, TRUE}, 
	{0x3D, "=", VK_EQUAL, FALSE}, 
	{0x3E, ">", VK_PERIOD, TRUE}, 
	{0x3F, "?", VK_SLASH, TRUE}, 
	{0x40, "@", VK_2, TRUE}, 
	{0x41, "A", VK_A, TRUE}, 
	{0x42, "B", VK_B, TRUE}, 
	{0x43, "C", VK_C, TRUE}, 
	{0x44, "D", VK_D, TRUE}, 
	{0x45, "E", VK_E, TRUE}, 
	{0x46, "F", VK_F, TRUE}, 
	{0x47, "G", VK_G, TRUE}, 
	{0x48, "H", VK_H, TRUE}, 
	{0x49, "I", VK_I, TRUE}, 
	{0x4A, "J", VK_J, TRUE}, 
	{0x4B, "K", VK_K, TRUE}, 
	{0x4C, "L", VK_L, TRUE}, 
	{0x4D, "M", VK_M, TRUE}, 
	{0x4E, "N", VK_N, TRUE}, 
	{0x4F, "O", VK_O, TRUE}, 
	{0x50, "P", VK_P, TRUE}, 
	{0x51, "Q", VK_Q, TRUE}, 
	{0x52, "R", VK_R, TRUE}, 
	{0x53, "S", VK_S, TRUE}, 
	{0x54, "T", VK_T, TRUE}, 
	{0x55, "U", VK_U, TRUE}, 
	{0x56, "V", VK_V, TRUE}, 
	{0x57, "W", VK_W, TRUE}, 
	{0x58, "X", VK_X, TRUE}, 
	{0x59, "Y", VK_Y, TRUE}, 
	{0x5A, "Z", VK_Z, TRUE}, 
	{0x5B, "[", VK_LBRACKET, FALSE}, 
	{0x5C, "\\", VK_BACKSLASH, FALSE}, 
	{0x5D, "]", VK_RBRACKET, FALSE}, 
	{0x5E, "^", VK_6, TRUE}, 
	{0x5F, "_", VK_HYPHEN, TRUE}, 
	{0x60, "`", VK_BACKQUOTE, FALSE}, 
	{0x61, "a", VK_A, FALSE}, 
	{0x62, "b", VK_B, FALSE}, 
	{0x63, "c", VK_C, FALSE}, 
	{0x64, "d", VK_D, FALSE}, 
	{0x65, "e", VK_E, FALSE}, 
	{0x66, "f", VK_F, FALSE}, 
	{0x67, "g", VK_G, FALSE}, 
	{0x68, "h", VK_H, FALSE}, 
	{0x69, "i", VK_I, FALSE}, 
	{0x6A, "j", VK_J, FALSE}, 
	{0x6B, "k", VK_K, FALSE}, 
	{0x6C, "l", VK_L, FALSE}, 
	{0x6D, "m", VK_M, FALSE}, 
	{0x6E, "n", VK_N, FALSE}, 
	{0x6F, "o", VK_O, FALSE}, 
	{0x70, "p", VK_P, FALSE}, 
	{0x71, "q", VK_Q, FALSE}, 
	{0x72, "r", VK_R, FALSE}, 
	{0x73, "s", VK_S, FALSE}, 
	{0x74, "t", VK_T, FALSE}, 
	{0x75, "u", VK_U, FALSE}, 
	{0x76, "v", VK_V, FALSE}, 
	{0x77, "w", VK_W, FALSE}, 
	{0x78, "x", VK_X, FALSE}, 
	{0x79, "y", VK_Y, FALSE}, 
	{0x7A, "z", VK_Z, FALSE}, 
	{0x7B, "{", VK_LBRACKET, TRUE}, 
	{0x7C, "|", VK_BACKSLASH, TRUE}, 
	{0x7D, "}", VK_RBRACKET, TRUE}, 
	{0x7E, "~", VK_BACKQUOTE, TRUE}, 
	{0x7F, "DEL", VK_DELETE, FALSE},
//this will not come from ASCII barcode readers
	{0x80, "\x80", 0x80, TRUE}, 
	{0x81, "\x81", 0x81, TRUE}, 
	{0x82, "\x82", 0x82, TRUE}, 
	{0x83, "\x83", 0x83, TRUE}, 
	{0x84, "\x84", 0x84, TRUE}, 
	{0x85, "\x85", 0x85, TRUE}, 
	{0x86, "\x86", 0x86, TRUE}, 
	{0x87, "\x87", 0x87, TRUE}, 
	{0x88, "\x88", 0x88, TRUE}, 
	{0x89, "\x89", 0x89, TRUE}, 
	{0x8A, "\x8A", 0x8A, TRUE}, 
	{0x8B, "\x8B", 0x8B, TRUE}, 
	{0x8C, "\x8C", 0x8C, TRUE}, 
	{0x8D, "\x8D", 0x8D, TRUE}, 
	{0x8E, "\x8E", 0x8E, TRUE}, 
	{0x8F, "\x8F", 0x8F, TRUE}, 
	{0x90, "\x90", 0x90, TRUE}, 
	{0x91, "\x91", 0x91, TRUE}, 
	{0x92, "\x92", 0x92, TRUE}, 
	{0x93, "\x93", 0x93, TRUE}, 
	{0x94, "\x94", 0x94, TRUE}, 
	{0x95, "\x95", 0x95, TRUE}, 
	{0x96, "\x96", 0x96, TRUE}, 
	{0x97, "\x97", 0x97, TRUE}, 
	{0x98, "\x98", 0x98, TRUE}, 
	{0x99, "\x99", 0x99, TRUE}, 
	{0x9A, "\x9A", 0x9A, TRUE}, 
	{0x9B, "\x9B", 0x9B, TRUE}, 
	{0x9C, "\x9C", 0x9C, TRUE}, 
	{0x9D, "\x9D", 0x9D, TRUE}, 
	{0x9E, "\x9E", 0x9E, TRUE}, 
	{0x9F, "\x9F", 0x9F, TRUE}, 
	{0xA0, "\xA0", 0xA0, TRUE}, 
	{0xA1, "¡", 0xA1, TRUE}, 
	{0xA2, "¢", 0xA2, TRUE}, 
	{0xA3, "£", 0xA3, TRUE}, 
	{0xA4, "¤", 0xA4, TRUE}, 
	{0xA5, "¥", 0xA5, TRUE}, 
	{0xA6, "¦", 0xA6, TRUE}, 
	{0xA7, "§", 0xA7, TRUE}, 
	{0xA8, "¨", 0xA8, TRUE}, 
	{0xA9, "©", 0xA9, TRUE}, 
	{0xAA, "ª", 0xAA, TRUE}, 
	{0xAB, "«", 0xAB, TRUE}, 
	{0xAC, "¬", 0xAC, TRUE}, 
	{0xAD, "­", 0xAD, TRUE}, 
	{0xAE, "®", 0xAE, TRUE}, 
	{0xAF, "¯", 0xAF, TRUE}, 
	{0xB0, "°", 0xB0, TRUE}, 
	{0xB1, "±", 0xB1, TRUE}, 
	{0xB2, "²", 0xB2, TRUE}, 
	{0xB3, "³", 0xB3, TRUE}, 
	{0xB4, "´", 0xB4, TRUE}, 
	{0xB5, "µ", 0xB5, TRUE}, 
	{0xB6, "¶", 0xB6, TRUE}, 
	{0xB7, "·", 0xB7, TRUE}, 
	{0xB8, "¸", 0xB8, TRUE}, 
	{0xB9, "¹", 0xB9, TRUE}, 
	{0xBA, "º", 0xBA, TRUE}, 
	{0xBB, "»", 0xBB, TRUE}, 
	{0xBC, "¼", 0xBC, TRUE}, 
	{0xBD, "½", 0xBD, TRUE}, 
	{0xBE, "¾", 0xBE, TRUE}, 
	{0xBF, "¿", 0xBF, TRUE}, 
	{0xC0, "À", 0xC0, TRUE}, 
	{0xC1, "Á", 0xC1, TRUE}, 
	{0xC2, "Â", 0xC2, TRUE}, 
	{0xC3, "Ã", 0xC3, TRUE}, 
	{0xC4, "Ä", 0xC4, TRUE}, 
	{0xC5, "Å", 0xC5, TRUE}, 
	{0xC6, "Æ", 0xC6, TRUE}, 
	{0xC7, "Ç", 0xC7, TRUE}, 
	{0xC8, "È", 0xC8, TRUE}, 
	{0xC9, "É", 0xC9, TRUE}, 
	{0xCA, "Ê", 0xCA, TRUE}, 
	{0xCB, "Ë", 0xCB, TRUE}, 
	{0xCC, "Ì", 0xCC, TRUE}, 
	{0xCD, "Í", 0xCD, TRUE}, 
	{0xCE, "Î", 0xCE, TRUE}, 
	{0xCF, "Ï", 0xCF, TRUE}, 
	{0xD0, "Ð", 0xD0, TRUE}, 
	{0xD1, "Ñ", 0xD1, TRUE}, 
	{0xD2, "Ò", 0xD2, TRUE}, 
	{0xD3, "Ó", 0xD3, TRUE}, 
	{0xD4, "Ô", 0xD4, TRUE}, 
	{0xD5, "Õ", 0xD5, TRUE}, 
	{0xD6, "Ö", 0xD6, TRUE}, 
	{0xD7, "×", 0xD7, TRUE}, 
	{0xD8, "Ø", 0xD8, TRUE}, 
	{0xD9, "Ù", 0xD9, TRUE}, 
	{0xDA, "Ú", 0xDA, TRUE}, 
	{0xDB, "Û", 0xDB, TRUE}, 
	{0xDC, "Ü", 0xDC, TRUE}, 
	{0xDD, "Ý", 0xDD, TRUE}, 
	{0xDE, "Þ", 0xDE, TRUE}, 
	{0xDF, "ß", 0xDF, TRUE}, 
	{0xE0, "à", 0xE0, TRUE}, 
	{0xE1, "á", 0xE1, TRUE}, 
	{0xE2, "â", 0xE2, TRUE}, 
	{0xE3, "ã", 0xE3, TRUE}, 
	{0xE4, "ä", 0xE4, TRUE}, 
	{0xE5, "å", 0xE5, TRUE}, 
	{0xE6, "æ", 0xE6, TRUE}, 
	{0xE7, "ç", 0xE7, TRUE}, 
	{0xE8, "è", 0xE8, TRUE}, 
	{0xE9, "é", 0xE9, TRUE}, 
	{0xEA, "ê", 0xEA, TRUE}, 
	{0xEB, "ë", 0xEB, TRUE}, 
	{0xEC, "ì", 0xEC, TRUE}, 
	{0xED, "í", 0xED, TRUE}, 
	{0xEE, "î", 0xEE, TRUE}, 
	{0xEF, "ï", 0xEF, TRUE}, 
	{0xF0, "ð", 0xF0, TRUE}, 
	{0xF1, "ñ", 0xF1, TRUE}, 
	{0xF2, "ò", 0xF2, TRUE}, 
	{0xF3, "ó", 0xF3, TRUE}, 
	{0xF4, "ô", 0xF4, TRUE}, 
	{0xF5, "õ", 0xF5, TRUE}, 
	{0xF6, "ö", 0xF6, TRUE}, 
	{0xF7, "÷", 0xF7, TRUE}, 
	{0xF8, "ø", 0xF8, TRUE}, 
	{0xF9, "ù", 0xF9, TRUE}, 
	{0xFA, "ú", 0xFA, TRUE}, 
	{0xFB, "û", 0xFB, TRUE}, 
	{0xFC, "ü", 0xFC, TRUE}, 
	{0xFD, "ý", 0xFD, TRUE}, 
	{0xFE, "þ", 0xFE, TRUE}, 
	{0xFF, "ÿ", 0xFF, TRUE}
};



