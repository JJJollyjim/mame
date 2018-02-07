// license:BSD-3-Clause
// copyright-holders:David Haywood
/***************************************************************************

    xavix.c

    6502 with custom opcodes
	integrated gfx / sound / coprocessor?

***************************************************************************/

#include "emu.h"
#include "xavix.h"
#include "xavixd.h"

DEFINE_DEVICE_TYPE(XAVIX, xavix_device, "xavix", "XaviX")

xavix_device::xavix_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	m6502_device(mconfig, XAVIX, tag, owner, clock),
	XPC(0)
{
	program_config.m_addr_width = 24;
	program_config.m_logaddr_width = 24;
	sprogram_config.m_addr_width = 24;
	sprogram_config.m_logaddr_width = 24;
}


util::disasm_interface *xavix_device::create_disassembler()
{
	return new xavix_disassembler;
}


void xavix_device::device_start()
{
	if(direct_disabled)
		mintf = std::make_unique<mi_xavix_nd>(this);
	else
		mintf = std::make_unique<mi_xavix_normal>(this);

	// bind delegates
	m_vector_callback.bind_relative_to(*owner());

	init();

	state_add(STATE_GENPC, "GENPC", XPC).callexport().noshow();
	state_add(STATE_GENPCBASE, "CURPC", XPC).callexport().noshow();
}

void xavix_device::state_export(const device_state_entry &entry)
{
	switch(entry.index()) {
	case STATE_GENPC:
	case STATE_GENPCBASE:
		XPC = adr_with_bank(NPC);
		break;
	}
}


void xavix_device::device_reset()
{
	m_farbank = 0;
	XPC = 0;
	m6502_device::device_reset();
}

xavix_device::mi_xavix_normal::mi_xavix_normal(xavix_device *_base)
{
	base = _base;
}


uint8_t xavix_device::mi_xavix_normal::read(uint16_t adr)
{
	if (adr < 0x8000)
		return program->read_byte(adr);
	else
		return program->read_byte(base->adr_with_bank(adr));
}

uint8_t xavix_device::mi_xavix_normal::read_sync(uint16_t adr)
{
	if (adr < 0x8000)
		return sdirect->read_byte(adr);
	else
		return sdirect->read_byte(base->adr_with_bank(adr));
}

uint8_t xavix_device::mi_xavix_normal::read_arg(uint16_t adr)
{
	if (adr < 0x8000)
		return direct->read_byte(adr);
	else
		return direct->read_byte(base->adr_with_bank(adr));
}

void xavix_device::mi_xavix_normal::write(uint16_t adr, uint8_t val)
{
	if (adr < 0x8000)
		program->write_byte(adr, val);
	else
		program->write_byte(base->adr_with_bank(adr), val);
}

xavix_device::mi_xavix_nd::mi_xavix_nd(xavix_device *_base) : mi_xavix_normal(_base)
{
}

uint8_t xavix_device::mi_xavix_nd::read_sync(uint16_t adr)
{
	if (adr < 0x8000)
		return sprogram->read_byte(adr);
	else
		return sprogram->read_byte(base->adr_with_bank(adr));
}

uint8_t xavix_device::mi_xavix_nd::read_arg(uint16_t adr)
{
	if (adr < 0x8000)
		return program->read_byte(adr);
	else
		return program->read_byte(base->adr_with_bank(adr));
}


#include "cpu/m6502/xavix.hxx"
