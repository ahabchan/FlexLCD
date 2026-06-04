#include "pic_hal.h"

static void pic_io_init(void *context)
{
	flexlcd_pic_context_t *hal_context = (flexlcd_pic_context_t *)context;
	// Set data and control pins as outputs
	*(hal_context->bus_tris) &= ~(hal_context->bus_mask);
	*(hal_context->bus_tris) &= ~(hal_context->rs);
	*(hal_context->bus_tris) &= ~(hal_context->en);
}

static void pic_set_control_line(void *context, enum enLCDControlPins line, bool value)
{
	flexlcd_pic_context_t *hal_context = (flexlcd_pic_context_t *)context;
	switch (line)
	{
	case E_RS_PIN:
		if (value)
			*(hal_context->bus_port) |= hal_context->rs;
		else
			*(hal_context->bus_port) &= ~(hal_context->rs);
		break;
	case E_EN_PIN:
		if (value)
			*(hal_context->bus_port) |= hal_context->en;
		else
			*(hal_context->bus_port) &= ~(hal_context->en);
		break;
	default:
		break;
	}
}

static void pic_write_bus(void *context, uint8_t data)
{
	flexlcd_pic_context_t *hal_context = (flexlcd_pic_context_t *)context;
	char temp = *(hal_context->bus_port);
	temp &= ~(hal_context->bus_mask);
	temp |= (data & 0x0F) << hal_context->bus_offset;
	*(hal_context->bus_port) = temp;
	// Pulse enable
	*(hal_context->bus_port) |= hal_context->en;
	__delay_us(10);
	*(hal_context->bus_port) &= ~(hal_context->en);
}

static void pic_delay_us(uint32_t us)
{
	while (us--)
		__delay_us(1);
}

void flexlcd_pic_init(gf_flexlcd_hal_t *hal, flexlcd_pic_context_t *context, volatile unsigned char *bus_port, volatile unsigned char *bus_tris, uint8_t bus_mask, uint8_t bus_offset, uint8_t rs_mask, uint8_t en_mask)
{
	context->bus_port = bus_port;
	context->bus_tris = bus_tris;
	context->bus_mask = bus_mask;
	context->bus_offset = bus_offset;
	context->rs = rs_mask;
	context->en = en_mask;

	hal->io_init = pic_io_init;
	hal->set_control_line = pic_set_control_line;
	hal->write_bus = pic_write_bus;
	hal->delay_us = pic_delay_us;
	hal->bus_mode = 4;
	hal->context = context;
}
