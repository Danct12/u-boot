// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2017 Theobroma Systems Design und Consulting GmbH
 * Copyright (c) 2015 Google, Inc
 * Copyright 2014 Rockchip Inc.
 */

#include <display.h>
#include <dm.h>
#include <log.h>
#include <regmap.h>
#include <video.h>
#include <asm/arch-rockchip/hardware.h>
#include <asm/global_data.h>
#include <linux/bitfield.h>
#include "rk_vop2.h"

DECLARE_GLOBAL_DATA_PTR;

#define M_MIPI_INFACE_MUX (3 << 16)
#define M_HDMI_INFACE_MUX (3 << 10)

#define V_MIPI_INFACE_MUX(x) (((x) & 3) << 16)
#define V_HDMI_INFACE_MUX(x) (((x) & 3) << 10)

#define M_MIPI_POL (0xf << 16)
#define M_EDP_POL (0xf << 12)
#define M_HDMI_POL (0xf << 4)
#define M_LVDS_POL (0xf << 0)

#define V_MIPI_POL(x) (((x) & 0xf) << 16)
#define V_EDP_POL(x) (((x) & 0xf) << 12)
#define V_HDMI_POL(x) (((x) & 0xf) << 4)
#define V_LVDS_POL(x) (((x) & 0xf) << 0)

#define M_BT656_OUT_EN (1 << 7)
#define M_BT1120_OUT_EN (1 << 6)
#define M_LVDS_OUT_EN (1 << 5)
#define M_MIPI_OUT_EN (1 << 4)
#define M_EDP_OUT_EN (1 << 3)
#define M_HDMI_OUT_EN (1 << 1)
#define M_RGB_OUT_EN (1 << 0)

#define M_ALL_OUT_EN (M_BT656_OUT_EN | M_BT1120_OUT_EN | M_LVDS_OUT_EN | M_MIPI_OUT_EN | M_EDP_OUT_EN | M_HDMI_OUT_EN | M_RGB_OUT_EN)

#define V_BT656_OUT_EN(x) (((x) & 1) << 7)
#define V_BT1120_OUT_EN(x) (((x) & 1) << 6)
#define V_LVDS_OUT_EN(x) (((x) & 1) << 5)
#define V_MIPI_OUT_EN(x) (((x) & 1) << 4)
#define V_EDP_OUT_EN(x) (((x) & 1) << 3)
#define V_HDMI_OUT_EN(x) (((x) & 1) << 1)
#define V_RGB_OUT_EN(x) (((x) & 1) << 0)


static void rk3568_set_output(struct udevice *dev,
				    enum vop_modes mode, u32 port)
{
	struct rk_vop2_priv *priv = dev_get_priv(dev);
	struct rk3568_vop_sysctrl *sysctrl = priv->regs + VOP2_SYSREG_OFFSET;

	switch (mode) {
	case VOP_MODE_HDMI:
		clrsetbits_le32(&sysctrl->dsp_en,
				M_HDMI_INFACE_MUX, V_HDMI_INFACE_MUX(port));
		break;

	case VOP_MODE_MIPI:
		clrsetbits_le32(&sysctrl->dsp_en,
				M_MIPI_INFACE_MUX, V_MIPI_INFACE_MUX(port));
		break;

	default:
		debug("%s: unsupported output mode %x\n", __func__, mode);
		return;
	}
}

static void rk3568_enable_output(struct udevice *dev, enum vop_modes mode)
{
	struct rk_vop2_priv *priv = dev_get_priv(dev);
	struct rk3568_vop_sysctrl *sysctrl = priv->regs + VOP2_SYSREG_OFFSET;

	switch (mode) {
	case VOP_MODE_HDMI:
		clrsetbits_le32(&sysctrl->dsp_en,
				M_ALL_OUT_EN, V_HDMI_OUT_EN(1));
		break;

	case VOP_MODE_MIPI:
		clrsetbits_le32(&sysctrl->dsp_en,
				M_ALL_OUT_EN, V_MIPI_OUT_EN(1));
		break;

	default:
		debug("%s: unsupported output mode %x\n", __func__, mode);
		return;
	}
}

static void rk3568_set_pin_polarity(struct udevice *dev,
				    enum vop_modes mode, u32 polarity)
{
	struct rk_vop2_priv *priv = dev_get_priv(dev);
	struct rk3568_vop_sysctrl *sysctrl = priv->regs + VOP2_SYSREG_OFFSET;

	switch (mode) {
	case VOP_MODE_HDMI:
		clrsetbits_le32(&sysctrl->dsp_pol,
				M_HDMI_POL, V_HDMI_POL(polarity));
		break;

	case VOP_MODE_MIPI:
		clrsetbits_le32(&sysctrl->dsp_pol,
				M_MIPI_POL, V_MIPI_POL(polarity));
		break;

	default:
		debug("%s: unsupported output mode %x\n", __func__, mode);
		return;
	}
}

static int rk3568_vop_probe(struct udevice *dev)
{
	/* Before relocation we don't need to do anything */
	if (!(gd->flags & GD_FLG_RELOC))
		return 0;

	return rk_vop2_probe(dev);
}

struct rkvop_driverdata rk3568_driverdata = {
	.set_pin_polarity = rk3568_set_pin_polarity,
	.enable_output = rk3568_enable_output,
	.set_output = rk3568_set_output,
};

static const struct udevice_id rk3568_vop_ids[] = {
	{ .compatible = "rockchip,rk3566-vop",
	  .data = (ulong)&rk3568_driverdata },
	{ .compatible = "rockchip,rk3568-vop",
	  .data = (ulong)&rk3568_driverdata },
	{ }
};

static const struct video_ops rk3568_vop_ops = {
};

U_BOOT_DRIVER(rk3568_vop) = {
	.name	= "rk3568_vop",
	.id	= UCLASS_VIDEO,
	.of_match = rk3568_vop_ids,
	.ops	= &rk3568_vop_ops,
	.bind	= rk_vop2_bind,
	.probe	= rk3568_vop_probe,
	.priv_auto	= sizeof(struct rk_vop2_priv),
};
