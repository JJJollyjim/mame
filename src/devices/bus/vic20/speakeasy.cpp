// license:BSD-3-Clause
// copyright-holders:smf
/**********************************************************************

    Personal Peripheral Products SpeakEasy cartridge emulation

**********************************************************************/

#include "speakeasy.h"



//**************************************************************************
//  MACROS/CONSTANTS
//**************************************************************************

#define SC01A_TAG       "sc01a"



//**************************************************************************
//  DEVICE DEFINITIONS
//**************************************************************************

const device_type VIC20_SPEAKEASY = &device_creator<vic20_speakeasy_t>;


//-------------------------------------------------
//  MACHINE_DRIVER( speakeasy )
//-------------------------------------------------

static MACHINE_CONFIG_FRAGMENT( speakeasy )
	MCFG_SPEAKER_ADD("mono", 0.0, 0.0, 1.0)

	MCFG_SOUND_ADD(SC01A_TAG, VOTRAX_SC01, 720000)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.85)
MACHINE_CONFIG_END


//-------------------------------------------------
//  machine_config_additions - device-specific
//  machine configurations
//-------------------------------------------------

machine_config_constructor vic20_speakeasy_t::device_mconfig_additions() const
{
	return MACHINE_CONFIG_NAME( speakeasy );
}



//**************************************************************************
//  LIVE DEVICE
//**************************************************************************

//-------------------------------------------------
//  vic20_speakeasy_t - constructor
//-------------------------------------------------

vic20_speakeasy_t::vic20_speakeasy_t(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock) :
	device_t(mconfig, VIC20_SPEAKEASY, "SpeakEasy", tag, owner, clock, "speakeasy", __FILE__),
	device_vic20_expansion_card_interface(mconfig, *this),
	m_votrax(*this, SC01A_TAG)
{
}


//-------------------------------------------------
//  device_start - device-specific startup
//-------------------------------------------------

void vic20_speakeasy_t::device_start()
{
}


//-------------------------------------------------
//  vic20_cd_r - cartridge data read
//-------------------------------------------------

uint8_t vic20_speakeasy_t::vic20_cd_r(address_space &space, offs_t offset, uint8_t data, int ram1, int ram2, int ram3, int blk1, int blk2, int blk3, int blk5, int io2, int io3)
{
	if (!io2)
	{
		return m_votrax->request() << 7;
	}

	return data;
}


//-------------------------------------------------
//  vic20_cd_w - cartridge data write
//-------------------------------------------------

void vic20_speakeasy_t::vic20_cd_w(address_space &space, offs_t offset, uint8_t data, int ram1, int ram2, int ram3, int blk1, int blk2, int blk3, int blk5, int io2, int io3)
{
	if (!io2)
	{
		m_votrax->write(space, 0, data & 0x3f);
		m_votrax->inflection_w(space, 0, data >> 6);
	}
}
