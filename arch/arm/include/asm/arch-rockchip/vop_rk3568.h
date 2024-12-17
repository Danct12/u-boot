/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2015 Google, Inc
 * Copyright 2014 Rockchip Inc.
 */

#ifndef _ASM_ARCH_VOP_RK3568_H
#define _ASM_ARCH_VOP_RK3568_H

struct rk3568_vop_sysctrl {
	u32 reg_cfg_done;
	u32 version_info;
	u32 autogating_ctrl;
	u32 win_reg_cfg_done;
	u32 axi_ctrl0;
	u32 axi_hurry_ctrl0;
	u32 axi_hurry_ctrl1;
	u32 axi_outstanding_ctrl0;
	u32 axi_outstanding_ctrl1;
	u32 axi_lut_ctrl;
	u32 dsp_en;
	u32 dsp_ctrl;
	u32 dsp_pol;
	u32 pwr_ctrl;
	u32 var_freq_ctrl;
	u32 mmu_raddr_range;
	u32 wb_ctrl0;
	u32 wb_xspd;
	u32 wb_yrgb_mst;
	u32 wb_cbr_mst;
	u32 otp_win;
	u32 otp_mirr_ctrl;
	u32 lut_port_sel;
	u32 pwr_stable_ctrl;
	u32 status0;
	u32 status1;
	u32 status2;
	u32 status3;
	u32 line_flag0;
	u32 line_flag1;
	u32 line_flag2;
	u32 line_flag3;
	u32 sys0_intr_en;
	u32 sys0_intr_clr;
	u32 sys0_intr_status;
	u32 sys0_intr_status_raw;
	u32 sys1_intr_en;
	u32 sys1_intr_clr;
	u32 sys1_intr_status;
	u32 sys1_intr_status_raw;
	u32 port0_intr_en;
	u32 port0_intr_clr;
	u32 port0_intr_status;
	u32 port0_intr_status_raw;
	u32 port1_intr_en;
	u32 port1_intr_clr;
	u32 port1_intr_status;
	u32 port1_intr_status_raw;
	u32 port2_intr_en;
	u32 port2_intr_clr;
	u32 port2_intr_status;
	u32 port2_intr_status_raw;
	u32 port3_intr_en;
	u32 port3_intr_clr;
	u32 port3_intr_status;
	u32 port3_intr_status_raw;
};
check_member(rk3568_vop_sysctrl, port3_intr_status_raw, 0x00DC);

struct rk3568_vop_overlay {
	u32 overlay_ctrl;
	u32 layer_sel;
	u32 port_sel;
};
check_member(rk3568_vop_overlay, port_sel, 0x0008);

struct rk3568_vop_post {
	u32 dsp_ctrl;
	u32 mipi_ctrl;
	u32 color_ctrl;
	u32 reserved2;
	u32 lut_reserved[4];
	u32 reserved[3];
	u32 dsp_bg;
	u32 prescan_htimings;
	u32 dsp_hact_info;
	u32 dsp_vact_info;
	u32 scl_factor_yrgb;
	u32 scl_ctrl;
	u32 dsp_vact_info_f1;
	u32 dsp_htotal_hs_end;
	u32 dsp_hact_st_end;
	u32 dsp_vtotal_vs_end;
	u32 dsp_vact_st_end;
	u32 dsp_vs_st_end_f1;
	u32 dsp_vact_st_end_f1;
};
check_member(rk3568_vop_post, dsp_vact_st_end_f1, 0x005C);

struct rk3568_vop_esmart {
	u32 esmart_ctrl0;
	u32 esmart_ctrl1;
	u32 reserved0[2];
	u32 esmart_region0_mst_ctl;
	u32 esmart_region0_mst_yrgb;
	u32 esmart_region0_mst_cbcr;
	u32 esmart_region0_vir;
	u32 esmart_region0_act_info;
	u32 esmart_region0_dsp_info;
	u32 esmart_region0_dsp_offset;
	u32 reserved1[1];
	u32 esmart_region0_scl_ctrl;
	u32 esmart_region0_scl_factor_yrgb;
	u32 esmart_region0_scl_factor_cbcr;
	u32 esmart_region0_scl_offset;
};
check_member(rk3568_vop_esmart, esmart_region0_scl_offset, 0x003C);

enum rockchip_fb_data_format_t {
	ARGB8888 = 0,
	RGB888 = 1,
	RGB565 = 2,
};

enum vop_modes {
	VOP_MODE_MIPI = 0,
	VOP_MODE_HDMI,
};

/* OFFSETS */
#define VOP2_SYSREG_OFFSET 0x0
#define VOP2_OVERLAY_OFFSET 0x0600
#define VOP2_POST_OFFSET(n) 0x0c00 + (n * 0x100)
#define VOP2_CLUSTER_OFFSET(n) 0x1000 + (n * 0x200)
#define VOP2_ESMART_OFFSET(n) 0x1800 + (n * 0x200)

/* System Registers */
/* VOP_REG_CFG_DONE */
#define M_GLOBAL_REGDONE (1 << 15)
#define M_LOAD_ESMART1 (1 << 11)
#define M_LOAD_ESMART0 (1 << 10)
#define M_LOAD_GLOBAL1 (1 << 1)
#define M_LOAD_GLOBAL0 (1 << 0)

#define V_GLOBAL_REGDONE(x) (((x) & 1) << 15)
#define V_LOAD_ESMART1(x) (((x) & 1) << 11)
#define V_LOAD_ESMART0(x) (((x) & 1) << 10)
#define V_LOAD_GLOBAL1(x) (((x) & 1) << 1)
#define V_LOAD_GLOBAL0(x) (((x) & 1) << 0)

/* VOP_VERSION_INFO */
#define M_FPGA_VERSION  (0xffff << 16)
#define M_RTL_VERSION (0xffff)

/* VOP_AUTO_GATING_CTRL */
#define M_AUTO_GATING (1 << 31)
#define M_PWMCLK_ACLK (1 << 12)
#define M_OVERLAY_ACLK (1 << 8)
#define M_SMART1_ACLK (1 << 7)
#define M_SMART0_ACLK (1 << 6)
#define M_ESMART1_ACLK (1 << 5)
#define M_ESMART0_ACLK (1 << 4)

#define V_AUTO_GATING(x) (((x) & 1) << 31)
#define V_PWMCLK_ACLK(x) (((x) & 1) << 12)
#define V_OVERLAY_ACLK(x) (((x) & 1) << 8)
#define V_SMART1_ACLK(x) (((x) & 1) << 7)
#define V_SMART0_ACLK(x) (((x) & 1) << 6)
#define V_ESMART1_ACLK(x) (((x) & 1) << 5)
#define V_ESMART0_ACLK(x) (((x) & 1) << 4)

/* VOP_DSP_INFACE_POL */
#define M_DSP_INFACE_REGDONE (1 << 28)
#define V_DSP_INFACE_REGDONE(x) (((x) & 1) << 28)

/* VOP_OTP_WIN_EN */
#define M_OTP_WIN (1 << 0)
#define V_OTP_WIN(x) (((x) & 1) << 0)

/* VOP_OVERLAY_CTRL */
#define M_LAYER_SEL_REGDONE_SEL (3 << 30)
#define M_LAYER_SEL_REGDONE_EN (1 << 28)
#define M_VP2_OVERLAY_MODE (1 << 2)
#define M_VP1_OVERLAY_MODE (1 << 1)
#define M_VP0_OVERLAY_MODE (1 << 0)

#define V_LAYER_SEL_REGDONE_SEL(x) (((x) & 3) << 30)
#define V_LAYER_SEL_REGDONE_EN(x) (((x) & 1) << 28)
#define V_VP2_OVERLAY_MODE(x) (((x) & 1) << 2)
#define V_VP1_OVERLAY_MODE(x) (((x) & 1) << 1)
#define V_VP0_OVERLAY_MODE(x) (((x) & 1) << 0)

/* VOP_LAYER_SEL */
#define M_LAYER0_SEL (7 << 0)
#define V_LAYER0_SEL(x) (((x) & 7) << 0)

/* VOP_PORT_SEL */
#define M_ESMART0_SEL_PORT (3 << 24)
#define V_ESMART0_SEL_PORT(x) (((x) & 3) << 24)

/* VOP_DSP_BG */
#define M_DSP_BG_RED     (0x3f << 20)
#define M_DSP_BG_GREEN   (0x3f << 10)
#define M_DSP_BG_BLUE    (0x3f << 0)

#define V_DSP_BG_RED(x)     (((x) & 0x3f) << 20)
#define V_DSP_BG_GREEN(x)   (((x) & 0x3f) << 10)
#define V_DSP_BG_BLUE(x)    (((x) & 0x3f) << 0)


/* VOP_POST */
#define M_POST_STANDBY (1 << 31)
#define M_POST_FP_STANDBY (1 << 30)
#define M_POST_BLACK (1 << 27)
#define M_POST_OUT_ZERO (1 << 26)

#define V_POST_STANDBY(x) (((x) & 1) << 31)
#define V_POST_FP_STANDBY(x) (((x) & 1) << 30)
#define V_POST_BLACK(x) (((x) & 1) << 27)
#define V_POST_OUT_ZERO(x) (((x) & 1) << 26)

/* DSP CTRL */
#define M_DSP_OUT_MODE              (0xf)
#define V_DSP_OUT_MODE(x)              ((x) & 0xf)

/* VOP_COLOR_BAR */
#define M_POST_COLORBAR_MODE (1 << 1)
#define M_POST_COLORBAR_EN (1 << 0)
#define V_POST_COLORBAR_MODE(x) (((x) & 1) << 1)
#define V_POST_COLORBAR_EN(x) (((x) & 1) << 0)

/* ESMART */
/* VOP2_ESMART_REGION0_VIR */
#define V_ARGB888_VIRWIDTH(x)	(((x) & 0x3fff) << 0)
#define V_RGB888_VIRWIDTH(x)	(((((x * 3) >> 2)+((x) % 3)) & 0x3fff) << 0)
#define V_RGB565_VIRWIDTH(x)	(((x / 2) & 0x3fff) << 0)
#define YUV_VIRWIDTH(x)		(((x / 4) & 0x3fff) << 0)

#define V_ACT_HEIGHT(x)         (((x) & 0x1fff) << 16)
#define V_ACT_WIDTH(x)          ((x) & 0x1fff)
#define V_DSP_HEIGHT(x)         (((x) & 0x1fff) << 16)
#define V_DSP_WIDTH(x)          ((x) & 0x1fff)
#define V_DSP_YST(x)         (((x) & 0x1fff) << 16)
#define V_DSP_XST(x)          ((x) & 0x1fff)

#define V_HSYNC(x)		(((x)&0x1fff)<<0)   /* hsync pulse width */
#define V_HORPRD(x)		(((x)&0x1fff)<<16)   /* horizontal period */
#define V_VSYNC(x)		(((x)&0x1fff)<<0)
#define V_VERPRD(x)		(((x)&0x1fff)<<16)

#define V_HEAP(x)		(((x)&0x1fff)<<0)/* horizontal active end */
#define V_HASP(x)		(((x)&0x1fff)<<16)/* horizontal active start */
#define V_VAEP(x)		(((x)&0x1fff)<<0)
#define V_VASP(x)		(((x)&0x1fff)<<16)

#endif
