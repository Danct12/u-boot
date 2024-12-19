// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (c) 2023 Alexander Warnecke <awarnecke002@hotmail.com>
 */

#include <backlight.h>
#include <dm.h>
#include <mipi_dsi.h>
#include <panel.h>
#include <asm/gpio.h>
#include <dm/device_compat.h>
#include <linux/delay.h>
#include <power/regulator.h>

struct th101mb31ig002_28a_panel_priv {
	struct udevice *backlight;
	struct udevice *reg_power;
	struct gpio_desc enable;
	struct gpio_desc reset;
};

static const struct display_timing default_timing = {
	.pixelclock.typ 	= 73500000,
	.hactive.typ 		= 800,
	.hfront_porch.typ 	= 64,
	.hback_porch.typ 	= 64,
	.hsync_len.typ 		= 16,
	.vactive.typ 		= 1280,
	.vfront_porch.typ 	= 2,
	.vback_porch.typ 	= 12,
	.vsync_len.typ 		= 4,
};

#define dsi_dcs_write_seq(device, seq...) do {					\
		static const u8 d[] = { seq };					\
		int ret;							\
		ret = mipi_dsi_dcs_write_buffer(device, d, ARRAY_SIZE(d));	\
		if (ret < 0)							\
			return ret;						\
	} while (0)

static int th101mb31ig002_28a_init_sequence(struct udevice *dev)
{
	struct mipi_dsi_panel_plat *plat = dev_get_plat(dev);
	struct mipi_dsi_device *device = plat->device;
	int ret;

	dsi_dcs_write_seq(device, 0xE0, 0xAB, 0xBA);
	dsi_dcs_write_seq(device, 0xE1, 0xBA, 0xAB);
	dsi_dcs_write_seq(device, 0xB1, 0x10, 0x01, 0x47, 0xFF);
	dsi_dcs_write_seq(device, 0xB2, 0x0C, 0x14, 0x04, 0x50, 0x50, 0x14);
	dsi_dcs_write_seq(device, 0xB3, 0x56, 0x53, 0x00);
	dsi_dcs_write_seq(device, 0xB4, 0x33, 0x30, 0x04);
	dsi_dcs_write_seq(device, 0xB6, 0xB0, 0x00, 0x00, 0x10, 0x00, 0x10, 0x00);
	dsi_dcs_write_seq(device, 0xB8, 0x05, 0x12, 0x29, 0x49, 0x48, 0x00, 0x00);
	dsi_dcs_write_seq(device, 0xB9, 0x7C, 0x65, 0x55, 0x49, 0x46, 0x36, 0x3B, 0x24, 0x3D, 0x3C, 0x3D, 0x5C, 0x4C, 0x55, 0x47, 0x46, 0x39, 0x26, 0x06, 0x7C, 0x65, 0x55, 0x49, 0x46, 0x36, 0x3B, 0x24, 0x3D, 0x3C, 0x3D, 0x5C, 0x4C, 0x55, 0x47, 0x46, 0x39, 0x26, 0x06);
	dsi_dcs_write_seq(device, 0xC0, 0xFF, 0x87, 0x12, 0x34, 0x44, 0x44, 0x44, 0x44, 0x98, 0x04, 0x98, 0x04, 0x0F, 0x00, 0x00, 0xC1);
	dsi_dcs_write_seq(device, 0xC1, 0x54, 0x94, 0x02, 0x85, 0x9F, 0x00, 0x7F, 0x00, 0x54, 0x00);
	dsi_dcs_write_seq(device, 0xC2, 0x17, 0x09, 0x08, 0x89, 0x08, 0x11, 0x22, 0x20, 0x44, 0xFF, 0x18, 0x00);
	dsi_dcs_write_seq(device, 0xC3, 0x86, 0x46, 0x05, 0x05, 0x1C, 0x1C, 0x1D, 0x1D, 0x02, 0x1F, 0x1F, 0x1E, 0x1E, 0x0F, 0x0F, 0x0D, 0x0D, 0x13, 0x13, 0x11, 0x11, 0x00);
	dsi_dcs_write_seq(device, 0xC4, 0x07, 0x07, 0x04, 0x04, 0x1C, 0x1C, 0x1D, 0x1D, 0x02, 0x1F, 0x1F, 0x1E, 0x1E, 0x0E, 0x0E, 0x0C, 0x0C, 0x12, 0x12, 0x10, 0x10, 0x00);
	dsi_dcs_write_seq(device, 0xC6, 0x2A, 0x2A);
	dsi_dcs_write_seq(device, 0xC8, 0x21, 0x00, 0x31, 0x42, 0x34, 0x16);
	dsi_dcs_write_seq(device, 0xCA, 0xCB, 0x43);
	dsi_dcs_write_seq(device, 0xCD, 0x0E, 0x4B, 0x4B, 0x20, 0x19, 0x6B, 0x06, 0xB3);
	dsi_dcs_write_seq(device, 0xD2, 0xE3, 0x2B, 0x38, 0x00);
	dsi_dcs_write_seq(device, 0xD4, 0x00, 0x01, 0x00, 0x0E, 0x04, 0x44, 0x08, 0x10, 0x00, 0x00, 0x00);
	dsi_dcs_write_seq(device, 0xE6, 0x80, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF);
	dsi_dcs_write_seq(device, 0xF0, 0x12, 0x03, 0x20, 0x00, 0xFF);
	dsi_dcs_write_seq(device, 0xF3, 0x00);

	ret = mipi_dsi_dcs_exit_sleep_mode(device);
	if (ret)
		return ret;

	mdelay(120);

	ret = mipi_dsi_dcs_set_display_on(device);
	if (ret)
		return ret;

	return 0;
}

static int th101mb31ig002_28a_panel_enable_backlight(struct udevice *dev)
{
	struct mipi_dsi_panel_plat *plat = dev_get_plat(dev);
	struct mipi_dsi_device *device = plat->device;
	struct th101mb31ig002_28a_panel_priv *priv = dev_get_priv(dev);
	int ret;

	ret = mipi_dsi_attach(device);
	if (ret < 0) {
		printf("mipi_dsi_attach failed %d\n", ret);
		return ret;
	}

	ret = th101mb31ig002_28a_init_sequence(dev);
	if (ret) {
		printf("hx8394_init_sequence failed %d\n", ret);
		return ret;
	}

	if (priv->backlight) {
		ret = backlight_enable(priv->backlight);
		if (ret) {
			printf("backlight enabled failed %d\n", ret);
			return ret;
		}

		backlight_set_brightness(priv->backlight, 60);
	}

	mdelay(10);

	return 0;
}

static int th101mb31ig002_28a_panel_get_display_timing(struct udevice *dev,
					   struct display_timing *timings)
{
	memcpy(timings, &default_timing, sizeof(*timings));

	return 0;
}

static int th101mb31ig002_28a_panel_of_to_plat(struct udevice *dev)
{
	struct th101mb31ig002_28a_panel_priv *priv = dev_get_priv(dev);
	int ret;

	if (CONFIG_IS_ENABLED(DM_REGULATOR)) {
		ret =  device_get_supply_regulator(dev, "power-supply",
						   &priv->reg_power);
		if (ret && ret != -ENOENT) {
			dev_err(dev, "Warning: cannot get power supply\n");
			return ret;
		}
	}

	ret = uclass_get_device_by_phandle(UCLASS_PANEL_BACKLIGHT, dev,
					   "backlight", &priv->backlight);
	if (ret)
		dev_warn(dev, "failed to get backlight\n");

	ret = gpio_request_by_name(dev, "enable-gpios", 0, &priv->enable,
				   GPIOD_IS_OUT);
	if (ret) {
		dev_err(dev, "Failed to get enable GPIO (%d)\n", ret);
		if (ret != -ENOENT)
			return ret;
	}

	ret = gpio_request_by_name(dev, "reset-gpios", 0, &priv->reset,
				   GPIOD_IS_OUT);
	if (ret) {
		dev_err(dev, "Failed to get reset GPIO (%d)\n", ret);
		if (ret != -ENOENT)
			return ret;
	}

	return 0;
}

static int th101mb31ig002_28a_panel_probe(struct udevice *dev)
{
	struct th101mb31ig002_28a_panel_priv *priv = dev_get_priv(dev);
	struct mipi_dsi_panel_plat *plat = dev_get_plat(dev);
	int ret;

	if (CONFIG_IS_ENABLED(DM_REGULATOR) && priv->reg_power) {
		ret = regulator_set_enable(priv->reg_power, true);
		if (ret)
			return ret;
	}

	/* enable panel */
	dm_gpio_set_value(&priv->enable, 1);
	mdelay(50);

	/* reset panel */
	dm_gpio_set_value(&priv->reset, 0);
	udelay(100);
	dm_gpio_set_value(&priv->reset, 1);
	udelay(100);
	dm_gpio_set_value(&priv->reset, 0);
	udelay(6000);

	plat->lanes = 4;
	plat->format = MIPI_DSI_FMT_RGB888;
	plat->mode_flags = MIPI_DSI_MODE_VIDEO |
			   MIPI_DSI_MODE_VIDEO_BURST |
			   MIPI_DSI_MODE_EOT_PACKET |
			   MIPI_DSI_MODE_LPM;

	return 0;
}

static const struct panel_ops th101mb31ig002_28a_panel_ops = {
	.enable_backlight = th101mb31ig002_28a_panel_enable_backlight,
	.get_display_timing = th101mb31ig002_28a_panel_get_display_timing,
};

static const struct udevice_id th101mb31ig002_28a_ids[] = {
	{ .compatible = "boe,th101mb31ig002-28a", },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(th101mb31ig002_28a_panel) = {
	.name 		= "th101mb31ig002_28a_panel",
	.id 		= UCLASS_PANEL,
	.of_match 	= th101mb31ig002_28a_ids,
	.ops		= &th101mb31ig002_28a_panel_ops,
	.of_to_plat	= th101mb31ig002_28a_panel_of_to_plat,
	.probe		= th101mb31ig002_28a_panel_probe,
	.plat_auto	= sizeof(struct mipi_dsi_panel_plat),
	.priv_auto	= sizeof(struct th101mb31ig002_28a_panel_priv),
};
