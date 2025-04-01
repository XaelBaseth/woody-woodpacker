#include "../../inc/woody_woodpacker.h"

/* *
 * Converts a 16-bit unsigned integer between big-endian and little-endian.
 *
 * @param byte uint16_t: The 16-bit value to convert.
 * @param endian t_endian: The endianness of the data.
 *
 * @return uint16_t: The value in the correct byte order.
 *
 * */
uint16_t	get_uint16(uint16_t byte, t_endian endian) {
	if (endian == LENDIAN)
		return (byte);
	return ((byte << 8) | (byte >> 8));
}

/* *
 * Converts a 32-bit unsigned integer between big-endian and little-endian.
 *
 * @param byte uint32_t: The 32-bit value to convert.
 * @param endian t_endian: The endianness of the data.
 *
 * @return uint32_t: The value in the correct byte order.
 *
 * */
uint32_t	get_uint32(uint32_t byte, t_endian endian) {
	if (endian == LENDIAN)
		return (byte);
	byte = ((byte << 8) & 0xFF00FF00) | ((byte >> 8) & 0xFF00FF);
	return (byte << 16) | (byte >> 16);
}

/* *
 * Converts a 32-bit signed integer between big-endian and little-endian.
 *
 * @param byte int32_t: The 32-bit signed value to convert.
 * @param endian t_endian: The endianness of the data.
 *
 * @return int32_t: The value in the correct byte order.
 *
 * */
int32_t	get_int32(int32_t byte, t_endian endian) {
	if (endian == LENDIAN)
		return (byte);
	byte = ((byte << 8) & 0xFF00FF00) | ((byte >> 8) & 0xFF00FF ); 
    return (byte << 16) | ((byte >> 16) & 0xFFFF);
}

/* *
 * Converts a 64-bit unsigned integer between big-endian and little-endian.
 *
 * @param byte uint64_t: The 64-bit value to convert.
 * @param endian t_endian: The endianness of the data.
 *
 * @return uint64_t: The value in the correct byte order.
 *
 * */
uint64_t	get_uint64(uint64_t byte, t_endian endian) {
	if (endian == LENDIAN)
		return (byte);
	byte = ((byte << 8) & 0xFF00FF00FF00FF00ULL)
		| ((byte >> 8) & 0x00FF00FF00FF00FFULL);
	byte = ((byte << 16) & 0xFFFF0000FFFF0000ULL)
		| ((byte >> 16) & 0x0000FFFF0000FFFFULL);
	return (byte << 32) | (byte >> 32);
}