/* arch/arm/mach-capri/board-baffin-spi.c
 *
 * Copyright (C) 2011 Samsung Electronics Co, Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/gpio.h>


#include <linux/irq.h>
#include <linux/spi/spi.h>
//#include <mach/msm8930-gpio.h>
#include <linux/platform_device.h>
#include <mach/gpiomux.h>


#define GPIO_IPC_MRDY		104
#define GPIO_IPC_SUB_MRDY	105
#define GPIO_IPC_SRDY		143
#define GPIO_IPC_SUB_SRDY	117
#define GPIO_CP_DUMP_INT	73



struct ipc_spi_platform_data {
	const char *name;
	unsigned gpio_ipc_mrdy;
	unsigned gpio_ipc_srdy;
	unsigned gpio_ipc_sub_mrdy;
	unsigned gpio_ipc_sub_srdy;
	unsigned gpio_cp_dump_int;

	void (*cfg_gpio)(void);
};

static struct ipc_spi_platform_data spi_modem_data;

#if 0
struct bcom_mcspi_device_config {
	unsigned turbo_mode:1;
	/* Do we want one channel enabled at the same time? */
	unsigned single_channel:1;
	};

static struct bcom_mcspi_device_config board_spi_mcspi_config = {
	.turbo_mode =	1,
	.single_channel = 1,
};

static struct spi_board_info modem_if_spi_device[] __initdata = {
	{
		.modalias	= "if_spi_driver",
		.bus_num=2,	// GSCHO
		.max_speed_hz	= 12000000,	//10800000,
		.chip_select	= 0,
		.mode           = SPI_MODE_1,
		.controller_data = &board_spi_mcspi_config,
		//.controller_data = (void *) SPI_GPIO_NO_CHIPSELECT,
		
	},
};
#endif

static struct platform_device spi_modem = {
	.name = "if_spi_platform_driver",
	.id = -1,
	.dev = {
		.platform_data = &spi_modem_data,
	},
};




void spi_modem_cfg_gpio(void)
{
	int ret;
	ret=gpio_request(GPIO_IPC_MRDY, "GPIO_IPC_MRDY");
	if (ret) {
				pr_err("gspi_modem_cfg: gpio_request "
					"failed for %d\n",GPIO_IPC_MRDY);
				;
			}
	gpio_tlmm_config(GPIO_CFG(GPIO_IPC_MRDY, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_set_value(GPIO_IPC_MRDY, 0);



	ret= gpio_request(GPIO_IPC_SUB_MRDY, "GPIO_IPC_SUB_MRDY");
	if (ret) {
				pr_err("spi_modem_cfg: gpio_request "
					"failed for %d\n",GPIO_IPC_SUB_MRDY);
				;
			}
	gpio_tlmm_config(GPIO_CFG(GPIO_IPC_SUB_MRDY, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_OUTPUT, GPIO_CFG_NO_PULL, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);
	gpio_set_value(GPIO_IPC_SUB_MRDY, 0);



	ret=gpio_request(GPIO_IPC_SRDY, "GPIO_IPC_SRDY");
	if (ret) {
				pr_err("spi_modem_cfg_: gpio_request "
					"failed for %d\n",GPIO_IPC_SRDY);
				;
			}
	gpio_tlmm_config(GPIO_CFG(GPIO_IPC_SRDY, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);



	ret=gpio_request(GPIO_IPC_SUB_SRDY, "GPIO_IPC_SUB_SRDY");
	if (ret) {
				pr_err("spi_modem_cfg: gpio_request "
					"failed for %d\n",GPIO_IPC_SUB_SRDY);
				;
			}
	gpio_tlmm_config(GPIO_CFG(GPIO_IPC_SUB_SRDY, GPIOMUX_FUNC_GPIO,
		GPIO_CFG_INPUT, GPIO_CFG_PULL_DOWN, GPIO_CFG_2MA),
		GPIO_CFG_ENABLE);

	spi_modem_data.gpio_ipc_mrdy = GPIO_IPC_MRDY;
	spi_modem_data.gpio_ipc_srdy = GPIO_IPC_SRDY;
	spi_modem_data.gpio_ipc_sub_mrdy = GPIO_IPC_SUB_MRDY;
	spi_modem_data.gpio_ipc_sub_srdy = GPIO_IPC_SUB_SRDY;
	spi_modem_data.gpio_cp_dump_int = GPIO_CP_DUMP_INT;

	pr_info("[SPI] %s done\n", __func__);

}

static int __init init_spi(void)
{
	pr_info("[SPI] %s\n", __func__);
	spi_modem_cfg_gpio();
	platform_device_register(&spi_modem);
/*
	spi_register_board_info(modem_if_spi_device,
		ARRAY_SIZE(modem_if_spi_device));
*/
	return 0;
}

module_init(init_spi);
