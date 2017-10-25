/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef SUNXI_CCU_H
#define SUNXI_CCU_H

#define CCU_GATE(n)       (((n) & 0xff) << 24)
#define CCU_GET_GATE(n)   (((n) >> 24) & 0xff)
#define CCU_RESET(n)      (((n) & 0xff) << 16)
#define CCU_GET_RESET(n)  (((n) >> 16) & 0xff)
#define CCU_MODULE(n)     ((n) & 0xffff)
#define CCU_GET_MODULE(n) ((n) & 0xffff)

#endif /* SUNXI_CCU_H */
