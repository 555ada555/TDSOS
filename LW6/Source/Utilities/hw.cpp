
#include "definitions.h"
#include "hw.h"


#pragma code_seg("PAGE")
CSimpleAudioSampleHW::CSimpleAudioSampleHW()
: m_ulMux(0),
    m_bDevSpecific(FALSE),
    m_iDevSpecific(0),
    m_uiDevSpecific(0)

{
    PAGED_CODE();
    
    MixerReset();
} // SimpleAudioSampleHW
#pragma code_seg()


//=============================================================================
BOOL
CSimpleAudioSampleHW::bGetDevSpecific()

{
    return m_bDevSpecific;
} // bGetDevSpecific

//=============================================================================
void
CSimpleAudioSampleHW::bSetDevSpecific
(
    _In_  BOOL                bDevSpecific
)

{
    m_bDevSpecific = bDevSpecific;
} // bSetDevSpecific

//=============================================================================
INT
CSimpleAudioSampleHW::iGetDevSpecific()

{
    return m_iDevSpecific;
} // iGetDevSpecific

//=============================================================================
void
CSimpleAudioSampleHW::iSetDevSpecific
(
    _In_  INT                 iDevSpecific
)

{
    m_iDevSpecific = iDevSpecific;
} // iSetDevSpecific

//=============================================================================
UINT
CSimpleAudioSampleHW::uiGetDevSpecific()

{
    return m_uiDevSpecific;
} // uiGetDevSpecific

//=============================================================================
void
CSimpleAudioSampleHW::uiSetDevSpecific
(
    _In_  UINT                uiDevSpecific
)

{
    m_uiDevSpecific = uiDevSpecific;
} 
BOOL
CSimpleAudioSampleHW::GetMixerMute
(
    _In_  ULONG                   ulNode,
    _In_  ULONG                   ulChannel
)

{
    UNREFERENCED_PARAMETER(ulChannel);
    
    if (ulNode < MAX_TOPOLOGY_NODES)
    {
        return m_MuteControls[ulNode];
    }

    return 0;
} 
ULONG                       
CSimpleAudioSampleHW::GetMixerMux()

{
    return m_ulMux;
} 
LONG
CSimpleAudioSampleHW::GetMixerVolume
(   
    _In_  ULONG                   ulNode,
    _In_  ULONG                   ulChannel
)

{
    UNREFERENCED_PARAMETER(ulChannel);

    if (ulNode < MAX_TOPOLOGY_NODES)
    {
        return m_VolumeControls[ulNode];
    }

    return 0;
} // GetMixerVolume

//=============================================================================
LONG
CSimpleAudioSampleHW::GetMixerPeakMeter
(   
    _In_  ULONG                   ulNode,
    _In_  ULONG                   ulChannel
)

{
    UNREFERENCED_PARAMETER(ulChannel);

    if (ulNode < MAX_TOPOLOGY_NODES)
    {
        return m_PeakMeterControls[ulNode];
    }

    return 0;
} // GetMixerVolume

//=============================================================================
#pragma code_seg("PAGE")
void 
CSimpleAudioSampleHW::MixerReset()

{
    PAGED_CODE();
    
    RtlFillMemory(m_VolumeControls, sizeof(LONG) * MAX_TOPOLOGY_NODES, 0xFF);
    // Endpoints are not muted by default.
    RtlZeroMemory(m_MuteControls, sizeof(BOOL) * MAX_TOPOLOGY_NODES);

    for (ULONG i=0; i<MAX_TOPOLOGY_NODES; ++i)
    {
        m_PeakMeterControls[i] = PEAKMETER_SIGNED_MAXIMUM/2;
    }
    
    // BUGBUG change this depending on the topology
    m_ulMux = 2;
} // MixerReset
#pragma code_seg()

//=============================================================================
void
CSimpleAudioSampleHW::SetMixerMute
(
    _In_  ULONG                   ulNode,
    _In_  ULONG                   ulChannel,
    _In_  BOOL                    fMute
)

{
    UNREFERENCED_PARAMETER(ulChannel);

    if (ulNode < MAX_TOPOLOGY_NODES)
    {
        m_MuteControls[ulNode] = fMute;
    }
} // SetMixerMute

//=============================================================================
void                        
CSimpleAudioSampleHW::SetMixerMux
(
    _In_  ULONG                   ulNode
)

{
    m_ulMux = ulNode;
} // SetMixMux

//=============================================================================
void  
CSimpleAudioSampleHW::SetMixerVolume
(   
    _In_  ULONG                   ulNode,
    _In_  ULONG                   ulChannel,
    _In_  LONG                    lVolume
)

{
    UNREFERENCED_PARAMETER(ulChannel);

    if (ulNode < MAX_TOPOLOGY_NODES)
    {
        m_VolumeControls[ulNode] = lVolume;
    }
} // SetMixerVolume
