// /*AFLA*/ /*
// /*AFLA*/  * A generic stack depot implementation
// /*AFLA*/  *
// /*AFLA*/  * Author: Alexander Potapenko <glider@google.com>
// /*AFLA*/  * Copyright (C) 2016 Google, Inc.
// /*AFLA*/  *
// /*AFLA*/  * Based on code by Dmitry Chernenkov.
// /*AFLA*/  *
// /*AFLA*/  * This program is free software; you can redistribute it and/or modify
// /*AFLA*/  * it under the terms of the GNU General Public License as published by
// /*AFLA*/  * the Free Software Foundation; either version 2 of the License, or
// /*AFLA*/  * (at your option) any later version.
// /*AFLA*/  *
// /*AFLA*/  * This program is distributed in the hope that it will be useful,
// /*AFLA*/  * but WITHOUT ANY WARRANTY; without even the implied warranty of
// /*AFLA*/  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// /*AFLA*/  * GNU General Public License for more details.
// /*AFLA*/  *
// /*AFLA*/  */
// /*AFLA*/ 
// /*AFLA*/ #ifndef _LINUX_STACKDEPOT_H
// /*AFLA*/ #define _LINUX_STACKDEPOT_H
// /*AFLA*/ 
// /*AFLA*/ typedef u32 depot_stack_handle_t;
// /*AFLA*/ 
// /*AFLA*/ struct stack_trace;
// /*AFLA*/ 
// /*AFLA*/ depot_stack_handle_t depot_save_stack(struct stack_trace *trace, gfp_t flags);
// /*AFLA*/ 
// /*AFLA*/ void depot_fetch_stack(depot_stack_handle_t handle, struct stack_trace *trace);
// /*AFLA*/ 
// /*AFLA*/ #endif
