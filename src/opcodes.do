L     F�  &     .text           �  �  tx           0`.data                               @ 0�.bss                                � 0�.rdata           6  �  <y      �   @ `@/4              l  :  ��      "   @ B/16             m  x>              @ B/30             �   �?              @ B/41                 �@   �         @ B/56             �  �@  �         @ B/68             C5  iB              @ B/83                �w              @ B/94             �   �w  �         @ 0@U��S��$�U�E��E�D$�E�$�    �É؃�$[]�U��VS��0��   �5�   �  ��)Љ\$�D$�$�5  �����E�    ��   �U��������-  � ����   �    ����~j�U��������-  �H�U��������-  �P�]��������-  � ���L$�T$�D$�E�D$�D$�5  �$   �    �O�U��������-  �P�M��������-  � ���T$�D$�E�D$�D$�5  �$    �    �E���   9E�������0[^]�U����E�    �E�    ��U���������-  � ��t�E��E���   9E�|؋E���U����E�E��E������E�    �$�U���������-  � :E�u�E��E���E���   9E�|ҋE��Ð��   < <= == != > >= in not in is is not exception match BAD                     	                                       	   !   
   (      8   f   STOP_CODE   Indicates end-of-code to the compiler, not used by the interpreter. POP_TOP Removes the top-of-stack (TOS) item. ROT_TWO    Swaps the two top-most stack items. ROT_THREE   Lifts second and third stack item one position up, moves top down to position three. DUP_TOP    Duplicates the reference on top of the stack. DUP_TOP_TWO NOP   Do nothing code. Used as a placeholder by the bytecode optimizer. UNARY_POSITIVE Implements TOS = +TOS. UNARY_NEGATIVE Implements TOS = -TOS. UNARY_NOT Implements TOS = not TOS. UNARY_INVERT Implements TOS = ~TOS. BINARY_POWER Implements TOS = TOS1 ** TOS. BINARY_MULTIPLY Implements TOS = TOS1 * TOS. BINARY_DIVIDE Implements TOS = TOS1 / TOS when from __future__ import division is not in effect. BINARY_MODULO Implements TOS = TOS1 % TOS. BINARY_ADD Implements TOS = TOS1 + TOS. BINARY_SUBTRACT Implements TOS = TOS1 - TOS. BINARY_SUBSCR Implements TOS = TOS1[TOS]. BINARY_FLOOR_DIVIDE Implements TOS = TOS1 // TOS. BINARY_TRUE_DIVIDE   Implements TOS = TOS1 / TOS when from __future__ import division is in effect. INPLACE_FLOOR_DIVIDE Implements in-place TOS = TOS1 // TOS. INPLACE_TRUE_DIVIDE  Implements in-place TOS = TOS1 / TOS when from __future__ import division is in effect. INPLACE_ADD Implements in-place TOS = TOS1 + TOS. INPLACE_SUBTRACT  Implements in-place TOS = TOS1 - TOS. INPLACE_MULTIPLY  Implements in-place TOS = TOS1 * TOS. INPLACE_DIVIDE    Implements in-place TOS = TOS1 / TOS when from __future__ import division is not in effect. INPLACE_MODULO  Implements in-place TOS = TOS1 % TOS. STORE_SUBSCR Implements TOS1[TOS] = TOS2. DELETE_SUBSCR Implements del TOS1[TOS]. BINARY_AND Implements TOS = TOS1 & TOS. BINARY_XOR Implements TOS = TOS1 ^ TOS. BINARY_OR Implements TOS = TOS1 | TOS. INPLACE_POWER    Implements in-place TOS = TOS1 ** TOS. GET_ITER Implements TOS = iter(TOS). PRINT_EXPR  Implements the expression statement for the interactive mode.
TOS is removed from the stack and printed. In non-interactive mode, an expression statement is terminated with POP_STACK. INPLACE_AND Implements in-place TOS = TOS1 & TOS. INPLACE_XOR   Implements in-place TOS = TOS1 ^ TOS. INPLACE_OR    Implements in-place TOS = TOS1 | TOS. BREAK_LOOP    Terminates a loop due to a break statement. RETURN_VALUE    Returns with TOS to the caller of the function. POP_BLOCK   Removes 1 block from the block stack.Per frame, there is a stack of blocks, denoting nested loops,try blocks,and such. STORE_NAME   Implements name = TOS. /namei/ is the index of name in the attribute co_names of the code object.
The compiler tries to use STORE_LOCAL or STORE_GLOBAL if possible. DELETE_NAME    Implements del name, where /namei/ is the index into co_names attribute of the code object. UNPACK_SEQUENCE Unpacks TOS into /count/ individual values, which are put onto the stack right-to-left. FOR_ITER    TOS is an iterator. Call its next() method. If this yields a new value, push it on the stack
(leaving the iterator below it). If the iterator indicates it is exhausted TOS is popped,
and the byte code counter is incremented by /delta/. STORE_GLOBAL    Works as STORE_NAME(/namei/), but stores the name as a global. DELETE_GLOBAL    Works as DELETE_NAME(/namei/), but deletes a global name. LOAD_CONST    Pushes "co_consts[/consti/]" onto the stack. LOAD_NAME  Pushes the value associated with "co_names[/namei/]" onto the stack. BUILD_TUPLE    Creates a tuple consuming /count/ items from the stack, and pushes the resulting tuple onto the stack. BUILD_LIST   Works as BUILD_TUPLE(/count/), but creates a list. COMPARE_OP   Performs a Boolean operation. The operation name can be found in cmp_op[/opname/]. JUMP_FORWARD Increments byte code counter by /delta/. JUMP_IF_FALSE  If TOS is false, increment the byte code counter by /delta/. TOS is not changed. JUMP_IF_TRUE   If TOS is true, increment the byte code counter by /delta/. TOS is left on the stack. JUMP_ABSOLUTE Set byte code counter to /target/. POP_JUMP_IF_FALSE    If TOS is false, sets the bytecode counter to target. TOS is popped. POP_JUMP_IF_TRUE   If TOS is true, sets the bytecode counter to target. TOS is popped. LOAD_GLOBAL Loads the global named co_names[/namei/] onto the stack. SETUP_LOOP Pushes a block for a loop onto the block stack.
The block spans from the current instruction with a size of /delta/ bytes. LOAD_FAST    Pushes a reference to the local co_varnames[/var_num/] onto the stack. STORE_FAST   Stores TOS into the local co_varnames[/var_num/]. DELETE_FAST   Deletes local co_varnames[/var_num/]. INPLACE_LSHIFT    Implements in-place TOS = TOS1 << TOS. INPLACE_RSHIFT   Implements in-place TOS = TOS1 >> TOS. BINARY_LSHIFT Implements TOS = TOS1 << TOS. BINARY_RSHIFT Implements TOS = TOS1 >> TOS. EXTENDED_ARG Support for opargs more than 16 bits long. CALL_FUNCTION    Calls a function. The low byte of /argc/ indicates the number of positional parameters,
the high byte the number of keyword parameters. On the stack, the opcode finds the keyword parameters first.
For each keyword argument, the value is on top of the key. Below the keyword parameters, the positional parameters are on the stack,
with the right-most parameter on top. Below the parameters, the function object to call is on the stack. MAKE_FUNCTION    Pushes a new function object on the stack. TOS is the code associated with the function.
The function object is defined to have /argc/ default parameters, which are found below TOS. BUILD_SLICE   Pushes a slice object on the stack. /argc/ must be 2 or 3. If it is 2, slice(TOS1, TOS) is pushed;
if it is 3, slice(TOS2, TOS1, TOS) is pushed. See the slice() built-in function for more information. LOAD_DEREF Loads the cell contained in slot /i/ of the cell and free variable storage.
Pushes a reference to the object the cell contains on the stack. STORE_DEREF    Stores TOS into the cell contained in slot /i/ of the cell and free variable storage. DELETE_DEREF  Empties the cell contained in slot i of the cell and free variable storage. Used by the del statement. CALL_FUNCTION_VAR    Calls a function. /argc/ is interpreted as in CALL_FUNCTION.
The top element on the stack contains the variable argument list, followed by keyword and positional arguments. CALL_FUNCTION_KW   Calls a function. /argc/ is interpreted as in CALL_FUNCTION.
The top element on the stack contains the keyword arguments dictionary, followed by explicit keyword and positional arguments. CALL_FUNCTION_VAR_KW    Calls a function. /argc/ is interpreted as in CALL_FUNCTION.
The top element on the stack contains the keyword arguments dictionary, followed by the variable-arguments tuple,
followed by explicit keyword and positional arguments. END_FINALLY   Terminates a finally clause. The interpreter recalls whether the exception has to be re-raised,
or whether the function returns, and continues with the outer-next block. LOAD_BUILD_CLASS  Pushes builtins.__build_class__() onto the stack. It is later called by CALL_FUNCTION to construct a class. BUILD_CLASS Creates a new class object. TOS is the methods dictionary,
TOS1 the tuple of the names of the base classes, and TOS2 the class name. RAISE_VARARGS  Raises an exception. /argc/ indicates the number of parameters to the raise statement,
ranging from 0 to 3. The handler will find the traceback as TOS2, the parameter as TOS1, and the exception as TOS. IMPORT_FROM   Loads the attribute co_names[/namei/] from the module found in TOS.
The resulting object is pushed onto the stack, to be subsequently stored by a STORE_FAST instruction. IMPORT_STAR   Loads all symbols not starting with "_" directly from the module TOS to the local namespace.
The module is popped after loading all names. This opcode implements from module import *. YIELD_VALUE Pops TOS and yields it from a generator. STORE_LOCALS   Pops TOS from the stack and stores it as the current frame�s f_locals. This is used in class construction. STORE_MAP    Store a key and value pair in a dictionary. Pops the key and value while leaving the dictionary on the stack. STORE_ATTR    Implements TOS.name = TOS1, where /namei/ is the index of name in co_names. DELETE_ATTR Implements del TOS.name, using /namei/ as index into co_names. SETUP_EXCEPT Pushes a try block from a try-except clause onto the block stack. /delta/ points to the first except block. SETUP_FINALLY   Pushes a try block from a try-except clause onto the block stack. /delta/ points to the finally block. WITH_CLEANUP Cleans up the stack when a with statement block exits.
TOS is the context manager�s __exit__() bound method. Below TOS are 1�3 values
indicating how/why the finally clause was entered:
SECOND = None
(SECOND, THIRD) = (WHY_{RETURN,CONTINUE}), retval
SECOND = WHY_*; no retval below it
(SECOND, THIRD, FOURTH) = exc_info()
In the last case, TOS(SECOND, THIRD, FOURTH) is called, otherwise TOS(None, None, None). In addition, TOS is removed from the stack.
If the stack represents an exception, and the function call returns a �true� value, this information is �zapped�
and replaced with a single WHY_SILENCED to prevent END_FINALLY from re-raising the exception. (But non-local gotos will still be resumed.) BUILD_MAP Pushes a new empty dictionary object onto the stack.
The argument is ignored and set to /zero/ by the compiler. BUILD_SET   Pushes a new empty set object onto the stack.
The argument is ignored and set to /zero/ by the compiler. MAKE_CLOSURE   Creates a new function object, sets its func_closure slot, and pushes it on the stack.
TOS is the code associated with the function. If the code object has N free variables, the next N items on the stack are the cells
for these variables. The function also has /argc/ default parameters, where are found before the cells. LOAD_CLOSURE  Pushes a reference to the cell contained in slot /i/ of the cell and free variable storage.
The name of the variable is co_cellvars[i] if i is less than the length of co_cellvars. Otherwise it is co_freevars[i - len(co_cellvars)]. LIST_APPEND  Calls list.append(TOS[-i], TOS). Used to implement list comprehensions. SET_ADD Calls set.add(TOS1[-i], TOS). Used to implement set comprehensions. MAP_ADD Calls dict.setitem(TOS1[-i], TOS, TOS1). Used to implement dict comprehensions. IMPORT_NAME Imports the module co_names[namei]. TOS and TOS1 are popped
and provide the fromlist and level arguments of __import__(). The module object is pushed onto the stack.
The current namespace is not affected: for a proper import statement,
a subsequent STORE_FAST instruction modifies the namespace. UNPACK_EX   Implements assignment with a starred target: Unpacks an iterable in TOS into individual values,
where the total number of values can be smaller than the number of items in the iterable:
one the new values will be a list of all leftover items.The low byte of counts is the number of values
before the list value, the high byte of counts the number of values after it. The resulting values are put onto the stack right-to-left. LOAD_ATTR Replaces TOS with getattr(TOS, co_names[/namei/]). SETUP_WITH   This opcode performs several operations before a with block starts.
 First, it loads __exit__() from the context manager and pushes it onto the stack for later use by WITH_CLEANUP.
 Then, __enter__() is called, and a finally block pointing to delta is pushed. Finally, the result of calling the enter method is pushed onto the stack.
 The next opcode will either ignore it (POP_TOP), or store it in (a) variable(s) (STORE_FAST, STORE_NAME, or UNPACK_SEQUENCE).                                    �   �             �   �             !  ,            P  \            �  �            �  �         	   �  �         
   >  M            d  s            �  �            �  �            �  �            �              *  8            �  �            �  �            �  �                          5  I            g  |            �  �                       7   t  �         8   �  �         9   �  �         :     (         ;   �  �         <   �  �         =   �  �         @              A   4  ?         B   \  f         C   �  �         D   �  �         F   �  �         M   �  �         N   �  �         O   
	  	         P   >	  L	         S   x	  �	         W   �	  �	         Z   ;
  H
        [   �
  �
        \   X  h        ]   �  �        a   �  �        b             d   R  `        e   �  �        f   �  �        g   S  `        k   �  �        n   �           o   )  8        p   �  �        q   �  �        r     4        s   y  �        t   �  �        x              |   �  �        }   �  �        ~   .  <        K   b  t         L   �  �         >   �  �         ?   �           �   +  8        �   c  t        �   '  8        �   �  �         �   �  �        �   ]  l        �   �  �        �   7  L        �   �          �   �  �        X   �  �         G   ~  �         Y   �           �   �  �         m   f  t        T     ,         V   �  �         E     (         6   �  �         _               `   h   t         y   �   �          z   ,!  <!         Q   �!  �!         i   r$  |$        h   �$  �$        �   a%  p%        �   �&  �&        �   �'  �'        �   �'  (        �   H(  P(        l   �(  �(        ^   �)  �)         j   �+  �+        �   �+  �+        unsupported opcodes(%d of %d):
 [%3d,%xh] opcode: [ %s ]
%s

 [%3d,%xh] opcode: [ %s ]
                 h       GNU C 4.6.1 opcodes.c c:\Users\pez2001\Documents\Projects\mypy\src     �          unsigned int short unsigned int __builtin_va_list �   char _iobuf  �=  _ptr �=  # _cnt �C  #_base �=  #_flag �C  #_file �C  #_charbuf �C  #_bufsiz �C  #_tmpfname �=  # �   int FILE ��   long long int long int ��  cm_op ��  # name �=  # unsigned char cmp_op �s  �  	   ��  # name �=  #description �=  #argcount �g  #supported ��  # 
   ��  printf �C      +       �  __format ��  � __retval �C  S__local_argv ��   �l �  �   DumpUnsupportedOpCodes �+   c  8   �  ^   \  i �g  �l  GetSupportedOpcodesNum �g  c  �  p   &  r �`   �tp  �  i �g  �p  GetOpcodeIndex �g  �  �  �        ��  �dr �C  �t�  �  i �g  �p  J  �   _iob �     EC      -�  g    �  `   e opcodes 2�  �  _iob �     EC  cmp_opscount �      �  #  `    cmp_ops 9  @         -�  �   opcodes 2f  �-  �   %C  $ >   I  :;   :;I8
   I   :;I  :;  	 :;I8
  
 :;I  .:;'I@   :;I
     4 :;I
  & I  .?:;'@    .?:;'I@   :;I
  I  !   4 :;I?<  4 :;I?<  ! I/  4 :;I?
  4 :;I?
           t       t   *    u*   +    t        +   ,    t,   .    t.   b   ub  c   t        c  d   td  f   tf  �   u�  �   t        �  �   t�  �   t�  �   u�  �   t                         �          �   $  �      c:/mingw/bin/../lib/gcc/mingw32/4.6.1/../../../../include c:/mingw/bin/../lib/gcc/mingw32/4.6.1/include  stdio.h   opcodes.c    opcodes.h    debug.h    features.h    types.h    _mingw.h   stddef.h   stdarg.h   assert.h   strops.h    memory.h    stdlib.h   string.h         �v�>/Df��W����jzO <
�wgv�u: <�=1�w��g) <�=  __STDC__ 1  __STDC_VERSION__ 199901L  __STDC_HOSTED__ 1  __GNUC__ 4  __GNUC_MINOR__ 6  __GNUC_PATCHLEVEL__ 1  __VERSION__ "4.6.1"  __FINITE_MATH_ONLY__ 0  __SIZEOF_INT__ 4  __SIZEOF_LONG__ 4  __SIZEOF_LONG_LONG__ 8  __SIZEOF_SHORT__ 2  __SIZEOF_FLOAT__ 4  __SIZEOF_DOUBLE__ 8  __SIZEOF_LONG_DOUBLE__ 12  __SIZEOF_SIZE_T__ 4  __CHAR_BIT__ 8  __BIGGEST_ALIGNMENT__ 16  __ORDER_LITTLE_ENDIAN__ 1234  __ORDER_BIG_ENDIAN__ 4321  __ORDER_PDP_ENDIAN__ 3412  __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__  __FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__  __SIZEOF_POINTER__ 4  __SIZE_TYPE__ unsigned int  __PTRDIFF_TYPE__ int  __WCHAR_TYPE__ short unsigned int  __WINT_TYPE__ short unsigned int  __INTMAX_TYPE__ long long int  __UINTMAX_TYPE__ long long unsigned int  __CHAR16_TYPE__ short unsigned int  __CHAR32_TYPE__ unsigned int  __SIG_ATOMIC_TYPE__ int  __INT8_TYPE__ signed char  __INT16_TYPE__ short int  __INT32_TYPE__ int  __INT64_TYPE__ long long int  __UINT8_TYPE__ unsigned char  __UINT16_TYPE__ short unsigned int  __UINT32_TYPE__ unsigned int  __UINT64_TYPE__ long long unsigned int  __INT_LEAST8_TYPE__ signed char  __INT_LEAST16_TYPE__ short int  __INT_LEAST32_TYPE__ int  __INT_LEAST64_TYPE__ long long int  __UINT_LEAST8_TYPE__ unsigned char  __UINT_LEAST16_TYPE__ short unsigned int  __UINT_LEAST32_TYPE__ unsigned int  __UINT_LEAST64_TYPE__ long long unsigned int  __INT_FAST8_TYPE__ signed char  __INT_FAST16_TYPE__ short int  __INT_FAST32_TYPE__ int  __INT_FAST64_TYPE__ long long int  __UINT_FAST8_TYPE__ unsigned char  __UINT_FAST16_TYPE__ short unsigned int  __UINT_FAST32_TYPE__ unsigned int  __UINT_FAST64_TYPE__ long long unsigned int  __INTPTR_TYPE__ int  __UINTPTR_TYPE__ unsigned int  __GXX_ABI_VERSION 1002  __SCHAR_MAX__ 127  __SHRT_MAX__ 32767  __INT_MAX__ 2147483647  __LONG_MAX__ 2147483647L  __LONG_LONG_MAX__ 9223372036854775807LL  __WCHAR_MAX__ 65535  __WCHAR_MIN__ 0  __WINT_MAX__ 65535  __WINT_MIN__ 0  __PTRDIFF_MAX__ 2147483647  __SIZE_MAX__ 4294967295U  __INTMAX_MAX__ 9223372036854775807LL  __INTMAX_C(c) c ## LL  __UINTMAX_MAX__ 18446744073709551615ULL  __UINTMAX_C(c) c ## ULL  __SIG_ATOMIC_MAX__ 2147483647  __SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)  __INT8_MAX__ 127  __INT16_MAX__ 32767  __INT32_MAX__ 2147483647  __INT64_MAX__ 9223372036854775807LL  __UINT8_MAX__ 255  __UINT16_MAX__ 65535  __UINT32_MAX__ 4294967295U  __UINT64_MAX__ 18446744073709551615ULL  __INT_LEAST8_MAX__ 127  __INT8_C(c) c  __INT_LEAST16_MAX__ 32767  __INT16_C(c) c  __INT_LEAST32_MAX__ 2147483647  __INT32_C(c) c  __INT_LEAST64_MAX__ 9223372036854775807LL  __INT64_C(c) c ## LL  __UINT_LEAST8_MAX__ 255  __UINT8_C(c) c  __UINT_LEAST16_MAX__ 65535  __UINT16_C(c) c  __UINT_LEAST32_MAX__ 4294967295U  __UINT32_C(c) c ## U  __UINT_LEAST64_MAX__ 18446744073709551615ULL  __UINT64_C(c) c ## ULL  __INT_FAST8_MAX__ 127  __INT_FAST16_MAX__ 32767  __INT_FAST32_MAX__ 2147483647  __INT_FAST64_MAX__ 9223372036854775807LL  __UINT_FAST8_MAX__ 255  __UINT_FAST16_MAX__ 65535  __UINT_FAST32_MAX__ 4294967295U  __UINT_FAST64_MAX__ 18446744073709551615ULL  __INTPTR_MAX__ 2147483647  __UINTPTR_MAX__ 4294967295U  __FLT_EVAL_METHOD__ 2  __DEC_EVAL_METHOD__ 2  __FLT_RADIX__ 2  __FLT_MANT_DIG__ 24  __FLT_DIG__ 6  __FLT_MIN_EXP__ (-125)  __FLT_MIN_10_EXP__ (-37)  __FLT_MAX_EXP__ 128  __FLT_MAX_10_EXP__ 38  __FLT_DECIMAL_DIG__ 9  __FLT_MAX__ 3.40282346638528859812e+38F  __FLT_MIN__ 1.17549435082228750797e-38F  __FLT_EPSILON__ 1.19209289550781250000e-7F  __FLT_DENORM_MIN__ 1.40129846432481707092e-45F  __FLT_HAS_DENORM__ 1  __FLT_HAS_INFINITY__ 1  __FLT_HAS_QUIET_NAN__ 1  __DBL_MANT_DIG__ 53  __DBL_DIG__ 15  __DBL_MIN_EXP__ (-1021)  __DBL_MIN_10_EXP__ (-307)  __DBL_MAX_EXP__ 1024  __DBL_MAX_10_EXP__ 308  __DBL_DECIMAL_DIG__ 17  __DBL_MAX__ ((double)1.79769313486231570815e+308L)  __DBL_MIN__ ((double)2.22507385850720138309e-308L)  __DBL_EPSILON__ ((double)2.22044604925031308085e-16L)  __DBL_DENORM_MIN__ ((double)4.94065645841246544177e-324L)  __DBL_HAS_DENORM__ 1  __DBL_HAS_INFINITY__ 1  __DBL_HAS_QUIET_NAN__ 1  __LDBL_MANT_DIG__ 64  __LDBL_DIG__ 18  __LDBL_MIN_EXP__ (-16381)  __LDBL_MIN_10_EXP__ (-4931)  __LDBL_MAX_EXP__ 16384  __LDBL_MAX_10_EXP__ 4932  __DECIMAL_DIG__ 21  __LDBL_MAX__ 1.18973149535723176502e+4932L  __LDBL_MIN__ 3.36210314311209350626e-4932L  __LDBL_EPSILON__ 1.08420217248550443401e-19L  __LDBL_DENORM_MIN__ 3.64519953188247460253e-4951L  __LDBL_HAS_DENORM__ 1  __LDBL_HAS_INFINITY__ 1  __LDBL_HAS_QUIET_NAN__ 1  __DEC32_MANT_DIG__ 7  __DEC32_MIN_EXP__ (-94)  __DEC32_MAX_EXP__ 97  __DEC32_MIN__ 1E-95DF  __DEC32_MAX__ 9.999999E96DF  __DEC32_EPSILON__ 1E-6DF  __DEC32_SUBNORMAL_MIN__ 0.000001E-95DF  __DEC64_MANT_DIG__ 16  __DEC64_MIN_EXP__ (-382)  __DEC64_MAX_EXP__ 385  __DEC64_MIN__ 1E-383DD  __DEC64_MAX__ 9.999999999999999E384DD  __DEC64_EPSILON__ 1E-15DD  __DEC64_SUBNORMAL_MIN__ 0.000000000000001E-383DD  __DEC128_MANT_DIG__ 34  __DEC128_MIN_EXP__ (-6142)  __DEC128_MAX_EXP__ 6145  __DEC128_MIN__ 1E-6143DL  __DEC128_MAX__ 9.999999999999999999999999999999999E6144DL  __DEC128_EPSILON__ 1E-33DL  __DEC128_SUBNORMAL_MIN__ 0.000000000000000000000000000000001E-6143DL  __REGISTER_PREFIX__   __USER_LABEL_PREFIX__ _  __GNUC_STDC_INLINE__ 1  __NO_INLINE__ 1  __STRICT_ANSI__ 1  __GCC_HAVE_DWARF2_CFI_ASM 1  __PRAGMA_REDEFINE_EXTNAME 1  __SIZEOF_WCHAR_T__ 2  __SIZEOF_WINT_T__ 2  __SIZEOF_PTRDIFF_T__ 4  __i386 1  __i386__ 1  __tune_i386__ 1  _X86_ 1  __stdcall __attribute__((__stdcall__))  __fastcall __attribute__((__fastcall__))  __thiscall __attribute__((__thiscall__))  __cdecl __attribute__((__cdecl__))  __GXX_MERGED_TYPEINFO_NAMES 0  __GXX_TYPEINFO_EQUALITY_INLINE 0  __MSVCRT__ 1  __MINGW32__ 1  _WIN32 1  __WIN32 1  __WIN32__ 1  __WINNT 1  __WINNT__ 1  _INTEGRAL_MAX_BITS 64  __declspec(x) __attribute__((x))  __DECIMAL_BID_FORMAT__ 1  USE_DEBUGGING 1  BUILD 234  MAJOR_VERSION 0  MINOR_VERSION 74  DEBUG_BUILD 440  OPCODES_H  FEATURES_H  -USE_FILE_STREAM  /USE_INTERNAL_FUNCTIONS  3USE_FMOD_FUNCTIONS  TYPES_H  _STDIO_H_  __MINGW_H  __MINGW32_VERSION 3.20 __MINGW32_MAJOR_VERSION 3 __MINGW32_MINOR_VERSION 20 __MINGW32_PATCHLEVEL 0 E__MINGW_ANSI_STDIO__ 0x0000000000000001ULL J__MINGW_LC_EXTENSIONS__ 0x0000000000000050ULL K__MINGW_LC_MESSAGES__ 0x0000000000000010ULL L__MINGW_LC_ENVVARS__ 0x0000000000000040ULL O__attribute__ }__MINGW_IMPORT extern __attribute__ ((__dllimport__)) �_CRTIMP  �__DECLSPEC_SUPPORTED  �__int64 long long �__int32 long �__int16 short �__int8 char �__small char �__hyper long long �__MINGW_GNUC_PREREQ(major,minor) (__GNUC__ > (major) || (__GNUC__ == (major) && __GNUC_MINOR__ >= (minor))) �__CRT_INLINE extern inline __attribute__((__gnu_inline__)) �_CRTALIAS __CRT_INLINE __attribute__ ((__always_inline__)) �__UNUSED_PARAM(x) x __attribute__ ((__unused__)) �__MINGW_ATTRIB_NORETURN __attribute__ ((__noreturn__)) �__MINGW_ATTRIB_CONST __attribute__ ((__const__)) �__MINGW_ATTRIB_MALLOC __attribute__ ((__malloc__)) �__MINGW_ATTRIB_PURE __attribute__ ((__pure__)) �__MINGW_ATTRIB_NONNULL(arg) __attribute__ ((__nonnull__ (arg))) �__MINGW_ATTRIB_DEPRECATED __attribute__ ((__deprecated__)) �__MINGW_NOTHROW __attribute__ ((__nothrow__)) �__MSVCRT_VERSION__ 0x0600 �__USE_MINGW_ANSI_STDIO 1 __need_size_t  __need_NULL  __need_wchar_t  __need_wint_t  �__size_t__  �__SIZE_T__  �_SIZE_T  �_SYS_SIZE_T_H  �_T_SIZE_  �_T_SIZE  �__SIZE_T  �_SIZE_T_  �_BSD_SIZE_T_  �_SIZE_T_DEFINED_  �_SIZE_T_DEFINED  �_BSD_SIZE_T_DEFINED_  �_SIZE_T_DECLARED  �___int_size_t_h  �_GCC_SIZE_T  �_SIZET_  �__size_t  �__need_size_t �__wchar_t__  �__WCHAR_T__  �_WCHAR_T  �_T_WCHAR_  �_T_WCHAR  �__WCHAR_T  �_WCHAR_T_  �_BSD_WCHAR_T_  �_WCHAR_T_DEFINED_  �_WCHAR_T_DEFINED  �_WCHAR_T_H  �___int_wchar_t_h  �__INT_WCHAR_T_H  �_GCC_WCHAR_T  �_WCHAR_T_DECLARED  �_BSD_WCHAR_T_ �__need_wchar_t �_WINT_T  �__need_wint_t �NULL �NULL ((void *)0) �__need_NULL __need___va_list  	"__need___va_list '__GNUC_VA_LIST  !_IOREAD 1 "_IOWRT 2 #_IORW 0x0080 *STDIN_FILENO 0 +STDOUT_FILENO 1 ,STDERR_FILENO 2 /EOF (-1) 7FILENAME_MAX (260) >FOPEN_MAX (20) ATMP_MAX 32767 H_P_tmpdir "\\" L_wP_tmpdir L"\\" TL_tmpnam (16) V_IOFBF 0x0000 W_IOLBF 0x0040 X_IONBF 0x0004 Z_IOMYBUF 0x0008 [_IOEOF 0x0010 \_IOERR 0x0020 ]_IOSTRG 0x0040 eBUFSIZ 512 jSEEK_SET 0 kSEEK_CUR 1 lSEEK_END 2 r__VALIST __gnuc_va_list �_FILE_DEFINED  �stdin (&_iob[STDIN_FILENO]) �stdout (&_iob[STDOUT_FILENO]) �stderr (&_iob[STDERR_FILENO]) �__mingw_stdio_redirect__ �__mingw_stdio_redirect__(F) __cdecl __MINGW_NOTHROW __mingw_ ##F �__mingw_stdio_redirect__ �__mingw_stdio_redirect__ static __inline__ __cdecl __MINGW_NOTHROW �__mingw_stdio_redirect__ �__mingw_stdio_redirect__(F) __cdecl __MINGW_NOTHROW __msvcrt_ ##F �__mingw_stdio_redirect__ �feof(__F) ((__F)->_flag & _IOEOF) �ferror(__F) ((__F)->_flag & _IOERR) �_WSTDIO_DEFINED  OBJECT_TYPE char  OBJECT_FLAGS unsigned char "OBJECT_REF_COUNT short /STREAM_NUM long 1INDEX long 4NUM long 6REL_NUM long 9STREAM_TYPE_ID char <INT long >FLOAT float BBOOL unsigned char DEBUG_H  $
assert +assert(e) ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__)) 'STROPS_H  MEMORY_H  _STDLIB_H_  __need_size_t  __need_wchar_t  __need_NULL  �__need_size_t �__need_wchar_t �NULL �NULL ((void *)0) �__need_NULL RAND_MAX 0x7FFF !EXIT_SUCCESS 0 "EXIT_FAILURE 1 oMB_CUR_MAX __mb_cur_max �errno (*_errno()) �_doserrno (*__doserrno()) �_WSTDLIB_DEFINED  
assert +assert(e) ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__)) 1MAX_MEM_CHUNKS 200000000 	_STDARG_H   _ANSI_STDARG_H_  "__need___va_list /va_start(v,l) __builtin_va_start(v,l) 0va_end(v) __builtin_va_end(v) 1va_arg(v,l) __builtin_va_arg(v,l) 3va_copy(d,s) __builtin_va_copy(d,s) 5__va_copy(d,s) __builtin_va_copy(d,s) l_VA_LIST_  o_VA_LIST  r_VA_LIST_DEFINED  u_VA_LIST_T_H  x__va_list__  !_STRING_H_  __need_size_t  __need_wchar_t  __need_NULL  �__need_size_t �__need_wchar_t �NULL �NULL ((void *)0) �__need_NULL �_WSTRING_DEFINED  .DEBUG_ALL 0 /DEBUG_MEMORY 1 0DEBUG_SHOW_OPCODES 2 1DEBUG_FULL_DUMP 4 2DEBUG_STACK 8 3DEBUG_LISTS 16 4DEBUG_GC 32 5DEBUG_VERBOSE_STEP 64 6DEBUG_INTERNAL_FUNCTIONS 128 7DEBUG_VM 256 8DEBUG_FREEING 512 9DEBUG_ALLOCS 1024 :DEBUG_DUMP_UNSUPPORTED 2048 ;DEBUG_DUMP_OBJECT 4096 <DEBUG_INTERACTIVE 8192 =DEBUG_COUNT_OBJECTS 16384 >DEBUG_CREATION 32768 ?DEBUG_VERBOSE_FREEING 65536 @DEBUG_PTR_LISTS 131072 ADEBUG_VERBOSE_TESTS 262144 GDEBUG if(1) HDEBUG_BLOCK if(1) { IDEBUG_BLOCK_END }  OPCODE_STOP_CODE 0x00 !OPCODE_POP_TOP 0x01 "OPCODE_ROT_TWO 0x02 #OPCODE_ROT_THREE 0x03 $OPCODE_DUP_TOP 0x04 %OPCODE_DUP_TOP_TWO 0x05 &OPCODE_NOP 0x09 'OPCODE_UNARY_POSITIVE 0x0a (OPCODE_UNARY_NEGATIVE 0x0b )OPCODE_UNARY_NOT 0x0c *OPCODE_UNARY_INVERT 0x0f +OPCODE_BINARY_POWER 0x13 ,OPCODE_BINARY_MULTIPLY 0x14 -OPCODE_BINARY_DIVIDE 0x15 .OPCODE_BINARY_MODULO 0x16 /OPCODE_BINARY_ADD 0x17 0OPCODE_BINARY_SUBTRACT 0x18 1OPCODE_BINARY_SUBSCR 0x19 2OPCODE_BINARY_FLOOR_DIVIDE 0x1a 3OPCODE_BINARY_TRUE_DIVIDE 0x1b 4OPCODE_INPLACE_FLOOR_DIVIDE 0x1c 5OPCODE_INPLACE_TRUE_DIVIDE 0x1d 6OPCODE_STORE_MAP 0x36 7OPCODE_INPLACE_ADD 0x37 8OPCODE_INPLACE_SUBTRACT 0x38 9OPCODE_INPLACE_MULTIPLY 0x39 :OPCODE_INPLACE_DIVIDE 0x3a ;OPCODE_INPLACE_MODULO 0x3b <OPCODE_STORE_SUBSCR 0x3c =OPCODE_DELETE_SUBSCR 0x3d >OPCODE_BINARY_LSHIFT 0x3e ?OPCODE_BINARY_RSHIFT 0x3f @OPCODE_BINARY_AND 0x40 AOPCODE_BINARY_XOR 0x41 BOPCODE_BINARY_OR 0x42 COPCODE_INPLACE_POWER 0x43 DOPCODE_GET_ITER 0x44 EOPCODE_STORE_LOCALS 0x45 FOPCODE_PRINT_EXPR 0x46 GOPCODE_LOAD_BUILD_CLASS 0x47 HOPCODE_INPLACE_LSHIFT 0x4b IOPCODE_INPLACE_RSHIFT 0x4c JOPCODE_INPLACE_AND 0x4d KOPCODE_INPLACE_XOR 0x4e LOPCODE_INPLACE_OR 0x4f MOPCODE_BREAK_LOOP 0x50 NOPCODE_WITH_CLEANUP 0x51 OOPCODE_RETURN_VALUE 0x53 POPCODE_IMPORT_STAR 0x54 QOPCODE_YIELD_VALUE 0x56 ROPCODE_POP_BLOCK 0x57 SOPCODE_END_FINALLY 0x58 TOPCODE_BUILD_CLASS 0x59 UOPCODE_STORE_NAME 0x5a VOPCODE_DELETE_NAME 0x5b WOPCODE_UNPACK_SEQUENCE 0x5c XOPCODE_FOR_ITER 0x5d YOPCODE_UNPACK_EX 0x5e ZOPCODE_STORE_ATTR 0x5f [OPCODE_DELETE_ATTR 0x60 \OPCODE_STORE_GLOBAL 0x61 ]OPCODE_DELETE_GLOBAL 0x62 ^OPCODE_LOAD_CONST 0x64 _OPCODE_LOAD_NAME 0x65 `OPCODE_BUILD_TUPLE 0x66 aOPCODE_BUILD_LIST 0x67 bOPCODE_BUILD_SET 0x68 cOPCODE_BUILD_MAP 0x69 dOPCODE_LOAD_ATTR 0x6a eOPCODE_COMPARE_OP 0x6b fOPCODE_IMPORT_NAME 0x6c gOPCODE_IMPORT_FROM 0x6d hOPCODE_JUMP_FORWARD 0x6e iOPCODE_JUMP_IF_FALSE 0x6f jOPCODE_JUMP_IF_TRUE 0x70 kOPCODE_JUMP_ABSOLUTE 0x71 lOPCODE_POP_JUMP_IF_FALSE 0x72 mOPCODE_POP_JUMP_IF_TRUE 0x73 nOPCODE_LOAD_GLOBAL 0x74 oOPCODE_SETUP_LOOP 0x78 pOPCODE_SETUP_EXCEPT 0x79 qOPCODE_SETUP_FINALLY 0x7a rOPCODE_LOAD_FAST 0x7c sOPCODE_STORE_FAST 0x7d tOPCODE_DELETE_FAST 0x7e uOPCODE_RAISE_VARARGS 0x82 vOPCODE_CALL_FUNCTION 0x83 wOPCODE_MAKE_FUNCTION 0x84 xOPCODE_BUILD_SLICE 0x85 yOPCODE_MAKE_CLOSURE 0x86 zOPCODE_LOAD_CLOSURE 0x87 {OPCODE_LOAD_DEREF 0x88 |OPCODE_STORE_DEREF 0x89 }OPCODE_DELETE_DEREF 0x8a ~OPCODE_CALL_FUNCTION_VAR 0x8c OPCODE_CALL_FUNCTION_KW 0x8d �OPCODE_CALL_FUNCTION_VAR_KW 0x8e �OPCODE_SETUP_WITH 0x8f �OPCODE_EXTENDED_ARG 0x90 �OPCODE_LIST_APPEND 0x91 �OPCODE_SET_ADD 0x92 �OPCODE_MAP_ADD 0x93  opcodecount opcode debug_level        zR |�            +    A�BY�M�A� (   @   /   8   A�BK��'�A�A�      l   g  C    A�B�     �   �  O    A�BK�     $    5       ;       U       x       �   #    �       �       �       �       �   %    
            ;      G  %    O      �      �      �      �      D       L       T       \       d       l       t       |       �       �       �       �       �-      �-      �-      �-      �-      �-       .      .      .      .      (.      ,.      <.      @.      P.      T.      d.      h.      x.      |.      �.      �.      �.      �.      �.      �.      �.      �.      �.      �.      �.      �.      /      /      /      /      ,/      0/      @/      D/      T/      X/      h/      l/      |/      �/      �/      �/      �/      �/      �/      �/      �/      �/      �/      �/      �/      �/      0      0      0       0      00      40      D0      H0      X0      \0      l0      p0      �0      �0      �0      �0      �0      �0      �0      �0      �0      �0      �0      �0      �0      �0      1      1       1      $1      41      81      H1      L1      \1      `1      p1      t1      �1      �1      �1      �1      �1      �1      �1      �1      �1      �1      �1      �1      �1       2      2      2      $2      (2      82      <2      L2      P2      `2      d2      t2      x2      �2      �2      �2      �2      �2      �2      �2      �2      �2      �2      �2      �2       3      3      3      3      (3      ,3      <3      @3      P3      T3      d3      h3      x3      |3      �3      �3      �3      �3      �3      �3      �3      �3      �3      �3      �3      �3      4      4      4      4      ,4      04      @4      D4      T4      X4      h4      l4      |4      �4      �4      �4      �4      �4      �4      �4      �4      �4      �4      �4      �4      �4      5      5      5       5      05      45      D5      H5      X5      \5      l5      p5      �5      �5      �5      �5      �5      �5             P       T       X       \       �            7      ;      ?      �      �      �      �      �      �      �      �                  >      B      F      O      i      m      �      �      �            5      ?      L      b                    1              D       p       �       .file       ��  gopcodes.c         _printf                              h   +           �   c          �   �      .text          �               .data                            .bss                             .rdata         6  �                 �          l  "                 �          m                    �          �                     �                               �       	   �                   �       
   C5                    �                                        �                              _cmp_ops@             �       _opcodes�-          '               4               E           S  .debug_info .debug_abbrev .debug_loc .debug_aranges .debug_line .debug_macinfo .debug_str .eh_frame _DumpUnsupportedOpCodes _GetSupportedOpcodesNum _GetOpcodeIndex .debug_info .debug_abbrev .debug_loc .debug_aranges .debug_line .debug_macinfo .debug_str .eh_frame _cmp_opscount _opcodecount _debug_level ___mingw_vprintf _debug_printf 