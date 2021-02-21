#ifndef __CONFIG_H__
#define __CONFIG_H__

/* Use nested functions? */
#define NESTED_FUNCTIONS_ALLOWED

/* SDL? */
#define HAVE_SDL

/* SDL OpenGL support? */
#undef HAVE_SDLGL

/* GTK+? */
#undef HAVE_GTK

/* CLI? */
#define HAVE_CLI

/* Mac OS X Carbon? */
#undef HAVE_CARBON

/* OSS audio? */
#undef HAVE_OSS_AUDIO

/* Sun audio? */
#undef HAVE_SUN_AUDIO

/* Mac OS X Core Audio? */
#undef HAVE_MACOSX_AUDIO

/* Jack audio? */
#undef HAVE_JACK_AUDIO

/* libsndfile? */
#undef HAVE_SNDFILE

/* GP32? */
#undef HAVE_GP32

/* NDS? */
#undef HAVE_NDS

/* PSP? */
#define HAVE_PSP

/* MinGW (Windows32)? */
#undef WINDOWS32

/* Build trace mode? */
#undef TRACE

#endif  /* def __CONFIG_H__ */
