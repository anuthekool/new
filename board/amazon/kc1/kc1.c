/*
 * Amazon Kindle Fire (first generation) codename kc1 config
 *
 * Copyright (C) 2016 Paul Kocialkowski <contact@paulk.fr>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <config.h>
#include <common.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/gpio.h>
#include <asm/emif.h>
#include <twl6030.h>
#include "kc1.h"

DECLARE_GLOBAL_DATA_PTR;

const struct omap_sysinfo sysinfo = {
	.board_string = "kc1"
};

void set_muxconf_regs(void)
{
	do_set_mux((*ctrl)->control_padconf_core_base, core_padconf_array,
		sizeof(core_padconf_array) / sizeof(struct pad_conf_entry));
}

struct lpddr2_device_details *emif_get_device_details(u32 emif_nr, u8 cs,
	struct lpddr2_device_details *lpddr2_dev_details)
{
	if (cs == CS1)
		return NULL;

	*lpddr2_dev_details = elpida_2G_S4_details;

	return lpddr2_dev_details;
}

void emif_get_device_timings(u32 emif_nr,
	const struct lpddr2_device_timings **cs0_device_timings,
	const struct lpddr2_device_timings **cs1_device_timings)
{
	*cs0_device_timings = &elpida_2G_S4_timings;
	*cs1_device_timings = NULL;
}

int board_init(void)
{
	/* GPMC init */
	gpmc_init();

	/* MACH number */
	gd->bd->bi_arch_number = MACH_TYPE_OMAP_4430SDP;

	/* ATAGs location */
	gd->bd->bi_boot_params = OMAP44XX_DRAM_ADDR_SPACE_START + 0x100;

	return 0;
}

int misc_init_r(void)
{
	/* Serial number */

	omap_die_id_serial();

	return 0;
}

u32 get_board_rev(void)
{
	u32 value = 0;

	gpio_request(KC1_GPIO_MBID0, "MBID0");
	gpio_request(KC1_GPIO_MBID1, "MBID1");
	gpio_request(KC1_GPIO_MBID2, "MBID2");
	gpio_request(KC1_GPIO_MBID3, "MBID3");

	gpio_direction_input(KC1_GPIO_MBID0);
	gpio_direction_input(KC1_GPIO_MBID1);
	gpio_direction_input(KC1_GPIO_MBID2);
	gpio_direction_input(KC1_GPIO_MBID3);

	value |= (gpio_get_value(KC1_GPIO_MBID0) << 0);
	value |= (gpio_get_value(KC1_GPIO_MBID1) << 1);
	value |= (gpio_get_value(KC1_GPIO_MBID2) << 2);
	value |= (gpio_get_value(KC1_GPIO_MBID3) << 3);

	return value;
}

void get_board_serial(struct tag_serialnr *serialnr)
{
	omap_die_id_get_board_serial(serialnr);
}

#ifndef CONFIG_SPL_BUILD
int board_mmc_init(bd_t *bis)
{
	return omap_mmc_init(1, 0, 0, -1, -1);
}
#endif

void board_mmc_power_init(void)
{
	twl6030_power_mmc_init(1);
}