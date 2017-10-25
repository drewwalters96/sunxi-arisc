/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef ERROR_H
#define ERROR_H

#define SUCCESS 0    /* The operation succeeded. */
#define EAGAIN  (-1) /* Try again immediately. */
#define EBUSY   (-2) /* Try again later. */
#define EEXIST  (-3) /* The object already exists. */
#define ENOBUS  (-4) /* The bus for this device does not exist. */
#define ENODEV  (-5) /* This device does not exist. */
#define ENOTSUP (-6) /* The operation is not supported. */

#endif /* ERROR_H */
