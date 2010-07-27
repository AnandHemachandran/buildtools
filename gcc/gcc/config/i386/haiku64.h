/* Definitions for AMD x86_64 running Haiku with ELF format.
   Copyright (C) 1998, 1999, 2000, 2001, 2002, 2004
   Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


#if TARGET_64BIT
#define TARGET_VERSION fprintf (stderr, " (x86-64 Haiku/ELF)");
#else
#define TARGET_VERSION fprintf (stderr, " (i386 Haiku/ELF)");
#endif

/* The SVR4 ABI for the i386 says that records and unions are returned
 *   in memory.
 *
 *   TODO: Linux64 doesn't use pcc_struct_return scheme. Does haiku?
 *         If not this could be removed.
 */
#undef DEFAULT_PCC_STRUCT_RETURN
#define DEFAULT_PCC_STRUCT_RETURN 1

#undef DBX_REGISTER_NUMBER
#define DBX_REGISTER_NUMBER(n) \
  (TARGET_64BIT ? dbx64_register_map[n] : svr4_dbx_register_map[n])

#if TARGET_64BIT
#define TARGET_OS_CPP_BUILTINS()					\
  do									\
    {									\
	builtin_define ("__HAIKU__");					\
	builtin_define ("__x86_64__");					\
	builtin_define ("__stdcall=__attribute__((__stdcall__))");	\
	builtin_define ("__cdecl=__attribute__((__cdecl__))");		\
	builtin_assert ("system=haiku");				\
    /* Haiku apparently doesn't support merging of symbols across shared \
       object boundaries. Hence we need to explicitly specify that \
       type_infos are not merged, so that they get compared by name \
       instead of by pointer. */ \
    	builtin_define ("__GXX_MERGED_TYPEINFO_NAMES=0");		\
    	builtin_define ("__GXX_TYPEINFO_EQUALITY_INLINE=0");		\
	    builtin_define ("__STDC_ISO_10646__"); \
    }									\
  while (0)
#else
#define TARGET_OS_CPP_BUILTINS()					\
  do									\
    {									\
	builtin_define ("__HAIKU__");					\
	builtin_define ("__INTEL__");					\
	builtin_define ("__X86__");					\
	builtin_define ("__stdcall=__attribute__((__stdcall__))");	\
	builtin_define ("__cdecl=__attribute__((__cdecl__))");		\
	builtin_assert ("system=haiku");				\
    /* Haiku apparently doesn't support merging of symbols across shared \
       object boundaries. Hence we need to explicitly specify that \
       type_infos are not merged, so that they get compared by name \
       instead of by pointer. */ \
    	builtin_define ("__GXX_MERGED_TYPEINFO_NAMES=0");		\
    	builtin_define ("__GXX_TYPEINFO_EQUALITY_INLINE=0");		\
	    builtin_define ("__STDC_ISO_10646__"); \
    }									\
  while (0)
#endif

/* Provide a LINK_SPEC appropriate for Haiku.  Here we provide support
   for the special GCC options -static and -shared, which allow us to
   link things in one of these three modes by applying the appropriate
   combinations of options at link-time.  */

#undef	LINK_SPEC
#if TARGET_64BIT
#define LINK_SPEC "-m elf_x86_64 -z max-page-size=0x1000 -shared -Bsymbolic %{nostart:-e 0} %{shared:-e 0} %{!shared: %{!nostart: -no-undefined}}"
#else
#define LINK_SPEC "-m elf_i386_haiku -shared -Bsymbolic %{nostart:-e 0} %{shared:-e 0} %{!shared: %{!nostart: -no-undefined}}"
#endif


/* A C statement (sans semicolon) to output to the stdio stream
   FILE the assembler definition of uninitialized global DECL named
   NAME whose size is SIZE bytes and alignment is ALIGN bytes.
   Try to use asm_output_aligned_bss to implement this macro.  */

#define ASM_OUTPUT_ALIGNED_BSS(FILE, DECL, NAME, SIZE, ALIGN) \
  asm_output_aligned_bss (FILE, DECL, NAME, SIZE, ALIGN)

/* A C statement to output to the stdio stream FILE an assembler
   command to advance the location counter to a multiple of 1<<LOG
   bytes if it is within MAX_SKIP bytes.

   This is used to align code labels according to Intel recommendations.  */

#ifdef HAVE_GAS_MAX_SKIP_P2ALIGN
#define ASM_OUTPUT_MAX_SKIP_ALIGN(FILE,LOG,MAX_SKIP) \
  if ((LOG)!=0) \
    if ((MAX_SKIP)==0) fprintf ((FILE), "\t.p2align %d\n", (LOG)); \
    else fprintf ((FILE), "\t.p2align %d,,%d\n", (LOG), (MAX_SKIP))
#endif


/* Output assembler code to FILE to call the profiler.  */
#define NO_PROFILE_COUNTERS 1

#undef ASM_SPEC
#define ASM_SPEC "%{v:-V} %{Qy:} %{!Qn:-Qy} %{n} %{T} %{Ym,*} %{Yd,*} \
 %{Wa,*:%*} %{m32:--32} %{m64:--64}"

#undef  ASM_OUTPUT_ALIGNED_COMMON
#define ASM_OUTPUT_ALIGNED_COMMON(FILE, NAME, SIZE, ALIGN)		\
  x86_elf_aligned_common (FILE, NAME, SIZE, ALIGN);


/* i386 System V Release 4 uses DWARF debugging info.
   x86-64 ABI specifies DWARF2.  */
#if TARGET_64BIT
	#define DWARF2_DEBUGGING_INFO 1
	#define DWARF2_UNWIND_INFO 1

	#undef PREFERRED_DEBUGGING_TYPE
	#define PREFERRED_DEBUGGING_TYPE DWARF2_DEBUG

	#undef TARGET_ASM_SELECT_SECTION
	#define TARGET_ASM_SELECT_SECTION  x86_64_elf_select_section

	#undef TARGET_ASM_UNIQUE_SECTION
	#define TARGET_ASM_UNIQUE_SECTION  x86_64_elf_unique_section
#endif