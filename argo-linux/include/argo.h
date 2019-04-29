/******************************************************************************
 * drivers/xen/argo.h
 *
 * Argo hypervisor-mediated data exchange interdomain communication driver.
 *
 * Copyright (c) 2009 Ross Philipson
 * Copyright (c) 2009 James McKenzie
 * Copyright (c) 2009 Citrix Systems, Inc.
 * Modifications by Christopher Clark are Copyright (c) 2018 BAE Systems
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation; or, when distributed
 * separately from the Linux kernel or incorporated into other
 * software packages, subject to the following license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this source file (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <xen/page.h>
#include <asm/xen/hypercall.h>
#include <xen/xen.h>
#include <xen/events.h>

#ifndef HYPERVISOR_argo_op
#define __HYPERVISOR_argo_op               39

#if defined(CONFIG_X86_64) || defined(CONFIG_X86_32)

#ifndef HYPERCALL_PHYSICAL_ADDRESS
#define HYPERCALL_PHYSICAL_ADDRESS    0x00080000
#endif

#ifndef hcall_addr
#define hcall_addr(name)                                                \
    ((unsigned long)HYPERCALL_PHYSICAL_ADDRESS + __HYPERVISOR_##name * 32)
#endif

#ifndef _hypercall5
#define _hypercall5(type, name, a1, a2, a3, a4, a5)     \
({                                                      \
    long __res, __ign1, __ign2, __ign3, __ign4, __ign5; \
    asm volatile (                                      \
        "call *%%eax"                                   \
        : "=a" (__res), "=b" (__ign1), "=c" (__ign2),   \
          "=d" (__ign3), "=S" (__ign4), "=D" (__ign5)   \
        : "0" (hcall_addr(name)),                       \
          "1" ((long)(a1)), "2" ((long)(a2)),           \
          "3" ((long)(a3)), "4" ((long)(a4)),           \
          "5" ((long)(a5))                              \
        : "memory" );                                   \
    (type)__res;                                        \
})
#endif

static inline int __must_check
HYPERVISOR_argo_op(int cmd, void *arg1, void *arg2, uint32_t arg3,
                   uint32_t arg4)
{
    int ret;

    stac();
    ret = _hypercall5(int, argo_op, cmd, arg1, arg2, arg3, arg4);
    clac();

    return ret;
}
#else
int __must_check
HYPERVISOR_argo_op(int cmd, void *arg1, void *arg2, uint32_t arg3,
                   uint32_t arg4);
#endif
#endif

#ifndef VIRQ_ARGO
#define VIRQ_ARGO       11 /* G. (DOM0) ARGO interdomain communication */
#endif
