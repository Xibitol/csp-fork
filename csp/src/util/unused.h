/**
 * @file unused.h
 *
 * @author Xibitol <xibitol@pimous.dev>
 * @date 2025
 */

#ifdef __GNUC__
	#define UNUSED_VAR(x) UNUSED_##x __attribute__((__unused__))
#else
	#define UNUSED_VAR(x) UNUSED_##x
#endif